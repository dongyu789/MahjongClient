/**
 * @file Controller.cpp
 * @author hufeiyu 
 * @brief 
 * @version 0.1
 * @date 2022-01-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <WINSOCK2.H>
#include <STDIO.H>
#include <iostream>
#include <cstring>
#include <vector>
#include <thread>
#include <time.h>
#include <windows.h>
#include "MySocket.h"
using namespace std;
#pragma comment(lib, "ws2_32.lib")

MySocket::MySocket() {
    MySocket::buffSize = sizeof(Message);
}


//类中的静态变量需要在类外再定义
Message MySocket::message;
SOCKET MySocket::sclient;
sockaddr_in MySocket::serAddr;
WORD MySocket::sockVersion;
WSADATA MySocket::data;
int MySocket::buffSize = sizeof(Message);

int MySocket::connectToServer() {

    //初始化socket
    sockVersion = MAKEWORD(2, 2);

	if(WSAStartup(sockVersion, &data)!=0)
	{
		printf("wsa error");
        return -1;
	}
	

    sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sclient == INVALID_SOCKET)
    {
        printf("invalid socket!");
        return -1;
    }
    

    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(SERVER_PORT);
    serAddr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
    if(connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
    {  //连接失败 
        printf("connect error !");
        closesocket(sclient);
        return -1;
    }

    return 0;

}


Message& MySocket::recvMsg() {
    //接收字节数设置大一点，保证可以接收完一个结构体
    char buff[4096] = {0};
    int len = recv(sclient, buff, sizeof(buff), 0);

    memset(&message, 0, sizeof(message));
    memcpy(&message, buff, sizeof(message));

    if(len == 0) {
        getchar();
        system("cls");
        printf("服务器断开链接..3秒后自动关闭客户端..\n");
        Sleep(3000);
        exit(0);
    } else if(len == -1) {
        printf("读取错误");
        perror("read");
    }

    return message;
    
}

void MySocket::sendMsg(Message &message) {
    char buff[4096] = {0};
    memset(buff, 0, sizeof(buff));
    memcpy(buff, &message, sizeof(message));
    send(sclient, buff, sizeof(buff), 0);
}


void MySocket::closeConnect() {
    closesocket(sclient);
    WSACleanup();
}