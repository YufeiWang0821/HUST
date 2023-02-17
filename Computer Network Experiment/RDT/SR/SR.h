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
	int expectSequenceNumberSend; // 下一个发送序号
	bool waitingState; // 是否处于等待Ack的状态
	Packet packetWaitingAck[seq_num]; //已发送并等待Ack的数据包
	int send_base;//基序号
	int packetAcked[seq_num];//已收到Ack
public:
	bool getWaitingState();
	//发送应用层下来的Message，由NetworkServiceSimulator调用,
	//如果发送方成功地将Message发送到网络层，返回true;
	//如果因为发送方处于等待正确确认状态而拒绝发送Message，则返回false
	bool send(const Message& message);
	//接受确认Ack，将被NetworkServiceSimulator调用
	void receive(const Packet& ackPkt);
	//Timeout handler，将被NetworkServiceSimulator调用
	void timeoutHandler(int seqNum);
public:
	SR_Send();
	virtual ~SR_Send();
};

class SR_Receive : public RdtReceiver {
private:
	int rcv_base;//基序号
	Packet lastAckPkt;//上次发送的确认报文
	int packetAcked[seq_num];//已发送Ack
	Message buffered[seq_num];//分组缓存区
public:
	SR_Receive();
	virtual ~SR_Receive();
public:
	void receive(const Packet& packet);//接收报文，将被NetworkService调用
};