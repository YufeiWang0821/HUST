#pragma once
#include "winsock2.h"
#include <stdio.h>
#include <iostream>
using namespace std;

#pragma comment(lib,"ws2_32.lib")

//对监听端口、监听地址、主目录的定义
int port = 80;
char inaddr[20] = "";
char mcatalog[100] = "";

//响应报文首部的字符串常量定义
//请求成功的响应报文首部
const char* header_200 = "HTTP/1.1 200 OK \r\n\Accept-Ranges:bytes\r\nContent-Length:%d\r\nConnection:close\r\nContent-Type:%s\r\n\r\n";
//请求失败的响应报文首部
const char* header_404 = "HTTP/1.1 404 Not Found \r\n\Accept-Ranges:bytes\r\nContent-Length:%d\r\nConnection:close\r\nContent-Type:%s\r\n\r\n";


//在长度为len的请求报文recvBuf中提取文件名filename
int extract_filename(char* recvBuf, char* filename) {
	memset(filename, 0, 1000);
	int i = 0, j = 0;
	while (recvBuf[i] != '/') { i++; }
	while (recvBuf[i + 1] != ' ') {
		filename[j++] = recvBuf[i + 1];
		i++;
	}
	filename[j] = '\0';
	return 0;
}

//获取文件后缀
char* get_file_suffix(char* filename) {
	char filesuf[20];
	int flag = 0, j = 0, filenamelen = strlen(filename);
	for (int i = 0; i < filenamelen; i++) {
		if (filename[i] == '.') { flag = 1; continue; }
		if (flag) { filesuf[j++] = filename[i]; }
	}
	filesuf[j] = '\0';
	return filesuf;
}

//获取文件类型
char contype[20];
void get_content_type(char* filesuf) {
	memset(contype, 0, 20);
	strcpy(contype, "text/plain");
	if (!strcmp(filesuf, "html")) { strcpy(contype, "text/html"); }
	if (!strcmp(filesuf, "gif")) { strcpy(contype, "image/gif"); }
	if (!strcmp(filesuf, "jpg")) { strcpy(contype, "image/jpg"); }
	if (!strcmp(filesuf, "png")) { strcpy(contype, "image/png"); }
}

//发送包含响应信息的响应报文
int send_respnonse_message(SOCKET socket, char* recvBuf, char* filename) {

	//获取文件类型
	char* filesuf = get_file_suffix(filename);
	get_content_type(filesuf);

	//设置文件路径
	char file_header[200];
	strcpy(file_header, header_200);
	char filepath[1000];
	strcpy(filepath, mcatalog);
	strcat(filepath, filename);

	//打开文件
	FILE* rfile = fopen(filepath, "rb");
	if (!rfile) {
		printf("\n%s does not exist!\nPlease check the input!\n", filepath);
		printf("Sending error.html instead!\n");
		rfile = fopen("error.html", "rb");
		strcpy(file_header, header_404);
		strcpy(contype, "text/html");
	}

	//计算响应文件的大小
	fseek(rfile, 0, SEEK_END);
	int filelen = ftell(rfile);
	fseek(rfile, 0, SEEK_SET);

	//生成并发送响应报文首部
	char resheader[1000];
	int headerlen = sprintf(resheader, file_header, filelen, contype);
	int sendlen = send(socket, resheader, headerlen, 0);
	if (sendlen == SOCKET_ERROR) {
		printf("Sending %s error!\n", filename);
		fclose(rfile);
		return -1;
	}

	//发送响应报文的文件
	char* resfilebuf = (char*)malloc(filelen + 1);
	fread(resfilebuf, filelen, 1, rfile);
	send(socket, resfilebuf, filelen, 0);

	//关闭文件并返回
	fclose(rfile);
	return 1;
}

void main() {

	//对监听端口、监听地址、主目录请求输入
	printf("Please input Listening Port:");
	scanf("%d", &port);
	printf("Please input Listening Address:");
	scanf("%s", inaddr);
	printf("Please input Master Catalog:");
	scanf("%s", mcatalog);

	WSADATA wsaData;

	int nRc = WSAStartup(0x0202, &wsaData);

	if (nRc) {
		printf("Winsock  startup failed with error!\n");
	}

	if (wsaData.wVersion != 0x0202) {
		printf("Winsock version is not correct!\n");
	}

	printf("Winsock  startup OK!\n");


	//监听socket
	SOCKET srvSocket;

	//服务器地址和客户端地址
	sockaddr_in addr, clientAddr;

	//会话socket，负责和client进程通信
	SOCKET sessionSocket;

	//ip地址长度
	int addrLen;

	//创建监听socket
	srvSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (srvSocket != INVALID_SOCKET)
		printf("Socket create OK!\n");


	//设置服务器的端口和地址
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = inet_addr(inaddr); //主机上任意一块网卡的IP地址


	//binding
	int rtn = bind(srvSocket, (LPSOCKADDR)&addr, sizeof(addr));
	if (rtn != SOCKET_ERROR)
		printf("Socket bind OK!\n");

	//监听
	rtn = listen(srvSocket, 5);
	if (rtn != SOCKET_ERROR)
		printf("Socket listen OK!\n");

	clientAddr.sin_family = AF_INET;
	addrLen = sizeof(clientAddr);

	//设置接收缓冲区
	char recvBuf[4096];

	while (true) {

		//产生会话SOCKET
		sessionSocket = accept(srvSocket, (LPSOCKADDR)&clientAddr, &addrLen);
		if (sessionSocket != INVALID_SOCKET) {
			printf("\nSocket listen one client request!\n");
			printf("The port of the request is %d\n", clientAddr.sin_port);
			printf("The IP Address of the request is %ul\n", clientAddr.sin_addr.S_un.S_addr);
		}
		else { printf("\nSession Socket error!\n"); break; }

		//receiving data from client
		memset(recvBuf, '\0', 4096);
		rtn = recv(sessionSocket, recvBuf, 4096, 0);
		if (rtn >= 0) {

			//打印接收到的报文
			printf("\nReceived %d bytes from client:\n", rtn);
			printf("%s\n", recvBuf);

			//定义并从报文中获取文件名
			char filename[1000];
			extract_filename(recvBuf, filename);

			//处理请求报文并发送响应报文
			if ((rtn = send_respnonse_message(sessionSocket, recvBuf, filename) == SOCKET_ERROR)) { //发送响应报文失败
				cout << "send_respnonse_message() failed with error!\n";
				break;
			}
			else { printf("Sent to client successfully!\n\n"); }
		}
		closesocket(sessionSocket);
	}
	closesocket(srvSocket);
	WSACleanup();

}