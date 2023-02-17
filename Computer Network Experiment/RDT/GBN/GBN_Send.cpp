#include"GBN.h"

GBN_Send::GBN_Send():expectSequenceNumberSend(0), waitingState(false),base(0){}

GBN_Send::~GBN_Send(){}

bool GBN_Send::getWaitingState() {
	return waitingState;
}

bool GBN_Send::send(const Message& message) {
	if (this->waitingState) { //发送方处于等待确认状态
		return false;
	}
	this->packetWaitingAck[expectSequenceNumberSend].acknum = -1; //忽略该字段
	this->packetWaitingAck[expectSequenceNumberSend].seqnum = this->expectSequenceNumberSend;
	this->packetWaitingAck[expectSequenceNumberSend].checksum = 0;
	memcpy(this->packetWaitingAck[expectSequenceNumberSend].payload, message.data, sizeof(message.data));
	this->packetWaitingAck[expectSequenceNumberSend].checksum = pUtils->calculateCheckSum(this->packetWaitingAck[expectSequenceNumberSend]);
	pUtils->printPacket("发送方发送报文", this->packetWaitingAck[expectSequenceNumberSend]);
	//调用模拟网络环境的sendToNetworkLayer，通过网络层发送到对方
	pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[expectSequenceNumberSend]);

	if (base == expectSequenceNumberSend) {
		//启动发送方定时器
		pns->startTimer(SENDER, Configuration::TIME_OUT, this->packetWaitingAck[expectSequenceNumberSend].seqnum);
	}
	expectSequenceNumberSend = (expectSequenceNumberSend + 1) % seq_num;
	if ((expectSequenceNumberSend - base + seq_num) % seq_num >= window_size) {
		//当前窗口已满，不能发送新的packet，进入等待状态
		this->waitingState = true;
	}
	return true;
}

void GBN_Send::receive(const Packet& ackPkt) {
	//检查校验和是否正确
	int checkSum = pUtils->calculateCheckSum(ackPkt);

	//如果校验和正确
	if (checkSum == ackPkt.checksum) {
		if ((ackPkt.acknum - base + seq_num) % seq_num < window_size) {
			pUtils->printPacket("发送方正确收到确认", ackPkt);

			//输出滑动窗口前
			printf("滑动窗口前base=%d，nextseqnum=%d\n在窗口中的序号为：", base, expectSequenceNumberSend);
			int tempseq = (expectSequenceNumberSend >= base) ? expectSequenceNumberSend : (expectSequenceNumberSend + seq_num);
			for (int i = base; i < tempseq; i++) {
				printf("%d ", i%seq_num);
			}
			printf("\n");

			//滑动窗口
			this->waitingState = false;//改变等待状态，可以发送新的分组
			pns->stopTimer(SENDER, base);//关闭当前序号的定时器
			base = (ackPkt.acknum + 1) % seq_num;//窗口移动

			//输出滑动窗口后
			printf("滑动窗口后base=%d，nextseqnum=%d\n在窗口中的序号为：", base, expectSequenceNumberSend);
			tempseq = (expectSequenceNumberSend >= base) ? expectSequenceNumberSend : (expectSequenceNumberSend + seq_num);
			for (int i = base; i < tempseq; i++) {
				printf("%d ", i % seq_num);
			}
			printf("\n");
			
			//仍有已发送但未被确认的分组，重新启动定时器
			if (base != expectSequenceNumberSend) {
				pns->startTimer(SENDER, Configuration::TIME_OUT, base);
			}
		}
	}
	//else:发送方收到了错误的报文，则一直等待直到超时进行重发处理
}

void GBN_Send::timeoutHandler(int seqNum) {
	printf("发送方定时器时间到，重发[%d,%d]的报文\n", base, expectSequenceNumberSend - 1);
	pns->stopTimer(SENDER, seqNum);//首先关闭定时器
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);//重新启动发送方定时器
	int len = (expectSequenceNumberSend - base + seq_num) % seq_num;
	for (int i = 0, j = base; i < len; i++, j = (j + 1) % seq_num) {
		pUtils->printPacket("重新发送数据包", packetWaitingAck[j]);
		pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[j]);		//重新发送数据包
	}
}