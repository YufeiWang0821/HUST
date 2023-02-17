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

class TCP_Send :public RdtSender {
private:
	int expectSequenceNumberSend; // ��һ���������
	bool waitingState; // �Ƿ��ڵȴ�Ack��״̬
	Packet packetWaitingAck[seq_num]; //�ѷ��Ͳ��ȴ�Ack�����ݰ�
	int base;//�����
	int y;
	int dupAck;//����ACK����
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
	TCP_Send();
	virtual ~TCP_Send();
};

class TCP_Receive : public RdtReceiver {
private:
	int expectSequenceNumberRcvd;// �ڴ��յ�����һ���������
	Packet lastAckPkt;//�ϴη��͵�ȷ�ϱ���
public:
	TCP_Receive();
	virtual ~TCP_Receive();
public:
	void receive(const Packet& packet);//���ձ��ģ�����NetworkService����
};