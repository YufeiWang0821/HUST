#include"SR.h"

SR_Send::SR_Send() :expectSequenceNumberSend(0), waitingState(false), send_base(0) {
	for (int i = 0; i < seq_num; i++) { packetAcked[i] = false; }
}

SR_Send::~SR_Send() {}

bool SR_Send::getWaitingState() {
	return waitingState;
}

bool SR_Send::send(const Message& message) {
	if (this->waitingState) { //���ͷ����ڵȴ�ȷ��״̬
		return false;
	}
	packetAcked[expectSequenceNumberSend] = false;
	this->packetWaitingAck[expectSequenceNumberSend].acknum = -1; //���Ը��ֶ�
	this->packetWaitingAck[expectSequenceNumberSend].seqnum = this->expectSequenceNumberSend;
	this->packetWaitingAck[expectSequenceNumberSend].checksum = 0;
	memcpy(this->packetWaitingAck[expectSequenceNumberSend].payload, message.data, sizeof(message.data));
	this->packetWaitingAck[expectSequenceNumberSend].checksum = pUtils->calculateCheckSum(this->packetWaitingAck[expectSequenceNumberSend]);
	pUtils->printPacket("���ͷ����ͱ���", this->packetWaitingAck[expectSequenceNumberSend]);
	//����ģ�����绷����sendToNetworkLayer��ͨ������㷢�͵��Է�
	pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[expectSequenceNumberSend]);
	pns->startTimer(SENDER, Configuration::TIME_OUT, this->packetWaitingAck[expectSequenceNumberSend].seqnum);
	expectSequenceNumberSend = (expectSequenceNumberSend + 1) % seq_num;
	if ((expectSequenceNumberSend - send_base + seq_num) % seq_num >= window_size) {
		//����ȴ�״̬
		this->waitingState = true;
	}
	return true;
}

void SR_Send::receive(const Packet& ackPkt) {
	//���У����Ƿ���ȷ
	int checkSum = pUtils->calculateCheckSum(ackPkt);

	//���У�����ȷ
 	if (checkSum == ackPkt.checksum) {
		if (packetAcked[ackPkt.acknum] == false && (ackPkt.acknum - send_base + seq_num) % seq_num < window_size) {
			pns->stopTimer(SENDER, ackPkt.acknum);//�رյ�ǰ��ŵĶ�ʱ��
			pUtils->printPacket("���ͷ���ȷ�յ�ȷ��", ackPkt);
			packetAcked[ackPkt.acknum] = true;

			//��������
			int templen = (expectSequenceNumberSend - send_base + seq_num) % seq_num;
			if (send_base == ackPkt.acknum) {

				//�����������ǰ
				printf("���ʹ��ڻ���ǰbase=%d��nextseqnum=%d\n�ڴ����е����Ϊ��", send_base, expectSequenceNumberSend);
				int tempseq = (expectSequenceNumberSend >= send_base) ? expectSequenceNumberSend : (expectSequenceNumberSend + seq_num);
				for (int i = send_base; i < tempseq; i++) {
					printf("%d ", i % seq_num);
				}
				printf("\n");

				for (int i = 0, j = send_base; i < templen; i++, j = (j + 1) % seq_num) {
					if (packetAcked[j] == false) { break; }
					packetAcked[j] = false;
					send_base = (send_base + 1) % seq_num;//�����ƶ�
				}
				this->waitingState = false;//�ı�ȴ�״̬�����Է����µķ���

				//����������ں�
				printf("���ʹ��ڻ�����base=%d��nextseqnum=%d\n�ڴ����е����Ϊ��", send_base, expectSequenceNumberSend);
				tempseq = (expectSequenceNumberSend >= send_base) ? expectSequenceNumberSend : (expectSequenceNumberSend + seq_num);
				for (int i = send_base; i < tempseq; i++) {
					printf("%d ", i % seq_num);
				}
				printf("\n");

			}
		}
	}
	//else:���ͷ��յ��˴���ı��ģ���һֱ�ȴ�ֱ����ʱ�����ط�����
}

void SR_Send::timeoutHandler(int seqNum) {
	printf("���ͷ���ʱ��ʱ�䵽���ط�%d�ı���\n", seqNum);
	pns->stopTimer(SENDER, seqNum);//���ȹرն�ʱ��
	pUtils->printPacket("���·������ݰ�", packetWaitingAck[seqNum]);
	pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[seqNum]);//���·������ݰ�
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);//�����������ͷ���ʱ��
}