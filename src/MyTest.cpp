#include "MySocket.h"
#include "MyPrint.h"
#include "Controller.h"
#include "Mahjong.h"
#include "MyTest.h"
#include <iostream>
using namespace std;

int MyTest::n = 0;

void MyTest::printMessage(Message &message, int pongBit, int kongBit, int chowBit, int winBit) {
    cout << "mahjong : " << message.mahjong << endl;
    cout << "remainMahjong : " << message.remainMahjong << endl;
    cout << "diceNum : " << message.diceNum<< endl;
    cout << "myNum : " << message.myNum << endl;
    cout << "leftNum : " << message.leftNum << endl;
    cout << "rightNum : " << message.rightNum << endl;
    cout << "oppositeNum : " << message.oppositeNum << endl;
    cout << "PongBit : " << message.Pong << endl;
    cout << "KongBit : " << message.Kong << endl;
    cout << "ChowBit : " << message.Chow << endl;
    cout << "WinBit : " << message.Win << endl;
    cout << "nowNum : " << message.nowNum << endl;
    cout << "allOnlineNum : " << message.allOnlineNum << endl;
    cout << "deskNum : " << message.deskNum << endl;
    cout << "deskOnlineNum : " << message.deskOnlineNum << endl;
    cout << "OPTION : " << message.OPTION << endl;
}


