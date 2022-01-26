/**
 * @file myPrint.h
 * @author hufeiyu
 * @brief 打印相关
 * @version 0.1
 * @date 2022-01-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MYPRINT_H
#define MYPRINT_H

//定义几个打印定位点,左上角是0,0
#define COLS 100
#define LINES 50
#define LEFTDOWNPOINT_X 0
#define LEFTDOWNPOINT_Y LINES-6
#define DESKMAHJONG_X COLS/5 //桌上麻将定位点
#define DESKMAHJONG_Y 6 //桌上麻将定位点
#define DESKMAHJONG_ONE_ROW_NUM 20 //桌上麻将一行打印几个
#define MY_PONGKONGCHOW_X 0 //碰杠吃定位点
#define MY_PONGKONGCHOW_Y LEFTDOWNPOINT_Y-6 //碰杠吃定位点
#define LEFT_PONGKONGCHOW_X 5 //碰杠吃定位点
#define LEFT_PONGKONGCHOW_Y 0 //碰杠吃定位点
#define RIGHT_PONGKONGCHOW_X COLS-12 //碰杠吃定位点
#define RIGHT_PONGKONGCHOW_Y 0 //碰杠吃定位点
#define OPPOSITE_PONGKONGCHOW_X 24 //碰杠吃定位点
#define OPPOSITE_PONGKONGCHOW_Y 2 //碰杠吃定位点
#define WINNER_X DESKMAHJONG_X //胡牌定位点
#define WINNER_Y DESKMAHJONG_Y //胡牌定位点



#define MY_COUNT_X          0                //剩余牌数定位点
#define MY_COUNT_Y          0                //剩余牌数定位点
#define LEFT_COUNT_X        0              //剩余牌数定位点
#define LEFT_COUNT_Y        LINES/2-10     //剩余牌数定位点
#define RIGHT_COUNT_X       COLS-2        //剩余牌数定位点
#define RIGHT_COUNT_Y       LINES/2-10    //剩余牌数定位点
#define OPPOSITE_COUNT_X    COLS/2-10       //剩余牌数定位点
#define OPPOSITE_COUNT_Y    0          //剩余牌数定位点

#define LEFT_NAME_X LEFT_COUNT_X 
#define LEFT_NAME_Y LEFT_COUNT_Y+20-3
#define RIGHT_NAME_X RIGHT_COUNT_X-5
#define RIGHT_NAME_Y LEFT_NAME_Y
#define OPPOSITE_NAME_X OPPOSITE_COUNT_X
#define OPPOSITE_NAME_Y 2
#define MY_NAME_X COLS*0.9-18
#define MY_NAME_Y LINES-2


#define PONG_BOTTON_X   0
#define PONG_BOTTON_Y   MY_PONGKONGCHOW_Y-3
#define KONG_BOTTON_X   5
#define KONG_BOTTON_Y   MY_PONGKONGCHOW_Y-3
#define CHOW_BOTTON_X   10
#define CHOW_BOTTON_Y   MY_PONGKONGCHOW_Y-3
#define WIN_BOTTON_X   15
#define WIN_BOTTON_Y   MY_PONGKONGCHOW_Y-3
#define CANCEL_BOTTON_X 20
#define CANCEL_BOTTON_Y MY_PONGKONGCHOW_Y-3
#define DEFAULT_BOTTON_X 25
#define DEFAULT_BOTTPN_Y MY_PONGKONGCHOW_Y-3



//打印类别标识
#define PRINTMAHJONG_MINE 1
#define PRINTMAHJONG_DESK 2
#define PRINTMAHJONG_PENGKONGCHOW 3
#define PRINTDESK_LIGHT 1
#define PRINTDESK_DARK  0

//玩家打印位置标识
#define LEFT_PLAYER 1
#define RIGHT_PLAYER 2
#define OPPOSITE_PLAYER 3
#define MYSELF_PLAYER 4



#include <unordered_map>
#include <list>
#include <stack>
#include <vector>
#include <Mahjong.h>
#include "Win.h"
using namespace std;
/**
 * @brief 
 * 跟打印有关的类
 * 比如打印麻将块，光标变动
 */
class MyPrint {
public:
    MyPrint();
    
    /**
     * @brief 打印一个麻将
     * 
     * @param id 麻将id
     * @param x 坐标
     * @param y 
     * @param flag 麻将类型，手牌、桌上牌、碰吃杠牌
     */
    void printOneMahjong(int id, int x, int y, int flag);

    /**
     * @brief Set the Window Size object
     * 
     * @param cols 多少列
     * @param lines 多少行
     */
    void setWindowSize(int cols, int lines);

    /**
     * @brief Set the Cursor Position object
     * 
     * @param x 坐标
     * @param y 坐标
     */
    void setCursorPosition(double x, int y);

    //设置颜色
    void setColor(int colorID);

    //打印我的所有麻将
    void printMyAllMahjong(const list<int> &myMahjong, int i = 0);

    //打印桌子上的麻将
    void printDeskMahjong(const vector<int> &deskMahjong, int flag = PRINTDESK_LIGHT);

    //打印碰杠吃
    void printPongKongChow(vector<PongKongChow*> allPongKongChow, int who = MYSELF_PLAYER);

    //打印各家剩下的麻将，只打印外形即可
    void printOtherCount(int count = 0, int who = LEFT_PLAYER);

    //打印一个麻将的背面
    void printBack(int x, int y, int who);

    //打印桌子轮廓
    void printDesk();

    //输出碰杠吃赢的选项,返回亮的选项，给controller控制
    int printPKCWBotton(vector<int> &pkcwArr, int lightIndex = 4);

    //打印桌子上的全部,设置打出来的最后一张是否为亮色
    void printAll(Mahjong* mahjong, int who, int light = PRINTDESK_LIGHT);

    void printTurnYou(Mahjong *mahjong);

    void printTurnYouOver(Mahjong *mahjong);

    void printAnKongChose(vector<int> &anKongArr, int lightIndex = 3);

    void printWinner(vector<WinMessage*> &wms);

    void printPlayerName(Mahjong *mahjong, int who);



    

private:
    unordered_map<int, string> mahjongName;
    
};

#endif