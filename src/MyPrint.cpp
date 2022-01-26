/**
 * @file Controller.cpp
 * @author hufeiyu 
 * @brief 
 * @version 0.1
 * @date 2022-01-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "MyPrint.h"
#include "Mahjong.h"
#include "Controller.h"
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

using namespace std;
//只有打印的时候需要展示，所以应该放在MyPrint类
MyPrint::MyPrint() {
    mahjongName[ONEDOT] = "一筒";
    mahjongName[TWODOT] = "二筒";
    mahjongName[THREEDOT] = "三筒";
    mahjongName[FOURDOT] = "四筒";
    mahjongName[FIVEDOT] = "五筒";
    mahjongName[SIXDOT] = "六筒";
    mahjongName[SEVENDOT] = "七筒";
    mahjongName[EIGHTDOT] = "八筒";
    mahjongName[NINEDOT] = "九筒";

    mahjongName[ONECHARACTER] = "一萬";
    mahjongName[TWOCHARACTER] = "二萬";
    mahjongName[THREECHARACTER] = "三萬";
    mahjongName[FOURCHARACTER] = "四萬";
    mahjongName[FIVECHARACTER] = "五萬";
    mahjongName[SIXCHARACTER] = "六萬";
    mahjongName[SEVENCHARACTER] = "七萬";
    mahjongName[EIGHTCHARACTER] = "八萬";
    mahjongName[NINECHARACTER] = "九萬";

    mahjongName[ONEBAMBOO] = "一条";
    mahjongName[TWOBAMBOO] = "二条";
    mahjongName[THREEBAMBOO] = "三条";
    mahjongName[FOURBAMBOO] = "四条";
    mahjongName[FIVEBAMBOO] = "五条";
    mahjongName[SIXBAMBOO] = "六条";
    mahjongName[SEVENBAMBOO] = "七条";
    mahjongName[EIGHTBAMBOO] = "八条";
    mahjongName[NINEBAMBOO] = "九条";

    mahjongName[EAST] = "东风";
    mahjongName[SOUTH] = "南风";
    mahjongName[WEST] = "西风";
    mahjongName[NORTH] = "北风";
    mahjongName[RED] = "红中";
    mahjongName[GREEN] = "发财";
    mahjongName[WHITEDRAGON] = "白板";


}

void MyPrint::setWindowSize(int cols = COLS, int lines = LINES) {
    system("麻将");//设置窗口标题
    char cmd[30];
    sprintf(cmd, "mode con cols=%d lines=%d", cols *2, lines);
    system(cmd);
}

void MyPrint::setCursorPosition(double x, int y) {
    //设置光标的位置
    COORD position;
    position.X = x*2;
    position.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

void MyPrint::printOneMahjong(int id, int x, int y, int flag) {
    if(id >= 1 && id <= 9) {
        setColor(3);
    } else if(id >= 11 && id <= 19) {
        setColor(4);
    } else if(id >= 21 && id <= 29) {
        setColor(5);
    } else {
        setColor(6);
    }
    string name = mahjongName[id];
    switch(flag) {
        case PRINTMAHJONG_MINE:
            setCursorPosition(x,y);
            printf(" ------ ");
            for(int i = 1; i < 5; i++) {
                setCursorPosition(x,y+i);
                printf("|      |");
            }
            setCursorPosition(x,y+5);
            printf(" ------ ");

            setCursorPosition(x+1,y+2);
            printf("%s",name.c_str());
            break;
        case PRINTMAHJONG_DESK:
            setCursorPosition(x,y);
            printf(" ---- ");
            for(int i = 1; i < 3; i++) {
                setCursorPosition(x,y+i);
                printf("|    |");
            }
            setCursorPosition(x,y+3);
            printf(" ---- ");

            setCursorPosition(x+0.5,y+1);
            printf("%s",name.c_str());
            break;
        case PRINTMAHJONG_PENGKONGCHOW:
            setCursorPosition(x,y);
            printf(" ---- ");
            for(int i = 1; i < 3; i++) {
                setCursorPosition(x,y+i);
                printf("|    |");
            }
            setCursorPosition(x,y+3);
            printf(" ---- ");

            setCursorPosition(x+0.5,y+1);
            printf("%s",name.c_str());
            break;
    }
    setColor(3);
}

void MyPrint::setColor(int colorID) {
    //设置文本颜色
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorID);
}

void MyPrint::printMyAllMahjong(const list<int> &myMahjong, int i) {    

    int step = 0;
    int num = 0;
    for(auto iter = myMahjong.begin(); iter != myMahjong.end(); iter++, step+=5, num++) {
        int id = *iter;
        if(num == i) {
            //打印覆盖
            setCursorPosition(LEFTDOWNPOINT_X + step - 5, LEFTDOWNPOINT_Y -1);
            printf("                               ");
            setCursorPosition(LEFTDOWNPOINT_X + step + 5, LEFTDOWNPOINT_Y -1);
            printf("                               ");
            setCursorPosition(LEFTDOWNPOINT_X + step, LEFTDOWNPOINT_Y+5);
            printf("         ");
            if(num == 0) {
                setCursorPosition(LEFTDOWNPOINT_X + 5*(myMahjong.size()-1), LEFTDOWNPOINT_Y-1);
                printf("          ");
            } else if(num == myMahjong.size()-1) {
                setCursorPosition(LEFTDOWNPOINT_X, LEFTDOWNPOINT_Y-1);
                printf("          ");
            }
            setColor(23);
            printOneMahjong(id, LEFTDOWNPOINT_X + step, LEFTDOWNPOINT_Y-1, PRINTMAHJONG_MINE);
            setColor(3);
            continue;
        } 
        printOneMahjong(id, LEFTDOWNPOINT_X + step, LEFTDOWNPOINT_Y, PRINTMAHJONG_MINE);
    }
    setCursorPosition(COLS, LINES);
}

/**
 * @brief 打印所有桌上已经打出的麻将，并且把最后一个打出的置为显色,
 * 
 * @param deskMahjong 桌上的麻将数组
 * @param flag 是否把最后一个置为显色
 */
void MyPrint::printDeskMahjong(const vector<int> &deskMahjong, int flag) {
    
    int column = DESKMAHJONG_ONE_ROW_NUM;
    int xstep = 0;
    int ystep = 0;
    setColor(3);
    for(int i = 0, n = deskMahjong.size(); i < n; i++, xstep += 3) {
        printOneMahjong(deskMahjong[i], DESKMAHJONG_X + xstep, DESKMAHJONG_Y + ystep, PRINTMAHJONG_DESK);
        if(i == n-1) {
            if(flag == PRINTDESK_LIGHT) {
                setColor(23);
            }
            printOneMahjong(deskMahjong[i], DESKMAHJONG_X + xstep, DESKMAHJONG_Y + ystep, PRINTMAHJONG_DESK);
            setColor(3);
        }
        if((i + 1) % column == 0) {
            ystep += 4;
            xstep = -3;
            printf("\n");
        }
    }
    
}
/**
 * @brief 打印所有碰杠吃
 * 
 * @param allPongKongChow 
 * @param who 打印谁的
 */
void MyPrint::printPongKongChow(vector<PongKongChow*> allPongKongChow, int who) {

    int positionX;
    int positionY;
    int oldPostitonX;
    int xstep = 0;
    int ystep = 0;
    int xaddstep = 0;//步增
    int yaddstep = 0;//步增
    switch(who) {
        case MYSELF_PLAYER:
            xaddstep = 3;
            yaddstep = 4;
            positionX = MY_PONGKONGCHOW_X;
            positionY = MY_PONGKONGCHOW_Y;
            break;
        case LEFT_PLAYER:
            xaddstep = 3;
            yaddstep = 4;
            positionX = LEFT_PONGKONGCHOW_X;
            positionY = LEFT_PONGKONGCHOW_Y;
            break;
        case RIGHT_PLAYER:
            xaddstep = 3;
            yaddstep = 4;
            positionX = RIGHT_PONGKONGCHOW_X;
            positionY = RIGHT_PONGKONGCHOW_Y;
            break;
        case OPPOSITE_PLAYER:
            xaddstep = 3;
            yaddstep = 4;
            positionX = OPPOSITE_PONGKONGCHOW_X;
            positionY = OPPOSITE_PONGKONGCHOW_Y;
            break;
    }

    oldPostitonX = positionX;
    if(who == MYSELF_PLAYER || who == OPPOSITE_PLAYER) {
        for(auto onePKC : allPongKongChow) {
            switch(onePKC->flag) {
                case PONG:
                    setColor(3);
                    for(int i = 0, n = onePKC->mahjong.size(); i < n; i++, xstep += xaddstep) {
                        if(i == onePKC->lightIndex) {
                            setColor(27);
                        }
                        printOneMahjong(onePKC->mahjong[i], positionX + xstep, positionY, PRINTMAHJONG_PENGKONGCHOW);
                        if(i == onePKC->lightIndex) {
                            setColor(3);
                        }

                    }
                    xstep += 1;
                    break;
                case KONG:
                    setColor(7);
                    for(int i = 0, n = onePKC->mahjong.size(); i < n; i++, xstep += xaddstep) {
                        if(i == onePKC->lightIndex) {
                            setColor(27);
                        }
                        printOneMahjong(onePKC->mahjong[i], positionX + xstep, positionY, PRINTMAHJONG_PENGKONGCHOW);
                        if(i == onePKC->lightIndex) {
                            setColor(7);
                        }

                    }
                    xstep += 1;
                    break;
                case CHOW:
                    setColor(6);
                    for(int i = 0, n = onePKC->mahjong.size(); i < n; i++, xstep += xaddstep) {
                        printOneMahjong(onePKC->mahjong[i], positionX + xstep, positionY, PRINTMAHJONG_PENGKONGCHOW);
                    }
                    xstep += 1;
                    break;
            }
        }
    } else {
        int count = 0;
        for(auto onePKC : allPongKongChow) {
            if(count % 2 == 0 && count != 0) {
                xstep += xaddstep + 1;
                ystep = 0;
            }
            count++;
            switch(onePKC->flag) {
                case PONG:
                    setColor(3);
                    for(int i = 0, n = onePKC->mahjong.size(); i < n; i++, ystep += yaddstep) {
                        if(i == onePKC->lightIndex) {
                            setColor(27);
                        }
                        printOneMahjong(onePKC->mahjong[i], positionX + xstep, positionY+ystep, PRINTMAHJONG_PENGKONGCHOW);
                        if(i == onePKC->lightIndex) {
                            setColor(3);
                        }

                    }
                    ystep += 1;

                    break;
                case KONG:
                    setColor(7);
                    for(int i = 0, n = onePKC->mahjong.size(); i < n; i++, ystep += yaddstep) {
                        if(i == onePKC->lightIndex) {
                            setColor(27);
                        }
                        printOneMahjong(onePKC->mahjong[i], positionX + xstep, positionY + ystep, PRINTMAHJONG_PENGKONGCHOW);
                        if(i == onePKC->lightIndex) {
                            setColor(7);
                        }

                    }
                    ystep += 1;
                    break;
                case CHOW:
                    setColor(6);
                    for(int i = 0, n = onePKC->mahjong.size(); i < n; i++, ystep += yaddstep) {
                        printOneMahjong(onePKC->mahjong[i], positionX + xstep, positionY + ystep, PRINTMAHJONG_PENGKONGCHOW);
                    }
                    ystep += 1;
                    break;
            }
        }
    }
    
}


void MyPrint::printOtherCount(int count, int who) {
    double positionX;
    int positionY;
    int xstep = 0;
    int ystep = 0;
    switch(who) {
        case LEFT_PLAYER:
            positionX = LEFT_COUNT_X;
            positionY = LEFT_COUNT_Y;
            break;
        case RIGHT_PLAYER:
            positionX = RIGHT_COUNT_X;
            positionY = RIGHT_COUNT_Y;
            break;
        case OPPOSITE_PLAYER:
            positionX = OPPOSITE_COUNT_X;
            positionY = OPPOSITE_COUNT_Y;
            break;
    }

    for(int i = 0; i <= count; i++) {
        if(i == count) {
            setCursorPosition(positionX, positionY);
            printf("%d", count);
            break;
        }
        printBack(positionX, positionY, who);
        if(who == OPPOSITE_PLAYER) {
            positionX += 1;
        } else {
            positionY += 1;
        }
    }

}

void MyPrint::printPlayerName(Mahjong *mahjong, int who) {
    //谁出牌谁亮
    who = MySocket::message.nowNum;
    setCursorPosition(LEFT_NAME_X, LEFT_NAME_Y);
    if(who == mahjong->leftNum) {
        setColor(23);
        cout << mahjong->allPlayerNumToName[mahjong->leftNum];
        setColor(3);
    } else {
        cout << mahjong->allPlayerNumToName[mahjong->leftNum];
    }
    

    setCursorPosition(RIGHT_NAME_X, RIGHT_NAME_Y);
    if(who == mahjong->rightNum) {
        setColor(23);
        cout << mahjong->allPlayerNumToName[mahjong->rightNum];
        setColor(3);
    } else {
        cout << mahjong->allPlayerNumToName[mahjong->rightNum];
    }
    

    setCursorPosition(OPPOSITE_NAME_X, OPPOSITE_NAME_Y);
    if(who == mahjong->oppositeNum) {
        setColor(23);
        cout << mahjong->allPlayerNumToName[mahjong->oppositeNum];
        setColor(3);
    } else {    
        cout << mahjong->allPlayerNumToName[mahjong->oppositeNum];
    }
    

    setCursorPosition(MY_NAME_X, MY_NAME_Y);
    if(who == mahjong->myNum) {
        setColor(23);
        cout << mahjong->allPlayerNumToName[mahjong->myNum];
        setColor(3);
    } else {
        cout << mahjong->allPlayerNumToName[mahjong->myNum];
    }
    

}

void MyPrint::printBack(int x, int y, int who) {
    if(who == OPPOSITE_PLAYER) {
        setCursorPosition(x, y);
        printf("︻");
        setCursorPosition(x, y+1);
        printf("︼");
    } else {
        setCursorPosition(x, y);
        printf("【");
        setCursorPosition(x+1, y);
        printf("】");
    }
}

void MyPrint::printWinner(vector<WinMessage*> &wms) {
    //先打印出手牌，再打印出碰杠吃
    int size = wms.size();
    int xstep = 0;
    
    int ystep = 0;
    for(int i = 0; i < size; i++, ystep += 6) {
        xstep = -4;
        setCursorPosition(WINNER_X + xstep, WINNER_Y + ystep - 1);
        printf("%d %s 赢了!", wms[i]->winnerNum, wms[i]->winnerName.c_str());
        xstep = 0;
       
        for(auto j : wms[i]->winPlayerMahjong) {
            setCursorPosition(WINNER_X + xstep, WINNER_Y + ystep);
            printOneMahjong(j, WINNER_X + xstep, WINNER_Y + ystep, PRINTMAHJONG_DESK);
            xstep += 3;
        }
        printPongKongChow(wms[i]->winPKC, wms[i]->winnerNum);

    }
}


void MyPrint::printAll(Mahjong* mahjong, int light, int who) {
    printPlayerName(mahjong, who);
    
    printMyAllMahjong(mahjong->myMahjong, mahjong->selectLocation);
    printDeskMahjong(mahjong->deskMahjong, light);
    printTurnYou(mahjong);

    printPongKongChow(mahjong->myPongKongChow, MYSELF_PLAYER);
    printPongKongChow(mahjong->leftPongKongChow, LEFT_PLAYER);
    printPongKongChow(mahjong->rightPongKongChow, RIGHT_PLAYER);
    printPongKongChow(mahjong->oppositePongKongChow, OPPOSITE_PLAYER);

    printOtherCount(mahjong->leftCount, LEFT_PLAYER);
    printOtherCount(mahjong->rightCount, RIGHT_PLAYER);
    printOtherCount(mahjong->oppositeCount, OPPOSITE_PLAYER);

}

void MyPrint::printAnKongChose(vector<int> &anKongArr, int lightIndex) {

    setCursorPosition(PONG_BOTTON_X, PONG_BOTTON_Y);
    printf("                                                                                                  ");

    if(lightIndex == 4) {
        setCursorPosition(DEFAULT_BOTTON_X, DEFAULT_BOTTPN_Y);
        setColor(23);
        printf("(左右键选择回车键发送size:%d, index:%d)", anKongArr.size(), lightIndex);  
        setColor(3);
    } else {
        printf("(左右键选择回车键发送size:%d, index:%d)", anKongArr.size(), lightIndex); 
    }
    
    for(int i = 0; i<anKongArr.size(); i++) {
        setCursorPosition(PONG_BOTTON_X + 5*i, PONG_BOTTON_Y);
        if(lightIndex == i) {
            setColor(23);
            if(i == anKongArr.size()-1) {
                printf("取消!");  
            }else {
                printf("%s", mahjongName[anKongArr[i]].c_str());  
            }
            setColor(3);
        }else {
            if(i == anKongArr.size()-1) {
                printf("取消!");  
            }else {
                printf("%s", mahjongName[anKongArr[i]].c_str());  
            }
        }
    }
}

int MyPrint::printPKCWBotton(vector<int> &pkcwArr, int lightIndex) {

    setCursorPosition(PONG_BOTTON_X, PONG_BOTTON_Y);
    printf("                                                                                   ");

    if(pkcwArr[0]) {
        setCursorPosition(PONG_BOTTON_X, PONG_BOTTON_Y);
        if(lightIndex == 0) {
            setColor(23);
            printf("碰!");  
            setColor(3);
        }else {
            printf("碰!");
        }
    } 

    if(pkcwArr[1]) {
        setCursorPosition(KONG_BOTTON_X, KONG_BOTTON_Y);
        if(lightIndex == 1) {
            setColor(23);
            printf("杠!");  
            setColor(3);
        }else {
            printf("杠!");
        }
    }
    
    if(pkcwArr[2]) {
        setCursorPosition(CHOW_BOTTON_X, CHOW_BOTTON_Y);
        if(lightIndex == 2) {
            setColor(23);
            printf("吃!");  
            setColor(3);
        }else {
            printf("吃!");
        }
    }

    if(pkcwArr[3]) {
        setCursorPosition(WIN_BOTTON_X, WIN_BOTTON_Y);
        if(lightIndex == 3) {
            setColor(23);
            printf("胡!");  
            setColor(3);
        }else {
            printf("胡!");
        }
    }

    //常显示
    if(pkcwArr[4]) {
        setCursorPosition(CANCEL_BOTTON_X, CANCEL_BOTTON_Y);
        if(lightIndex == 4) {
            setColor(23);
            printf("取消!");  
            setColor(3);
        }else {
            printf("取消!");
        }
    }

    if(pkcwArr[5]) {
        setCursorPosition(DEFAULT_BOTTON_X, DEFAULT_BOTTPN_Y);
        if(lightIndex == 5) {
            setColor(23);
            printf("(左右键选择回车键发送)");  
            setColor(3);
        }else {
            printf("(左右键选择回车键发送)");
        }
    }
    return lightIndex;
}

void MyPrint::printTurnYou(Mahjong *mahjong) {
    if(mahjong->ifMyPermissionDiscard == 1) {
        setCursorPosition(PONG_BOTTON_X, PONG_BOTTON_Y - 2);
        printf("轮到你出牌, myPermissionDiscard = %d\n",mahjong->ifMyPermissionDiscard);
    } else {
        setCursorPosition(PONG_BOTTON_X, PONG_BOTTON_Y);
        printf("                                                                                ");
    }
}

void MyPrint::printTurnYouOver(Mahjong *mahjong) {

}
