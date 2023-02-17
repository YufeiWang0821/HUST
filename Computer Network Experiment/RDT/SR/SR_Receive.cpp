#include"SR.h"

SR_Receive::SR_Receive() :rcv_base(0) {
	//初始状态下，上次发送的确认包的确认序号为-1，
	//使得当第一个接受的数据包出错时该确认报文的确认号为-1
	lastAckPkt.acknum = -1;
	lastAckPkt.checksum = 0;
	lastAckPkt.seqnum = -1; //忽略该字段
	for (int i = 0; i < seq_num; i++) { packetAcked[i] = false; }
	for (int i = 0; i < Configuration::PAYLOAD_SIZE; i++) {
		lastAckPkt.payload[i] = '.';
	}
	lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
}

SR_Receive::~SR_Receive() {}

void SR_Receive::receive(const Packet& packet) {

	//检查校验和是否正确
	int checkSum = pUtils->calculateCheckSum(packet);

	//如果校验和正确，同时收到报文的序号在窗口间
	if (checkSum == packet.checksum && (packet.seqnum - rcv_base + seq_num) % seq_num <= window_size - 1) {

		if (packetAcked[packet.seqnum] == false) {
			//该分组未收到并回应ack过
			packetAcked[packet.seqnum] = true;//改变对应的标志
			memcpy(buffered[packet.seqnum].data, packet.payload, sizeof(packet.payload));//放入缓存区
			pUtils->printPacket("接收方正确收到发送方的报文", packet);
		}

		if (packet.seqnum == rcv_base) {

			//输出滑动窗口前
			printf("接收窗口滑动前rcv_base=%d\n在窗口中的序号为：", rcv_base);
			for (int i = 0, j = rcv_base; i < seq_num; i++, j = (j + 1) % seq_num) { printf("%d ", j); }
			printf("\n");

			//分组序号等于接收方的基序号，说明接收窗口可以滑动，将缓冲区的数据全部向上递交给应用层
			int subnum = 0;//提交分组的个数
			for (int i = 0, j = rcv_base; i < seq_num; i++, j = (j + 1) % seq_num) {
				if (packetAcked[j] == false) { subnum = i; break; }
				//向上递交给应用层
				pns->delivertoAppLayer(RECEIVER, buffered[j]);
				packetAcked[j] = false;
			}
			printf("接收窗口滑动，向上递交了[%d,%d]的报文\n", rcv_base, (rcv_base + subnum - 1 + seq_num) % seq_num);
			rcv_base = (rcv_base + subnum) % seq_num;

			//输出滑动窗口后
			printf("接收窗口滑动后rcv_base=%d\n", rcv_base);
		}

		lastAckPkt.acknum = packet.seqnum; //确认序号等于收到的报文序号
		lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
		pUtils->printPacket("接收方发送确认报文", lastAckPkt);
		//调用模拟网络环境的sendToNetworkLayer，通过网络层发送确认报文到对方
		pns->sendToNetworkLayer(SENDER, lastAckPkt);
	}

	//校验和正确但是分组已经发送了ack
	else if (checkSum == packet.checksum && (packet.seqnum - (rcv_base - window_size + seq_num) % seq_num + seq_num) % seq_num <= window_size - 1) {
		lastAckPkt.acknum = packet.seqnum; //确认序号等于收到的报文序号
		lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
		printf("接受发收到的报文已发送过ACK\n");
		pUtils->printPacket("接收方发送确认报文", lastAckPkt);
		//调用模拟网络环境的sendToNetworkLayer，通过网络层发送确认报文到对方
		pns->sendToNetworkLayer(SENDER, lastAckPkt);
	}

	else {
		if (checkSum != packet.checksum) {
			pUtils->printPacket("接收方没有正确收到发送方的报文,数据校验错误", packet);
		}
		else {
			pUtils->printPacket("接收方没有正确收到发送方的报文,报文序号不对", packet);
		}
		pUtils->printPacket("接收方重新发送上次的确认报文", lastAckPkt);
		//调用模拟网络环境的sendToNetworkLayer，通过网络层发送上次的确认报文
		pns->sendToNetworkLayer(SENDER, lastAckPkt);
	}
}