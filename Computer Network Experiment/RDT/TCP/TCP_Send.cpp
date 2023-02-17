#include"TCP.h"

TCP_Send::TCP_Send() :expectSequenceNumberSend(0), waitingState(false), base(0), y(-1), dupAck(0) {}

TCP_Send::~TCP_Send() {}

bool TCP_Send::getWaitingState() {
	return waitingState;
}

bool TCP_Send::send(const Message& message) {
	if (this->waitingState) { //���ͷ����ڵȴ�ȷ��״̬
		return false;
	}
	this->packetWaitingAck[expectSequenceNumberSend].acknum = -1; //���Ը��ֶ�
	this->packetWaitingAck[expectSequenceNumberSend].seqnum = this->expectSequenceNumberSend;
	this->packetWaitingAck[expectSequenceNumberSend].checksum = 0;
	memcpy(this->packetWaitingAck[expectSequenceNumberSend].payload, message.data, sizeof(message.data));
	this->packetWaitingAck[expectSequenceNumberSend].checksum = pUtils->calculateCheckSum(this->packetWaitingAck[expectSequenceNumberSend]);
	pUtils->printPacket("���ͷ����ͱ���", this->packetWaitingAck[expectSequenceNumberSend]);
	//����ģ�����绷����sendToNetworkLayer��ͨ������㷢�͵��Է�
	pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[expectSequenceNumberSend]);

	if (base == expectSequenceNumberSend) {
		//�������ͷ���ʱ��
		pns->startTimer(SENDER, Configuration::TIME_OUT, this->packetWaitingAck[expectSequenceNumberSend].seqnum);
	}
	expectSequenceNumberSend = (expectSequenceNumberSend + 1) % seq_num;
	if ((expectSequenceNumberSend - base + seq_num) % seq_num >= window_size) {
		//��ǰ�������������ܷ����µ�packet������ȴ�״̬
		this->waitingState = true;
	}
	return true;
}

void TCP_Send::receive(const Packet& ackPkt) {
	//���У����Ƿ���ȷ
	int checkSum = pUtils->calculateCheckSum(ackPkt);

	//���У�����ȷ
	if (checkSum == ackPkt.checksum) {
		y = ackPkt.acknum;
		if (((y - base + seq_num) % seq_num < window_size)&&y!=base) {
			this->waitingState = false;
			pUtils->printPacket("���ͷ���ȷ�յ�ȷ��", ackPkt);

			//�����������ǰ
			printf("��������ǰbase=%d��nextseqnum=%d\n�ڴ����е����Ϊ��", base, expectSequenceNumberSend);
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

			//����������ں�
			printf("�������ں�base=%d��nextseqnum=%d\n�ڴ����е����Ϊ��", base, expectSequenceNumberSend);
			tempseq = (expectSequenceNumberSend >= base) ? expectSequenceNumberSend : (expectSequenceNumberSend + seq_num);
			for (int i = base; i < tempseq; i++) {
				printf("%d ", i % seq_num);
			}
			printf("\n");

		}
		else {
			//�յ�����ACK
			this->dupAck++;
			printf("���ͷ��յ���%d������ACK����seqnumΪ%d\n", dupAck, ackPkt.acknum);
			if (dupAck == 3) {
				//�����ش�
				pUtils->printPacket("���ͷ��յ���������ACK�����п����ش�", this->packetWaitingAck[y]);
				pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[y]);
				y = -1, dupAck = 0;
			}
		}
	}
	//else:���ͷ��յ��˴���ı��ģ���һֱ�ȴ�ֱ����ʱ�����ط�����
}

void TCP_Send::timeoutHandler(int seqNum) {
	pns->stopTimer(SENDER, seqNum);										//���ȹرն�ʱ��
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);			//�����������ͷ���ʱ��
	pUtils->printPacket("���ͷ���ʱ��ʱ�䵽���ط�����ı���", this->packetWaitingAck[base]);
	pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[base]);		//���·������ݰ�
}