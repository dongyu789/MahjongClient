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
//ֻ�д�ӡ��ʱ����Ҫչʾ������Ӧ�÷���MyPrint��
MyPrint::MyPrint() {
    mahjongName[ONEDOT] = "һͲ";
    mahjongName[TWODOT] = "��Ͳ";
    mahjongName[THREEDOT] = "��Ͳ";
    mahjongName[FOURDOT] = "��Ͳ";
    mahjongName[FIVEDOT] = "��Ͳ";
    mahjongName[SIXDOT] = "��Ͳ";
    mahjongName[SEVENDOT] = "��Ͳ";
    mahjongName[EIGHTDOT] = "��Ͳ";
    mahjongName[NINEDOT] = "��Ͳ";

    mahjongName[ONECHARACTER] = "һ�f";
    mahjongName[TWOCHARACTER] = "���f";
    mahjongName[THREECHARACTER] = "���f";
    mahjongName[FOURCHARACTER] = "���f";
    mahjongName[FIVECHARACTER] = "���f";
    mahjongName[SIXCHARACTER] = "���f";
    mahjongName[SEVENCHARACTER] = "���f";
    mahjongName[EIGHTCHARACTER] = "���f";
    mahjongName[NINECHARACTER] = "���f";

    mahjongName[ONEBAMBOO] = "һ��";
    mahjongName[TWOBAMBOO] = "����";
    mahjongName[THREEBAMBOO] = "����";
    mahjongName[FOURBAMBOO] = "����";
    mahjongName[FIVEBAMBOO] = "����";
    mahjongName[SIXBAMBOO] = "����";
    mahjongName[SEVENBAMBOO] = "����";
    mahjongName[EIGHTBAMBOO] = "����";
    mahjongName[NINEBAMBOO] = "����";

    mahjongName[EAST] = "����";
    mahjongName[SOUTH] = "�Ϸ�";
    mahjongName[WEST] = "����";
    mahjongName[NORTH] = "����";
    mahjongName[RED] = "����";
    mahjongName[GREEN] = "����";
    mahjongName[WHITEDRAGON] = "�װ�";


}

void MyPrint::setWindowSize(int cols = COLS, int lines = LINES) {
    system("�齫");//���ô��ڱ���
    char cmd[30];
    sprintf(cmd, "mode con cols=%d lines=%d", cols *2, lines);
    system(cmd);
}

void MyPrint::setCursorPosition(double x, int y) {
    //���ù���λ��
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
    //�����ı���ɫ
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorID);
}

void MyPrint::printMyAllMahjong(const list<int> &myMahjong, int i) {    

    int step = 0;
    int num = 0;
    for(auto iter = myMahjong.begin(); iter != myMahjong.end(); iter++, step+=5, num++) {
        int id = *iter;
        if(num == i) {
            //��ӡ����
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
 * @brief ��ӡ���������Ѿ�������齫�����Ұ����һ���������Ϊ��ɫ,
 * 
 * @param deskMahjong ���ϵ��齫����
 * @param flag �Ƿ�����һ����Ϊ��ɫ
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
 * @brief ��ӡ�������ܳ�
 * 
 * @param allPongKongChow 
 * @param who ��ӡ˭��
 */
void MyPrint::printPongKongChow(vector<PongKongChow*> allPongKongChow, int who) {

    int positionX;
    int positionY;
    int oldPostitonX;
    int xstep = 0;
    int ystep = 0;
    int xaddstep = 0;//����
    int yaddstep = 0;//����
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
    //˭����˭��
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
        printf("��");
        setCursorPosition(x, y+1);
        printf("��");
    } else {
        setCursorPosition(x, y);
        printf("��");
        setCursorPosition(x+1, y);
        printf("��");
    }
}

void MyPrint::printWinner(vector<WinMessage*> &wms) {
    //�ȴ�ӡ�����ƣ��ٴ�ӡ�����ܳ�
    int size = wms.size();
    int xstep = 0;
    
    int ystep = 0;
    for(int i = 0; i < size; i++, ystep += 6) {
        xstep = -4;
        setCursorPosition(WINNER_X + xstep, WINNER_Y + ystep - 1);
        printf("%d %s Ӯ��!", wms[i]->winnerNum, wms[i]->winnerName.c_str());
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
        printf("(���Ҽ�ѡ��س�������size:%d, index:%d)", anKongArr.size(), lightIndex);  
        setColor(3);
    } else {
        printf("(���Ҽ�ѡ��س�������size:%d, index:%d)", anKongArr.size(), lightIndex); 
    }
    
    for(int i = 0; i<anKongArr.size(); i++) {
        setCursorPosition(PONG_BOTTON_X + 5*i, PONG_BOTTON_Y);
        if(lightIndex == i) {
            setColor(23);
            if(i == anKongArr.size()-1) {
                printf("ȡ��!");  
            }else {
                printf("%s", mahjongName[anKongArr[i]].c_str());  
            }
            setColor(3);
        }else {
            if(i == anKongArr.size()-1) {
                printf("ȡ��!");  
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
            printf("��!");  
            setColor(3);
        }else {
            printf("��!");
        }
    } 

    if(pkcwArr[1]) {
        setCursorPosition(KONG_BOTTON_X, KONG_BOTTON_Y);
        if(lightIndex == 1) {
            setColor(23);
            printf("��!");  
            setColor(3);
        }else {
            printf("��!");
        }
    }
    
    if(pkcwArr[2]) {
        setCursorPosition(CHOW_BOTTON_X, CHOW_BOTTON_Y);
        if(lightIndex == 2) {
            setColor(23);
            printf("��!");  
            setColor(3);
        }else {
            printf("��!");
        }
    }

    if(pkcwArr[3]) {
        setCursorPosition(WIN_BOTTON_X, WIN_BOTTON_Y);
        if(lightIndex == 3) {
            setColor(23);
            printf("��!");  
            setColor(3);
        }else {
            printf("��!");
        }
    }

    //����ʾ
    if(pkcwArr[4]) {
        setCursorPosition(CANCEL_BOTTON_X, CANCEL_BOTTON_Y);
        if(lightIndex == 4) {
            setColor(23);
            printf("ȡ��!");  
            setColor(3);
        }else {
            printf("ȡ��!");
        }
    }

    if(pkcwArr[5]) {
        setCursorPosition(DEFAULT_BOTTON_X, DEFAULT_BOTTPN_Y);
        if(lightIndex == 5) {
            setColor(23);
            printf("(���Ҽ�ѡ��س�������)");  
            setColor(3);
        }else {
            printf("(���Ҽ�ѡ��س�������)");
        }
    }
    return lightIndex;
}

void MyPrint::printTurnYou(Mahjong *mahjong) {
    if(mahjong->ifMyPermissionDiscard == 1) {
        setCursorPosition(PONG_BOTTON_X, PONG_BOTTON_Y - 2);
        printf("�ֵ������, myPermissionDiscard = %d\n",mahjong->ifMyPermissionDiscard);
    } else {
        setCursorPosition(PONG_BOTTON_X, PONG_BOTTON_Y);
        printf("                                                                                ");
    }
}

void MyPrint::printTurnYouOver(Mahjong *mahjong) {

}
