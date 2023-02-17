#include"TCP.h"

TCP_Send::TCP_Send() :expectSequenceNumberSend(0), waitingState(false), base(0), y(-1), dupAck(0) {}

TCP_Send::~TCP_Send() {}

bool TCP_Send::getWaitingState() {
	return waitingState;
}

bool TCP_Send::send(const Message& message) {
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

void TCP_Send::receive(const Packet& ackPkt) {
	//检查校验和是否正确
	int checkSum = pUtils->calculateCheckSum(ackPkt);

	//如果校验和正确
	if (checkSum == ackPkt.checksum) {
		y = ackPkt.acknum;
		if (((y - base + seq_num) % seq_num < window_size)&&y!=base) {
			this->waitingState = false;
			pUtils->printPacket("发送方正确收到确认", ackPkt);

			//输出滑动窗口前
			printf("滑动窗口前base=%d，nextseqnum=%d\n在窗口中的序号为：", base, expectSequenceNumberSend);
			int tempseq = (expectSequenceNumberSend >= base) ? expectSequenceNumberSend : (expectSequenceNumberSend + seq_num);
			for (int i = base; i < tempseq; i++) {
				printf("%d ", i % seq_num);
			}
			printf("\n");
			
			if (y == expectSequenceNumberSend) { pns->stopTimer(SENDER, base); }
			else {
				pns->stopTimer(SENDER, base);
				pns->startTimer(SENDER, Configuration::TIME_OUT, y);
			}
			dupAck = 0;
			base = y;

			//输出滑动窗口后
			printf("滑动窗口后base=%d，nextseqnum=%d\n在窗口中的序号为：", base, expectSequenceNumberSend);
			tempseq = (expectSequenceNumberSend >= base) ? expectSequenceNumberSend : (expectSequenceNumberSend + seq_num);
			for (int i = base; i < tempseq; i++) {
				printf("%d ", i % seq_num);
			}
			printf("\n");

		}
		else {
			//收到冗余ACK
			this->dupAck++;
			printf("发送方收到第%d个冗余ACK，其seqnum为%d\n", dupAck, ackPkt.acknum);
			if (dupAck == 3) {
				//快速重传
				pUtils->printPacket("发送方收到三个冗余ACK，进行快速重传", this->packetWaitingAck[y]);
				pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[y]);
				y = -1, dupAck = 0;
			}
		}
	}
	//else:发送方收到了错误的报文，则一直等待直到超时进行重发处理
}

void TCP_Send::timeoutHandler(int seqNum) {
	pns->stopTimer(SENDER, seqNum);										//首先关闭定时器
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);			//重新启动发送方定时器
	pUtils->printPacket("发送方定时器时间到，重发最早的报文", this->packetWaitingAck[base]);
	pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[base]);		//重新发送数据包
}