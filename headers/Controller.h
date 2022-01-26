/**
 * @file Controller.h
 * @author 胡飞宇
 * @brief 控制相关
 * @version 0.1
 * @date 2022-01-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Mahjong.h"
#include "MyPrint.h"
#include "MySocket.h"
#include "MyTest.h"
#include "Win.h"
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ENTER 13
#define FLASH 102
#define LEFT_PKCW 97
#define RIGHT_PKCW 100
#define LEFT_PKCW_A 65
#define RIGHT_PKCW_D 68

//OPTION操作
#define CLIENT_BREAK 1  //用户断开read error
#define CLIENT_CLOSED 2 //用户主动关闭，用户退出
#define SELECT_DESK 10  //选择桌子
#define SELECT_DESK_SUCCESS 11
#define SELECT_DESK_FAIL 12
#define DISCARD 13
#define DEAL 14 //发牌
#define DRAW 15 //摸牌
#define GAME_OVER 16
#define GAME_BEGIN 17
#define JUDGE 18 //每个人看牌是否需要处理
#define NORMAL 19


class Controller {
public:

    Controller();
    ~Controller();

    void start();//游戏启动，准备开始，准备数据

    void playGame();//正式开始游戏

    void selectMahjong();//选择麻将的子线程

    void throwDice();//掷色子

    void selectDesk(int deskNum, string name);

    void doSelect();//移动光标

    void deal();//发牌

    void draw();//摸牌

    void discard();//出牌

    int judgePKCW();//判断吃碰杠赢

    int selectPKCW();//选择吃碰杠赢

    int delOneMahjongFromLocation(int index);//删掉一个指定位置的麻将,不能连起来用,注意这个要改后期

    int delOneMahjongFromNum(int num);//删掉一个指定值的麻将

    int judgePongKong();//碰杠牌

    int judgeAnKongAnWin();//判断暗杠自摸，就是在自己出牌的时候判断

    int judgeAnKong();//判断暗杠

    int judgeAnWin();//判断自摸

    void judgeKong();//杠牌

    void judgeChow();//吃牌

    void judgeWin();//赢W

    int judgePongKongWin();//碰杠赢

    int getDeskOnlineNum();//获取本桌当前在线人数

    int sendMsg(Message &message);

    int handleServerMsg();//接收处理服务器消息

    void refrashScreen(int microsecond);//卡主就刷新屏幕f

    Message* recvMsg();

    void handlePongKong();

    void fillName();

    void handleAnkong();//处理暗杠

    void handleWin();

    void handleChow();

    int whoDiscardLastNum = -1;


    void testPrint();



    void test();//测试函数
    void testSocket();
private:
    MyPrint *myPrint;
    Mahjong *mahjong;
    MySocket *mysocket;

    
    int pongBit = 0;
    int kongBit = 0;
    int chowBit = 0;
    int winBit = 0;

    int myAnKongMahjong = 0;//我暗杠的那个麻将
    vector<int> allAnKongMahjong;//所有可能暗杠的麻将
    vector<WinMessage*> allWinner;//赢家信息，可以一炮多响
    

    int findNotZero(vector<int> &arr, int begin, int arrSize, int leftRoRight);//找到左边第一个或者右边第一个非零元素下标

    int blockSelectMahjong = 0;//阻塞选择线程
};


#endif