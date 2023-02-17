#pragma once
#include"../quote/StopWait/DataStructure.h"
#include"../quote/StopWait/Global.h"
#include"../quote/StopWait/NetworkService.h"
#include"../quote/StopWait/RandomEventEnum.h"
#include"../quote/StopWait/RdtReceiver.h"
#include"../quote/StopWait/RdtSender.h"
#include"../quote/StopWait/stdafx.h"
#include"../quote/StopWait/targetver.h"
#include"../quote/StopWait/Tool.h"

#include<stdio.h>
#include<string.h>
#include<iostream>

#define window_size 4
#define seq_bit 3
#define seq_num 8

class SR_Send :public RdtSender {
private:
	int expectSequenceNumberSend; // ��һ���������
	bool waitingState; // �Ƿ��ڵȴ�Ack��״̬
	Packet packetWaitingAck[seq_num]; //�ѷ��Ͳ��ȴ�Ack�����ݰ�
	int send_base;//�����
	int packetAcked[seq_num];//���յ�Ack
public:
	bool getWaitingState();
	//����Ӧ�ò�������Message����NetworkServiceSimulator����,
	//������ͷ��ɹ��ؽ�Message���͵�����㣬����true;
	//�����Ϊ���ͷ����ڵȴ���ȷȷ��״̬���ܾ�����Message���򷵻�false
	bool send(const Message& message);
	//����ȷ��Ack������NetworkServiceSimulator����
	void receive(const Packet& ackPkt);
	//Timeout handler������NetworkServiceSimulator����
	void timeoutHandler(int seqNum);
public:
	SR_Send();
	virtual ~SR_Send();
};

class SR_Receive : public RdtReceiver {
private:
	int rcv_base;//�����
	Packet lastAckPkt;//�ϴη��͵�ȷ�ϱ���
	int packetAcked[seq_num];//�ѷ���Ack
	Message buffered[seq_num];//���黺����
public:
	SR_Receive();
	virtual ~SR_Receive();
public:
	void receive(const Packet& packet);//���ձ��ģ�����NetworkService����
};