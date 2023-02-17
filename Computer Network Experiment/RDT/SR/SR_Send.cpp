#include"SR.h"

SR_Send::SR_Send() :expectSequenceNumberSend(0), waitingState(false), send_base(0) {
	for (int i = 0; i < seq_num; i++) { packetAcked[i] = false; }
}

SR_Send::~SR_Send() {}

bool SR_Send::getWaitingState() {
	return waitingState;
}

bool SR_Send::send(const Message& message) {
	if (this->waitingState) { //发送方处于等待确认状态
		return false;
	}
	packetAcked[expectSequenceNumberSend] = false;
	this->packetWaitingAck[expectSequenceNumberSend].acknum = -1; //忽略该字段
	this->packetWaitingAck[expectSequenceNumberSend].seqnum = this->expectSequenceNumberSend;
	this->packetWaitingAck[expectSequenceNumberSend].checksum = 0;
	memcpy(this->packetWaitingAck[expectSequenceNumberSend].payload, message.data, sizeof(message.data));
	this->packetWaitingAck[expectSequenceNumberSend].checksum = pUtils->calculateCheckSum(this->packetWaitingAck[expectSequenceNumberSend]);
	pUtils->printPacket("发送方发送报文", this->packetWaitingAck[expectSequenceNumberSend]);
	//调用模拟网络环境的sendToNetworkLayer，通过网络层发送到对方
	pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[expectSequenceNumberSend]);
	pns->startTimer(SENDER, Configuration::TIME_OUT, this->packetWaitingAck[expectSequenceNumberSend].seqnum);
	expectSequenceNumberSend = (expectSequenceNumberSend + 1) % seq_num;
	if ((expectSequenceNumberSend - send_base + seq_num) % seq_num >= window_size) {
		//进入等待状态
		this->waitingState = true;
	}
	return true;
}

void SR_Send::receive(const Packet& ackPkt) {
	//检查校验和是否正确
	int checkSum = pUtils->calculateCheckSum(ackPkt);

	//如果校验和正确
 	if (checkSum == ackPkt.checksum) {
		if (packetAcked[ackPkt.acknum] == false && (ackPkt.acknum - send_base + seq_num) % seq_num < window_size) {
			pns->stopTimer(SENDER, ackPkt.acknum);//关闭当前序号的定时器
			pUtils->printPacket("发送方正确收到确认", ackPkt);
			packetAcked[ackPkt.acknum] = true;

			//滑动窗口
			int templen = (expectSequenceNumberSend - send_base + seq_num) % seq_num;
			if (send_base == ackPkt.acknum) {

				//输出滑动窗口前
				printf("发送窗口滑动前base=%d，nextseqnum=%d\n在窗口中的序号为：", send_base, expectSequenceNumberSend);
				int tempseq = (expectSequenceNumberSend >= send_base) ? expectSequenceNumberSend : (expectSequenceNumberSend + seq_num);
				for (int i = send_base; i < tempseq; i++) {
					printf("%d ", i % seq_num);
				}
				printf("\n");

				for (int i = 0, j = send_base; i < templen; i++, j = (j + 1) % seq_num) {
					if (packetAcked[j] == false) { break; }
					packetAcked[j] = false;
					send_base = (send_base + 1) % seq_num;//窗口移动
				}
				this->waitingState = false;//改变等待状态，可以发送新的分组

				//输出滑动窗口后
				printf("发送窗口滑动后base=%d，nextseqnum=%d\n在窗口中的序号为：", send_base, expectSequenceNumberSend);
				tempseq = (expectSequenceNumberSend >= send_base) ? expectSequenceNumberSend : (expectSequenceNumberSend + seq_num);
				for (int i = send_base; i < tempseq; i++) {
					printf("%d ", i % seq_num);
				}
				printf("\n");

			}
		}
	}
	//else:发送方收到了错误的报文，则一直等待直到超时进行重发处理
}

void SR_Send::timeoutHandler(int seqNum) {
	printf("发送方定时器时间到，重发%d的报文\n", seqNum);
	pns->stopTimer(SENDER, seqNum);//首先关闭定时器
	pUtils->printPacket("重新发送数据包", packetWaitingAck[seqNum]);
	pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[seqNum]);//重新发送数据包
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);//重新启动发送方定时器
}