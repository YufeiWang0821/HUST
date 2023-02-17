#include"GBN.h"

GBN_Send::GBN_Send():expectSequenceNumberSend(0), waitingState(false),base(0){}

GBN_Send::~GBN_Send(){}

bool GBN_Send::getWaitingState() {
	return waitingState;
}

bool GBN_Send::send(const Message& message) {
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

void GBN_Send::receive(const Packet& ackPkt) {
	//���У����Ƿ���ȷ
	int checkSum = pUtils->calculateCheckSum(ackPkt);

	//���У�����ȷ
	if (checkSum == ackPkt.checksum) {
		if ((ackPkt.acknum - base + seq_num) % seq_num < window_size) {
			pUtils->printPacket("���ͷ���ȷ�յ�ȷ��", ackPkt);

			//�����������ǰ
			printf("��������ǰbase=%d��nextseqnum=%d\n�ڴ����е����Ϊ��", base, expectSequenceNumberSend);
			int tempseq = (expectSequenceNumberSend >= base) ? expectSequenceNumberSend : (expectSequenceNumberSend + seq_num);
			for (int i = base; i < tempseq; i++) {
				printf("%d ", i%seq_num);
			}
			printf("\n");

			//��������
			this->waitingState = false;//�ı�ȴ�״̬�����Է����µķ���
			pns->stopTimer(SENDER, base);//�رյ�ǰ��ŵĶ�ʱ��
			base = (ackPkt.acknum + 1) % seq_num;//�����ƶ�

			//����������ں�
			printf("�������ں�base=%d��nextseqnum=%d\n�ڴ����е����Ϊ��", base, expectSequenceNumberSend);
			tempseq = (expectSequenceNumberSend >= base) ? expectSequenceNumberSend : (expectSequenceNumberSend + seq_num);
			for (int i = base; i < tempseq; i++) {
				printf("%d ", i % seq_num);
			}
			printf("\n");
			
			//�����ѷ��͵�δ��ȷ�ϵķ��飬����������ʱ��
			if (base != expectSequenceNumberSend) {
				pns->startTimer(SENDER, Configuration::TIME_OUT, base);
			}
		}
	}
	//else:���ͷ��յ��˴���ı��ģ���һֱ�ȴ�ֱ����ʱ�����ط�����
}

void GBN_Send::timeoutHandler(int seqNum) {
	printf("���ͷ���ʱ��ʱ�䵽���ط�[%d,%d]�ı���\n", base, expectSequenceNumberSend - 1);
	pns->stopTimer(SENDER, seqNum);//���ȹرն�ʱ��
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);//�����������ͷ���ʱ��
	int len = (expectSequenceNumberSend - base + seq_num) % seq_num;
	for (int i = 0, j = base; i < len; i++, j = (j + 1) % seq_num) {
		pUtils->printPacket("���·������ݰ�", packetWaitingAck[j]);
		pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[j]);		//���·������ݰ�
	}
}