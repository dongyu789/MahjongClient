/**
 * @file Mahjong.h
 * @author hufeiyu
 * @brief 麻将的数字
 * @version 0.1
 * @date 2022-01-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MAHJONG_H
#define MAHJONG_H

//筒
#define ONEDOT 1
#define TWODOT 2
#define THREEDOT 3
#define FOURDOT 4
#define FIVEDOT 5
#define SIXDOT 6
#define SEVENDOT 7
#define EIGHTDOT 8
#define NINEDOT 9
//万字
#define ONECHARACTER 11
#define TWOCHARACTER 12
#define THREECHARACTER 13
#define FOURCHARACTER 14
#define FIVECHARACTER 15
#define SIXCHARACTER 16
#define SEVENCHARACTER 17
#define EIGHTCHARACTER 18
#define NINECHARACTER 19
//条字
#define ONEBAMBOO 21
#define TWOBAMBOO 22
#define THREEBAMBOO 23
#define FOURBAMBOO 24
#define FIVEBAMBOO 25
#define SIXBAMBOO 26
#define SEVENBAMBOO 27
#define EIGHTBAMBOO 28
#define NINEBAMBOO 29


//东南西北中发白 
#define EAST 31
#define SOUTH 33
#define WEST 35
#define NORTH 37
#define RED 39
#define GREEN 41
#define WHITEDRAGON 43

//碰杠吃胡
#define CHOW 20
#define PONG 21
#define KONG 22
#define WIN 23
#define ANKONG 24//暗杠
#define ANWIN 25//自摸

#include <list>
#include <stack>
#include <vector>
#include <unordered_map>
using namespace std;

struct PongKongChow {
    PongKongChow();
    vector<int> mahjong;
    int lightIndex = -1;
    int flag = -1;
};  

class Mahjong {
public:
    Mahjong();   
    ~Mahjong();
    list<int> myMahjong;
    vector<int> deskMahjong;
    vector<PongKongChow*> myPongKongChow;

    //各家的碰杠吃
    vector<PongKongChow*> leftPongKongChow;
    vector<PongKongChow*> rightPongKongChow;
    vector<PongKongChow*> oppositePongKongChow;
    
    int remainMahjong;//剩余的麻将
    int selectLocation = 0;//我选择哪个麻将

    //四家对应的编号，controller的时候由服务器发送，然后决定如何打印
    int myNum;//自己的编号，东南西北位置
    int leftNum;//上家编号
    int rightNum;//下家编号
    int oppositeNum;//对门编号

    string myName;
    string leftName;
    string rightName;
    string oppositeName;

    unordered_map<int, string> allPlayerNumToName;//各家编号对应的名字

    //各家剩余牌数
    int myCount = 0;
    int leftCount = 0;
    int rightCount = 0;
    int oppositeCount = 0;

    int ifMyPermissionDiscard = 0;//是否我的出牌权
    int ifMyPermissionPKCW = 0;//是否我的决策权


private:


};



#endif