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
#include <iostream>
#include <stdio.h>
#include <thread>
#include <conio.h>
#include <time.h>
#include "Mahjong.h"
#include "MyPrint.h"
#include "Controller.h"
#include "MySocket.h"
#include "MyTest.h"
#include "Win.h"
using namespace std;

Controller::Controller() {
    myPrint = new MyPrint();
    mahjong = new Mahjong();
}

Controller::~Controller() {
    delete mahjong;
    delete myPrint;
}


void Controller::start() {
    //��Ϸ���

    
    myPrint->setWindowSize(COLS, LINES);
    printf("��ӭ���� �����齫 �������ҷ�����Ϸ���򲻷��ϼ����齫������ϵ����:�� 1207155125@qq.com\n");
    printf("����ʹ��������ģ����ʾ�����ܳ��ֻ������� f ��ˢ�¼���\n");
    printf("��׼�����ӷ�����...\n");
    int ret = MySocket::connectToServer();
    if(ret == -1) {
        printf("���ӷ�����ʧ�ܣ�����ϵ�����ֻ�13248175521��΢��ͬ��\n");
        printf("�����������\n");
        getchar();
        exit(0);
    }
    printf("���������ӳɹ�,server IP : %s Port : %d\n\n",SERVER_IP, SERVER_PORT);
    
    int deskNum = 999;
    string name = "testName";

    cout << "������Ҫ��������Ӻ�" << endl;
    cin >> deskNum;
    cout << "�������������" <<endl;
    cin >> name;
    
    printf("���Ӻ� : %d ����: %s\n", deskNum, name.c_str());

    selectDesk(deskNum, name);
    int deskOnlineNum = MySocket::message.deskOnlineNum;
    
    printf("��ǰ����������%d��,��ȴ�....\n", deskOnlineNum);

    while(deskOnlineNum < 4) {
        deskOnlineNum = getDeskOnlineNum();
        Sleep(1000);
        printf("��ǰ����������%d��,��ȴ�....\n", deskOnlineNum);
    }

    fillName();

    //��������֮��������˷���һ��������ֵ���Ϣ


    printf("������������ǰ��������%d�ˣ��������ת��ʼ��Ϸ\n", deskOnlineNum);

    Sleep(3000);

    //�������ˣ�������Ϸ


    system("cls");
    //test();
    playGame();

}

void Controller::fillName() {
    Message message;
    //�����Ϣ
    while(MySocket::message.OPTION != 0) {
        Message message = MySocket::recvMsg();
    }
    
    mahjong->allPlayerNumToName[mahjong->leftNum] = message.leftName;
    mahjong->allPlayerNumToName[mahjong->rightNum] = message.rightName;
    mahjong->allPlayerNumToName[mahjong->oppositeNum] = message.oppositeName;
    mahjong->allPlayerNumToName[mahjong->myNum] = message.myName;
    
}

void Controller::playGame() {
    //��ʽ��ʼ��Ϸ,����һ��ѡ���߳�����ѡ���齫�����߳�������������
    thread threadSelectMahjong(&Controller::selectMahjong, this);
    threadSelectMahjong.detach();

    //һ���ӡһ�½��յ���Ϣ
    thread threadTest(&Controller::testPrint, this);
    threadTest.detach();

    //���߳�����������Ϣ
    while(1) {
        //�����������Ϣ
        myPrint->printAll(mahjong, MySocket::message.nowNum, PRINTDESK_LIGHT);
        handleServerMsg();

        //MyTest::printMessage(MySocket::message);
        
    }

}

void Controller::selectMahjong() {
    while(1) {
        doSelect();
    }
}



int Controller::handleServerMsg() {
    Message *message = recvMsg();
    Sleep(50);
    MyTest::n++;
    int option = message->OPTION;
    
    Message sendMessage;
    switch(option) {
        case DISCARD:
            if(message->nowNum == mahjong->myNum) {
                refrashScreen(50);
                mahjong->ifMyPermissionDiscard = 1;
                judgeAnKongAnWin();//�ж������Ͱ���
                myPrint->printTurnYou(mahjong);
            }
            break;
        case DEAL:
            draw();
            break;
        case DRAW:
            draw();
            refrashScreen(300);
            break;
        case JUDGE:
            //����
            option = judgePKCW();
            sendMessage.OPTION = option;
            sendMessage.nowNum = mahjong->myNum;
            sendMessage.mahjong = message->mahjong;
            whoDiscardLastNum = message->nowNum;
            if(option == WIN) {
                for(auto iter = mahjong->myMahjong.begin(); iter != mahjong->myMahjong.end(); iter++) {
                    sendMessage.winPlayerMahjong[sendMessage.winPlayerMahjongSize++] = *iter;
                }
            }
            sendMsg(sendMessage);
            break;
        case PONG:
            //��Ҫ�����жϣ�����Ƿ����Լ��ģ��������Լ���������Ƿ������˵ģ���֪ͨ������
            handlePongKong();

            refrashScreen(300);
            break;
        case KONG:

            handlePongKong();

            refrashScreen(300);

            break;
        case ANKONG:
            handleAnkong();
            // refrashScreen(300);���漰�����Ʋ���Ҫˢ��
            break;

        case CHOW:

            break;

        case WIN:
            handleWin();
            //refrashScreen(300);

            break;
        case ANWIN:
            handleWin();
            //refrashScreen(300);
            break;

    }
    return 0;

}

void Controller::handleWin() {
    //����Ӯ
    WinMessage *wm = new WinMessage;
    //wm->winPlayerMahjong = MySocket::message.winPlayerMahjong;
    int winSize = MySocket::message.winPlayerMahjongSize;
    for(int i = 0; i< winSize; i++) {
        wm->winPlayerMahjong.push_back(MySocket::message.winPlayerMahjong[i]);
    }
    wm->winnerNum = MySocket::message.nowNum;
    if(MySocket::message.nowNum == mahjong->leftNum) {
        wm->winPKC = mahjong->leftPongKongChow;
    } else if(MySocket::message.nowNum == mahjong->rightNum) {
        wm->winPKC = mahjong->rightPongKongChow;
    } else if(MySocket::message.nowNum == mahjong->oppositeNum) {
        wm->winPKC = mahjong->oppositePongKongChow;
    } else if(MySocket::message.nowNum == mahjong->myNum) {
        wm->winPKC = mahjong->myPongKongChow;
    }
    wm->winnerName = mahjong->allPlayerNumToName[wm->winnerNum];

    allWinner.push_back(wm);

    system("cls");
    Sleep(400);
    myPrint->printWinner(allWinner);
}

void Controller::handlePongKong() {
    //����������
    int mahjongNum = MySocket::message.mahjong;
    int option = MySocket::message.OPTION;
    int who = MySocket::message.nowNum;

    PongKongChow *pkc = new PongKongChow();

    mahjong->deskMahjong.pop_back();

   

    int num = 0;
    if(option == PONG) {
        num = 2;
    } else if(option == KONG) {
        num = 3;
    } else {
        printf("OPTION����\n");
        return;
    }

    if(who != mahjong->myNum) {
        //���˵�����
        for(int i = 0; i< num+1; i++) {
            pkc->mahjong.push_back(mahjongNum);
        }
        pkc->flag = option;
        
        if(who == mahjong->leftNum) {
            mahjong->leftPongKongChow.push_back(pkc);
            mahjong->leftCount -= num;
        } else if(who == mahjong->rightNum) {
            mahjong->rightPongKongChow.push_back(pkc);
            mahjong->rightCount -= num;
        } else if(who == mahjong->oppositeNum) {
            mahjong->oppositePongKongChow.push_back(pkc);
            mahjong->oppositeCount-=num;
        }
        return;
    }   

    //�Լ�������
    int size = mahjong->myMahjong.size();

    vector<int> indexes;
    int index = 0;

    //�ҵ��齫�±�
    for(auto iter = mahjong->myMahjong.begin(); iter != mahjong->myMahjong.end(); iter++, index++) {
        if(*iter == mahjongNum) {
            indexes.push_back(index);
        }
    }

    
    for(int i = 0; i < num; i++) {
        index = indexes[i];
        delOneMahjongFromLocation(index);
        pkc->mahjong.push_back(mahjongNum);
    }

    pkc->mahjong.push_back(mahjongNum);//�ó��ƶ��е��Ǹ�

    //�ж����Ļ��߸ܵ�˭
    if(whoDiscardLastNum == mahjong->leftNum) {
        pkc->lightIndex = 0;
    } else if(whoDiscardLastNum == mahjong->rightNum) {
        pkc->lightIndex = pkc->mahjong.size()-1;
    } else if(whoDiscardLastNum == mahjong->oppositeNum) {
        pkc->lightIndex = 1;
    }
    pkc->flag = option;

    mahjong->myPongKongChow.push_back(pkc);


}

int Controller::judgePKCW() {
    Message message = MySocket::message;
    //�Ȱ��ƴ�ӡ��������
    mahjong->deskMahjong.push_back(message.mahjong);

    refrashScreen(300);

    if(message.nowNum == mahjong->leftNum) {
        mahjong->leftCount--;
    } else if(message.nowNum == mahjong->rightNum) {
        mahjong->rightCount--;
    } else if(message.nowNum == mahjong->oppositeNum){
        mahjong->oppositeCount--;
    }


    //�жϳ�����Ӯ,�����ڲ����ж����Լ������ƻ��Ǳ��˳�����
    return judgePongKongWin();
}

int Controller::judgePongKongWin() {

    if(MySocket::message.nowNum != mahjong->myNum) {
        //���˳��Ƶ�ʱ���ж�
        judgePongKong();
        judgeWin();
    }
    

    // refrashScreen(300);

    if((pongBit || kongBit || chowBit || winBit) == 0) {
        return NORMAL;//û�����ܳ�
    }

    int lightIndex = 5;
    vector<int> pkcwArr(5,0);
    pkcwArr[0] = pongBit;
    pkcwArr[1] = kongBit;
    pkcwArr[2] = chowBit;
    pkcwArr[3] = winBit;
    pkcwArr[4] = 1;//cancelѡ��
    pkcwArr[5] = 1;
    int right = RIGHT;
    int left = LEFT;
    int size = 0;
    size = pongBit + kongBit + chowBit + winBit + 1;

    //����ѡ���߳�
    blockSelectMahjong = 1;

    while(1) {

        myPrint->printPKCWBotton(pkcwArr, lightIndex);
        char ch = getch();

        
        switch(ch) {
            case RIGHT_PKCW:
                lightIndex = findNotZero(pkcwArr, lightIndex, pkcwArr.size(), right);
                break;
            case LEFT_PKCW:
                lightIndex = findNotZero(pkcwArr, lightIndex, pkcwArr.size(), left);
                break;
            case RIGHT:
                lightIndex = findNotZero(pkcwArr, lightIndex, pkcwArr.size(), right);
                break;
            case LEFT:
                lightIndex = findNotZero(pkcwArr, lightIndex, pkcwArr.size(), left);
                break;
            case ENTER:
                //�������ľ��������ѡ���̵߳�����
                blockSelectMahjong = 0;
                if(lightIndex == 4) {
                    //Normal
                    kongBit = 0;
                    pongBit = 0;
                    winBit = 0;
                    chowBit = 0;
                    return NORMAL;
                }
            
                return pkcwArr[lightIndex];//���ؾ������
                break;
        }
    }
    return 0;
}

void Controller::handleAnkong() {
    int mahjongNum = MySocket::message.mahjong;
    PongKongChow *pkc = new PongKongChow;
    //������
    if(MySocket::message.nowNum == mahjong->myNum) {
        //���Լ��İ���
        
        for(int i =0; i<4;i ++) {
            delOneMahjongFromNum(mahjongNum);
            pkc->mahjong.push_back(mahjongNum);
        }
        pkc->lightIndex = -1;
        pkc->flag = KONG;
        mahjong->myPongKongChow.push_back(pkc);
    } else {
        //���˵İ���
        for(int i = 0; i< 4; i++) {
            pkc->mahjong.push_back(mahjongNum);
        }
        pkc->lightIndex = -1;
        pkc->flag = KONG;

        if(MySocket::message.nowNum == mahjong->leftNum) {
            //�ϼ�
            mahjong->leftCount -= 4;
            mahjong->leftPongKongChow.push_back(pkc);
        } else if(MySocket::message.nowNum == mahjong->rightNum) {
            mahjong->rightCount -= 4;
            mahjong->rightPongKongChow.push_back(pkc);
        } else if(MySocket::message.nowNum == mahjong->oppositeNum) {
            mahjong->oppositeCount -= 4;
            mahjong->oppositePongKongChow.push_back(pkc);
        }
    }
}

int Controller::delOneMahjongFromNum(int num) {
    for(auto iter = mahjong->myMahjong.begin(); iter != mahjong->myMahjong.end(); iter++) {
        if(*iter == num) {
            mahjong->myMahjong.erase(iter);
            break;
        }
    }
    return 0;
}


int Controller::judgeAnKongAnWin() {
    judgeAnKong();
    judgeAnWin();

    int option = judgePongKongWin();
    if(option == NORMAL) {
        return 0;//��������
    }

    if(option == WIN) {
        //��������
        option = ANWIN;
    }

    if(option == ANKONG) {
        //���ܼ���ѡ��
        if(allAnKongMahjong.size() == 0) {
            return 0;
        }
        if(allAnKongMahjong.size() == 1) {
            //ֱ�ӷ����������
            Message message;
            message.OPTION = ANKONG;
            message.mahjong = allAnKongMahjong.back();
            message.nowNum = mahjong->myNum;
            sendMsg(message);
            pongBit = 0;
            kongBit = 0;
            chowBit = 0;
            winBit = 0;
            allAnKongMahjong.clear();
            return 0;
        } else {
            //��ֹһ������Ҫѡ��,�������
            // refrashScreen(300);
            blockSelectMahjong = 1;
            int lightIndex = allAnKongMahjong.size();
            int left = LEFT;
            int right = RIGHT;
            allAnKongMahjong.push_back(1);
            
            while(1) {
                myPrint->printAnKongChose(allAnKongMahjong, lightIndex);
                char ch = getch();
                switch(ch) {
                    case LEFT:
                        lightIndex = findNotZero(allAnKongMahjong, lightIndex, allAnKongMahjong.size(), left);
                        break;
                    case RIGHT:
                        lightIndex = findNotZero(allAnKongMahjong, lightIndex, allAnKongMahjong.size(), right);
                        break;
                    case ENTER:
                        //�������ľ���
                        blockSelectMahjong = 0;
                        if(lightIndex == allAnKongMahjong.size()-1) {
                            //ȡ��
                            judgeAnKongAnWin();//����һ��ֱ����ѡȡ��
                            return 0;
                        }

                        Message message;
                        message.OPTION = ANKONG;
                        message.mahjong = allAnKongMahjong[lightIndex];
                        message.nowNum = mahjong->myNum;
                        sendMsg(message);
                        pongBit = 0;
                        kongBit = 0;
                        chowBit = 0;
                        winBit = 0;
                        allAnKongMahjong.clear();
                        //���ܼ����ܣ��ȷ���������

                        // refrashScreen(300);
                        return 0;
                }
            }

        }

    } else if(option == ANWIN) {
        //��������
        Message message;
        message.OPTION = ANWIN;
        message.nowNum = mahjong->myNum;
        for(auto iter = mahjong->myMahjong.begin(); iter != mahjong->myMahjong.end(); iter++, message.winPlayerMahjongSize++) {
            message.winPlayerMahjong[message.winPlayerMahjongSize] = *iter;
        }
        myPrint->setCursorPosition(10, 0);
        
        sendMsg(message);

        refrashScreen(300);
        return 0;
    }

    return 0;


}

int Controller::findNotZero(vector<int> &arr, int begin, int arrSize, int leftRoRight) {
    int next;
    switch(leftRoRight) {
        case LEFT:
            next = (begin -1 + arrSize) % arrSize;
            while(next != begin) {
                if(arr[next] > 0) {
                    return next;
                }
                next = (next -1 + arrSize) % arrSize;
            }
            return begin;
            break;
        case RIGHT:
            next = (begin + 1 +arrSize) % arrSize;
            while(next != begin) {
                if(arr[next] > 0) {
                    return next;
                }
                next = (next + 1 +arrSize) % arrSize;
            }
            return begin;
            break;
    }
    return -1;
}

int Controller::judgePongKong() {
    int mahjongNum;
    int num = 0;
    

    //��ҳ��ƣ�ֻ���ж�����
    mahjongNum = MySocket::message.mahjong;
    for(auto iter = mahjong->myMahjong.begin(); iter != mahjong->myMahjong.end(); iter++) {
        if(*iter == mahjongNum) {
            num++;
        }
    }

   
    if(num >= 3) {
        pongBit = PONG;
        kongBit = KONG;
    } else if(num >= 2) {
        pongBit = PONG;
    } else {
        pongBit = 0;
        kongBit = 0;
        return -1;
    }
    return 0;
}
void Controller::judgeChow() {

}

void Controller::judgeWin() {
    vector<int> myMahjong;
    for(auto iter = mahjong->myMahjong.begin(); iter != mahjong->myMahjong.end(); iter++) {
        myMahjong.push_back(*iter);
    }
    if(MySocket::message.nowNum != mahjong->myNum) {
        //����Ǳ�����ƣ��Ͱ�����ƷŽ�ȥ�жϣ�������Լ�����ƣ���һ�����Ѿ��Ž�ȥ��
        myMahjong.push_back(MySocket::message.mahjong);
    }
    
    bool ans = Win::judgeWin(myMahjong);
    if(ans == true) {
        winBit = WIN;
    } else {
        winBit = 0;
    }
}

int Controller::judgeAnKong() {
    int mahjongNum;
    int num = 0;
    int tmpNum;
    int tmpMahjong;
    int kongMahjong;
    //�жϰ���
    allAnKongMahjong.clear();
    
    tmpNum = 0;
    tmpMahjong = -1;
    for(auto iter = mahjong->myMahjong.begin(); iter != mahjong->myMahjong.end(); iter++) {
        if(*iter != tmpMahjong) {
            tmpMahjong = *iter;
            tmpNum = 1;
        } else if(*iter == tmpMahjong) {
            tmpNum++;
        }
        if(tmpNum == 4) {
            allAnKongMahjong.push_back(tmpMahjong);//�Ž���¼����
            num = tmpNum;
            tmpNum = 0;
        }
    }

    if(num >= 4) {
        myPrint->setCursorPosition(0,0);
        printf("����\n");
        kongBit = ANKONG;
        myAnKongMahjong = kongMahjong;
    } else {
        kongBit = 0;
    }
    return 0;
}

int Controller::judgeAnWin() {
    //�ж�����
    judgeWin();
    return 0;
}


void Controller::draw() {
    if(MySocket::message.nowNum == mahjong->myNum) {
        int mahNum = MySocket::message.mahjong;
        mahjong->myMahjong.push_back(mahNum);
        // if(MySocket::message.OPTION == DRAW) {
        //     refrashScreen(500);
        // }
        mahjong->myMahjong.sort();
    } else if (MySocket::message.nowNum == mahjong->leftNum) {
        mahjong->leftCount++;
    } else if (MySocket::message.nowNum == mahjong->rightNum) {
        mahjong->rightCount++;
    } else if (MySocket::message.nowNum == mahjong->oppositeNum) {
        mahjong->oppositeCount++;
    }
}





//�����������ȴ����û�����󷵻أ�����˴���
int Controller::getDeskOnlineNum() {
    Message message = MySocket::recvMsg();
    printf("��������������%s, ��ţ�%d\n", message.myName,message.nowNum);
    return message.deskOnlineNum;
}

void Controller::doSelect() {
    char ch = 'k';
    if (blockSelectMahjong == 0) {
        //�������
        ch = getch();
    }
    //�л�ѡ��
    switch(ch) {
        case LEFT:
            mahjong->selectLocation > 0 ? mahjong->selectLocation-- : mahjong->selectLocation = mahjong->myMahjong.size() -1;
            break;
        case RIGHT:
            mahjong->selectLocation < mahjong->myMahjong.size() - 1 ? 
                mahjong->selectLocation++ : mahjong->selectLocation = 0;
            break;
        case ENTER:
            //��
            //����
            if(blockSelectMahjong == 1) {
                return;
            }
            discard();
            break;
        case FLASH:
            //ˢ��
            refrashScreen(100);
            break;
        default:
            return;
    }
    myPrint->printMyAllMahjong(mahjong->myMahjong, mahjong->selectLocation);
}

void Controller::refrashScreen(int microsecond) {
    system("cls");
    Sleep(microsecond);
    myPrint->printAll(mahjong,MySocket::message.nowNum, PRINTDESK_LIGHT);
}

void Controller::discard() {
    //����ʱ�жϣ��Ƿ����Լ��Ļغϣ�������ǣ������Գ���
    
    if(MySocket::message.nowNum == mahjong->leftNum) {
        myPrint->setCursorPosition(0,0);
        printf("�ϼҳ���\n");
    } else if(MySocket::message.nowNum == mahjong->rightNum) {
        myPrint->setCursorPosition(0,0);
        printf("�¼ҳ���\n");
    } else if(MySocket::message.nowNum == mahjong->oppositeNum) {
        myPrint->setCursorPosition(0,0);
        printf("���ų���\n");
    } else if(MySocket::message.nowNum == mahjong->myNum && MySocket::message.OPTION == DISCARD) {
        //����

        
        
        Message message = MySocket::message;
        message.OPTION = DISCARD;
        message.mahjong = delOneMahjongFromLocation(mahjong->selectLocation);
        sendMsg(message);//���齫���͸�������
        mahjong->deskMahjong.push_back(message.mahjong);
        mahjong->ifMyPermissionDiscard = 0;
        MySocket::message.OPTION = NORMAL;
        
        refrashScreen(300);
    }
    
    
    
}

int Controller::delOneMahjongFromLocation(int index) {
    //ɾ��ָ��λ�õ��齫
    int mahjongNum = -1;
    auto iter = mahjong->myMahjong.begin();
    for(int i = 0; i < index; i++) {
        iter++;
    }
    mahjongNum = *iter;

    mahjong->myMahjong.erase(iter);
    return mahjongNum;
}



void Controller::selectDesk(int deskNum, string name) {
    while(1) {
        Message message;
        message.deskNum = deskNum;
        message.OPTION = SELECT_DESK;

        strcpy(message.myName, name.c_str());
        sendMsg(message);
        recvMsg();
        message = MySocket::message;
        if(message.OPTION == SELECT_DESK_FAIL) {
            printf("������Ա���뻻һ������\n");
            printf("������������:\n");
            cin >> deskNum;
            printf("���ؽ��\n");
            MyTest::printMessage(message, pongBit, kongBit, chowBit, winBit);
        } else {
            //ѡ��ɹ����������
            mahjong->myNum = message.myNum;
            mahjong->leftNum = message.leftNum;
            mahjong->rightNum = message.rightNum;
            mahjong->oppositeNum = message.oppositeNum;
            mahjong->myCount = 0;
            mahjong->leftCount = 0;
            mahjong->rightCount= 0;
            mahjong->oppositeCount =0;

            break;
        }
    }
}

int Controller::sendMsg(Message &message) {
    MySocket::sendMsg(message);

    return 0;
}

Message* Controller::recvMsg() {
    Message *message = &MySocket::recvMsg();

    return message;
}






void Controller::test() {
    //���Լ�13���齫
    for(int i = 0; i < 13; i++) {
        mahjong->myMahjong.push_back(SEVENBAMBOO);
    }

    //��������30���齫��

    for(int i = 0; i < 30 ; i++) {
        mahjong->deskMahjong.push_back(SIXBAMBOO);
    }

    //�����ܳԸ�һ��
   
    PongKongChow *pkc = new PongKongChow();
    pkc->mahjong.push_back(SIXBAMBOO);
    pkc->mahjong.push_back(SIXBAMBOO);
    pkc->mahjong.push_back(SIXBAMBOO);
    pkc->flag = PONG;
    pkc->lightIndex = 2;

    mahjong->myPongKongChow.push_back(pkc);
    mahjong->leftPongKongChow.push_back(pkc);
    mahjong->rightPongKongChow.push_back(pkc);
    mahjong->oppositePongKongChow.push_back(pkc);

    PongKongChow *pkc2 = new PongKongChow();
    pkc2->mahjong.push_back(SIXBAMBOO);
    pkc2->mahjong.push_back(FIVEBAMBOO);
    pkc2->mahjong.push_back(FOURBAMBOO);
    pkc2->flag = CHOW;
    pkc2->lightIndex = 2;

    mahjong->myPongKongChow.push_back(pkc2);
    mahjong->leftPongKongChow.push_back(pkc2);
    mahjong->rightPongKongChow.push_back(pkc2);
    mahjong->oppositePongKongChow.push_back(pkc2);

    PongKongChow *pkc3 = new PongKongChow();
    pkc3->mahjong.push_back(SIXBAMBOO);
    pkc3->mahjong.push_back(SIXBAMBOO);
    pkc3->mahjong.push_back(SIXBAMBOO);
    pkc3->mahjong.push_back(SIXBAMBOO);
    pkc3->flag = KONG;
    pkc3->lightIndex = 2;

    mahjong->myPongKongChow.push_back(pkc3);
    mahjong->leftPongKongChow.push_back(pkc3);
    mahjong->rightPongKongChow.push_back(pkc3);
    mahjong->oppositePongKongChow.push_back(pkc3);


    mahjong->leftCount = 10;
    mahjong->rightCount = 10;
    mahjong->oppositeCount = 10;

    

    myPrint->printAll(mahjong,MySocket::message.nowNum, PRINTDESK_LIGHT);


}

void Controller::testSocket() {
    MySocket::connectToServer();
    Message message;
    message.mahjong = 20;
    MySocket::sendMsg(message);
    Message msg = MySocket::recvMsg();
    printf("%d\n------", msg.mahjong);
}



void Controller::testPrint() {
    Message &message = MySocket::message;
    int col = COLS /2 + 10;
    while(1) {
        int step = 1;
        Sleep(5000);
        myPrint->setCursorPosition(col , LINES/2 + step++);
        cout << "mahjong : " << message.mahjong << endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout << "remainMahjong : " << message.remainMahjong << endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout << "diceNum : " << message.diceNum<< endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout << "myNum : " << message.myNum << endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout << "leftNum : " << message.leftNum << endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout << "rightNum : " << message.rightNum << endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout << "oppositeNum : " << message.oppositeNum << endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout << "PongBit : " << pongBit << endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout << "KongBit : " << kongBit << endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout << "�ϼ����� :" <<mahjong->allPlayerNumToName[mahjong->leftNum] << 
        "�¼����� :" << mahjong->allPlayerNumToName[mahjong->rightNum] << 
        "�Լ�����:" << mahjong->allPlayerNumToName[mahjong->oppositeNum] <<endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout << "WinBit : " << winBit << endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout << "nowNum : " << message.nowNum << endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout << "Ӯ�����ƣ�";
        for(int i = 0; i < message.winPlayerMahjongSize; i++) {
            cout << message.winPlayerMahjong[i] << ' ';
        }
        cout <<endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout << "deskNum : " << message.deskNum << endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout << "Ӯ������������" << message.winPlayerMahjongSize <<endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout << "OPTION : " << message.OPTION << endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout <<"�յ�����Ϣ������"<< MyTest::n<<endl;
        
    }
}


