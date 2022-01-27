/**
 * @file MySocket.h
 * @author hufeiyu
 * @brief 通信相关
 * @version 0.1
 * @date 2022-01-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MYSOCKET_H
#define MYSOCKET_H
#include <WINSOCK2.H>
#include <STDIO.H>
#include <iostream>
#include <cstring>
#include <vector>
#include <thread>
using namespace std;
#pragma comment(lib, "ws2_32.lib")


#define SERVER_IP "172.28.167.206"
#define SERVER_PORT 9998

//用来和服务器传输的消息
struct Message{
    int mahjong;//麻将编号
    int remainMahjong;//剩余的麻将
    int diceNum;//色子大小
    int myNum;//自己的编号
    int leftNum;//上家编号
    int rightNum;//下家编号
    int oppositeNum;//对家编号
    char myName[20] = {0};
    char leftName[20] = {0};
    char rightName[20] = {0};
    char oppositeName[20] = {0};
    bool Pong; //废弃
    bool Kong;  //废弃
    bool Chow;  //废弃
    bool Win;   //废弃
    int nowNum = -1;//这次出牌人的编号
    int allOnlineNum;//当前在线人数
    int deskNum;//玩家的桌号
    int deskOnlineNum;//当前桌子在线人数
    int OPTION;//此次通信进行的操作

    //赢字段
    int winPlayerMahjongSize = 0;
    int winPlayerMahjong[15];
    
    
};

class MySocket {
public:
    MySocket();


    //链接客户端
    static int connectToServer();

    static void closeConnect();

    //发消息,注意这里是静态函数
    static void sendMsg(Message &message);

    //接收消息
    static Message& recvMsg();

    //消息结构体
    static Message message;

    //通信套接字
    static SOCKET sclient;

    static sockaddr_in serAddr;
    static WORD sockVersion;
	static WSADATA data;

    static int buffSize;;

};


#endif
