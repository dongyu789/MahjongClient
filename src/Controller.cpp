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
    //游戏入口

    
    myPrint->setWindowSize(COLS, LINES);
    printf("欢迎游玩 济阳麻将 ，如果玩家发现游戏规则不符合济阳麻将，请联系作者:胡 1207155125@qq.com\n");
    printf("由于使用命令行模拟显示，可能出现花屏，按 f 键刷新即可\n");
    printf("正准备连接服务器...\n");
    int ret = MySocket::connectToServer();
    if(ret == -1) {
        printf("连接服务器失败，请联系作者手机13248175521，微信同号\n");
        printf("按任意键结束\n");
        getchar();
        exit(0);
    }
    printf("服务器连接成功,server IP : %s Port : %d\n\n",SERVER_IP, SERVER_PORT);
    
    int deskNum = 999;
    string name = "testName";

    cout << "输入你要进入的桌子号" << endl;
    cin >> deskNum;
    cout << "请输入你的名字" <<endl;
    cin >> name;
    
    printf("桌子号 : %d 姓名: %s\n", deskNum, name.c_str());

    selectDesk(deskNum, name);
    int deskOnlineNum = MySocket::message.deskOnlineNum;
    
    printf("当前桌在线人数%d人,请等待....\n", deskOnlineNum);

    while(deskOnlineNum < 4) {
        deskOnlineNum = getDeskOnlineNum();
        Sleep(1000);
        printf("当前桌在线人数%d人,请等待....\n", deskOnlineNum);
    }

    fillName();

    //人数满了之后给所有人发送一个填充名字的信息


    printf("人数已满，当前在线人数%d人，三秒后跳转开始游戏\n", deskOnlineNum);

    Sleep(3000);

    //人数够了，进入游戏


    system("cls");
    //test();
    playGame();

}

void Controller::fillName() {
    Message message;
    //填充信息
    while(MySocket::message.OPTION != 0) {
        Message message = MySocket::recvMsg();
    }
    
    mahjong->allPlayerNumToName[mahjong->leftNum] = message.leftName;
    mahjong->allPlayerNumToName[mahjong->rightNum] = message.rightName;
    mahjong->allPlayerNumToName[mahjong->oppositeNum] = message.oppositeName;
    mahjong->allPlayerNumToName[mahjong->myNum] = message.myName;
    
}

void Controller::playGame() {
    //正式开始游戏,创建一个选择线程用来选择麻将，主线程用来阻塞读入
    thread threadSelectMahjong(&Controller::selectMahjong, this);
    threadSelectMahjong.detach();

    // //一秒打印一下接收的信息
    // thread threadTest(&Controller::testPrint, this);
    // threadTest.detach();

    //主线程用来接收消息
    while(1) {
        //处理服务器消息
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
                judgeAnKongAnWin();//判断自摸和暗杠
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
            //看牌
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
            //需要进行判断，如果是发给自己的，是允许自己碰，如果是发给别人的，是通知别人碰
            handlePongKong();

            refrashScreen(300);
            break;
        case KONG:

            handlePongKong();

            refrashScreen(300);

            break;
        case ANKONG:
            handleAnkong();
            // refrashScreen(300);会涉及到起牌不需要刷新
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
    //处理赢
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
    //处理碰、杠
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
        printf("OPTION错误\n");
        return;
    }

    if(who != mahjong->myNum) {
        //别人的碰杠
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

    //自己的碰杠
    int size = mahjong->myMahjong.size();

    

    //删除指定麻将
    for(int i = 0; i < num; i++) {
        delOneMahjongFromNum(mahjongNum);
        pkc->mahjong.push_back(mahjongNum);
    }

    pkc->mahjong.push_back(mahjongNum);//拿出牌堆中的那个

    //判断碰的或者杠的谁
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
    //先把牌打印到桌子上
    mahjong->deskMahjong.push_back(message.mahjong);

    refrashScreen(300);

    if(message.nowNum == mahjong->leftNum) {
        mahjong->leftCount--;
    } else if(message.nowNum == mahjong->rightNum) {
        mahjong->rightCount--;
    } else if(message.nowNum == mahjong->oppositeNum){
        mahjong->oppositeCount--;
    }


    //判断吃碰杠赢,函数内部会判断是自己摸的牌还是别人出的牌
    return judgePongKongWin();
}

int Controller::judgePongKongWin() {

    if(MySocket::message.nowNum != mahjong->myNum) {
        //别人出牌的时候判断
        judgePongKong();
        judgeWin();
    }
    

    // refrashScreen(300);

    if((pongBit || kongBit || chowBit || winBit) == 0) {
        return NORMAL;//没有碰杠吃
    }

    int lightIndex = 5;
    vector<int> pkcwArr(5,0);
    pkcwArr[0] = pongBit;
    pkcwArr[1] = kongBit;
    pkcwArr[2] = chowBit;
    pkcwArr[3] = winBit;
    pkcwArr[4] = 1;//cancel选项
    pkcwArr[5] = 1;
    int right = RIGHT;
    int left = LEFT;
    int size = 0;
    size = pongBit + kongBit + chowBit + winBit + 1;

    //阻塞选择线程
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
                //返回最后的决定，解除选择线程的阻塞
                blockSelectMahjong = 0;
                kongBit = 0;
                pongBit = 0;
                winBit = 0;
                chowBit = 0;
                if(lightIndex == 4) {
                    //Normal
                    return NORMAL;
                }
            
                return pkcwArr[lightIndex];//返回决定结果
                break;
        }
    }
    return 0;
}

void Controller::handleAnkong() {
    int mahjongNum = MySocket::message.mahjong;
    PongKongChow *pkc = new PongKongChow;
    //处理暗杠
    if(MySocket::message.nowNum == mahjong->myNum) {
        //我自己的暗杠
        
        for(int i =0; i<4;i ++) {
            delOneMahjongFromNum(mahjongNum);
            pkc->mahjong.push_back(mahjongNum);
        }
        pkc->lightIndex = -1;
        pkc->flag = KONG;
        mahjong->myPongKongChow.push_back(pkc);
    } else {
        //别人的暗杠
        for(int i = 0; i< 4; i++) {
            pkc->mahjong.push_back(mahjongNum);
        }
        pkc->lightIndex = -1;
        pkc->flag = KONG;

        if(MySocket::message.nowNum == mahjong->leftNum) {
            //上家
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
        return 0;//不做处理
    }

    if(option == WIN) {
        //自摸处理
        option = ANWIN;
    }

    if(option == ANKONG) {
        //暗杠继续选择
        if(allAnKongMahjong.size() == 0) {
            return 0;
        }
        if(allAnKongMahjong.size() == 1) {
            //直接发送这个暗杠
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
            //不止一个，需要选择,最多三个
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
                        //返回最后的决定
                        blockSelectMahjong = 0;
                        if(lightIndex == allAnKongMahjong.size()-1) {
                            //取消
                            judgeAnKongAnWin();//再来一遍直到都选取消
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
                        //可能继续杠，等服务器发牌

                        // refrashScreen(300);
                        return 0;
                }
            }

        }

    } else if(option == ANWIN) {
        //自摸处理
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
    

    //别家出牌，只需判断手牌
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
        //如果是别出的牌，就把这个牌放进去判断，如果是自己起的牌，上一步就已经放进去了
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
    //判断暗杠
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
            allAnKongMahjong.push_back(tmpMahjong);//放进记录容器
            num = tmpNum;
            tmpNum = 0;
        }
    }

    if(num >= 4) {
        myPrint->setCursorPosition(0,0);
        printf("暗杠\n");
        kongBit = ANKONG;
        myAnKongMahjong = kongMahjong;
    } else {
        kongBit = 0;
    }
    return 0;
}

int Controller::judgeAnWin() {
    //判断自摸
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





//阻塞函数，等待新用户接入后返回，服务端处理
int Controller::getDeskOnlineNum() {
    Message message = MySocket::recvMsg();
    printf("接入新人姓名：%s, 编号：%d\n", message.myName,message.nowNum);
    return message.deskOnlineNum;
}

void Controller::doSelect() {
    char ch = 'k';
    if (blockSelectMahjong == 0) {
        //阻塞解除
        ch = getch();
    }
    //切换选择
    switch(ch) {
        case LEFT:
            mahjong->selectLocation > 0 ? mahjong->selectLocation-- : mahjong->selectLocation = mahjong->myMahjong.size() -1;
            break;
        case RIGHT:
            mahjong->selectLocation < mahjong->myMahjong.size() - 1 ? 
                mahjong->selectLocation++ : mahjong->selectLocation = 0;
            break;
        case ENTER:
            //√
            //出牌
            if(blockSelectMahjong == 1) {
                return;
            }
            discard();
            break;
        case FLASH:
            //刷新
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
    //出牌时判断，是否是自己的回合，如果不是，不可以出牌
    
    if(MySocket::message.nowNum == mahjong->leftNum) {
        myPrint->setCursorPosition(0,0);
        printf("上家出牌\n");
    } else if(MySocket::message.nowNum == mahjong->rightNum) {
        myPrint->setCursorPosition(0,0);
        printf("下家出牌\n");
    } else if(MySocket::message.nowNum == mahjong->oppositeNum) {
        myPrint->setCursorPosition(0,0);
        printf("对门出牌\n");
    } else if(MySocket::message.nowNum == mahjong->myNum && MySocket::message.OPTION == DISCARD) {
        //出牌

        
        
        Message message = MySocket::message;
        message.OPTION = DISCARD;
        message.mahjong = delOneMahjongFromLocation(mahjong->selectLocation);
        sendMsg(message);//把麻将发送给服务器
        mahjong->deskMahjong.push_back(message.mahjong);
        mahjong->ifMyPermissionDiscard = 0;
        MySocket::message.OPTION = NORMAL;
        
        refrashScreen(300);
    }
    
    
    
}

int Controller::delOneMahjongFromLocation(int index) {
    //删掉指定位置的麻将
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
            printf("该桌满员，请换一个桌子\n");
            printf("重新输入桌号:\n");
            cin >> deskNum;
            printf("返回结果\n");
            MyTest::printMessage(message, pongBit, kongBit, chowBit, winBit);
        } else {
            //选择成功，标记数据
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
    //给自己13张麻将
    for(int i = 0; i < 13; i++) {
        mahjong->myMahjong.push_back(SEVENBAMBOO);
    }

    //给桌子上30个麻将机

    for(int i = 0; i < 30 ; i++) {
        mahjong->deskMahjong.push_back(SIXBAMBOO);
    }

    //给碰杠吃各一对
   
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
        cout << "上家名字 :" <<mahjong->allPlayerNumToName[mahjong->leftNum] << 
        "下家名字 :" << mahjong->allPlayerNumToName[mahjong->rightNum] << 
        "对家名字:" << mahjong->allPlayerNumToName[mahjong->oppositeNum] <<endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout << "WinBit : " << winBit << endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout << "nowNum : " << message.nowNum << endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout << "赢家手牌：";
        for(int i = 0; i < message.winPlayerMahjongSize; i++) {
            cout << message.winPlayerMahjong[i] << ' ';
        }
        cout <<endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout << "deskNum : " << message.deskNum << endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout << "赢家手牌数量：" << message.winPlayerMahjongSize <<endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout << "OPTION : " << message.OPTION << endl;
        myPrint->setCursorPosition(col, LINES/2 + step++);
        cout <<"收到的消息数量："<< MyTest::n<<endl;
        
    }
}


