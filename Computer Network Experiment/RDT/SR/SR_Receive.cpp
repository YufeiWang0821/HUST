#include"SR.h"

SR_Receive::SR_Receive() :rcv_base(0) {
	//��ʼ״̬�£��ϴη��͵�ȷ�ϰ���ȷ�����Ϊ-1��
	//ʹ�õ���һ�����ܵ����ݰ�����ʱ��ȷ�ϱ��ĵ�ȷ�Ϻ�Ϊ-1
	lastAckPkt.acknum = -1;
	lastAckPkt.checksum = 0;
	lastAckPkt.seqnum = -1; //���Ը��ֶ�
	for (int i = 0; i < seq_num; i++) { packetAcked[i] = false; }
	for (int i = 0; i < Configuration::PAYLOAD_SIZE; i++) {
		lastAckPkt.payload[i] = '.';
	}
	lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
}

SR_Receive::~SR_Receive() {}

void SR_Receive::receive(const Packet& packet) {

	//���У����Ƿ���ȷ
	int checkSum = pUtils->calculateCheckSum(packet);

	//���У�����ȷ��ͬʱ�յ����ĵ�����ڴ��ڼ�
	if (checkSum == packet.checksum && (packet.seqnum - rcv_base + seq_num) % seq_num <= window_size - 1) {

		if (packetAcked[packet.seqnum] == false) {
			//�÷���δ�յ�����Ӧack��
			packetAcked[packet.seqnum] = true;//�ı��Ӧ�ı�־
			memcpy(buffered[packet.seqnum].data, packet.payload, sizeof(packet.payload));//���뻺����
			pUtils->printPacket("���շ���ȷ�յ����ͷ��ı���", packet);
		}

		if (packet.seqnum == rcv_base) {

			//�����������ǰ
			printf("���մ��ڻ���ǰrcv_base=%d\n�ڴ����е����Ϊ��", rcv_base);
			for (int i = 0, j = rcv_base; i < seq_num; i++, j = (j + 1) % seq_num) { printf("%d ", j); }
			printf("\n");

			//������ŵ��ڽ��շ��Ļ���ţ�˵�����մ��ڿ��Ի�������������������ȫ�����ϵݽ���Ӧ�ò�
			int subnum = 0;//�ύ����ĸ���
			for (int i = 0, j = rcv_base; i < seq_num; i++, j = (j + 1) % seq_num) {
				if (packetAcked[j] == false) { subnum = i; break; }
				//���ϵݽ���Ӧ�ò�
				pns->delivertoAppLayer(RECEIVER, buffered[j]);
				packetAcked[j] = false;
			}
			printf("���մ��ڻ��������ϵݽ���[%d,%d]�ı���\n", rcv_base, (rcv_base + subnum - 1 + seq_num) % seq_num);
			rcv_base = (rcv_base + subnum) % seq_num;

			//����������ں�
			printf("���մ��ڻ�����rcv_base=%d\n", rcv_base);
		}

		lastAckPkt.acknum = packet.seqnum; //ȷ����ŵ����յ��ı������
		lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
		pUtils->printPacket("���շ�����ȷ�ϱ���", lastAckPkt);
		//����ģ�����绷����sendToNetworkLayer��ͨ������㷢��ȷ�ϱ��ĵ��Է�
		pns->sendToNetworkLayer(SENDER, lastAckPkt);
	}

	//У�����ȷ���Ƿ����Ѿ�������ack
	else if (checkSum == packet.checksum && (packet.seqnum - (rcv_base - window_size + seq_num) % seq_num + seq_num) % seq_num <= window_size - 1) {
		lastAckPkt.acknum = packet.seqnum; //ȷ����ŵ����յ��ı������
		lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
		printf("���ܷ��յ��ı����ѷ��͹�ACK\n");
		pUtils->printPacket("���շ�����ȷ�ϱ���", lastAckPkt);
		//����ģ�����绷����sendToNetworkLayer��ͨ������㷢��ȷ�ϱ��ĵ��Է�
		pns->sendToNetworkLayer(SENDER, lastAckPkt);
	}

	else {
		if (checkSum != packet.checksum) {
			pUtils->printPacket("���շ�û����ȷ�յ����ͷ��ı���,����У�����", packet);
		}
		else {
			pUtils->printPacket("���շ�û����ȷ�յ����ͷ��ı���,������Ų���", packet);
		}
		pUtils->printPacket("���շ����·����ϴε�ȷ�ϱ���", lastAckPkt);
		//����ģ�����绷����sendToNetworkLayer��ͨ������㷢���ϴε�ȷ�ϱ���
		pns->sendToNetworkLayer(SENDER, lastAckPkt);
	}
}