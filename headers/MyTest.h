#ifndef MYTEST_H
#define MYTEST_H

#include "MySocket.h"
#include "MyPrint.h"
#include "Controller.h"
#include "Mahjong.h"
#include <iostream>
using namespace std;

/**
 * @brief 测试类
 * 
 */
class MyTest {
public:

    static void printMessage(Message &message, int p, int k, int c, int w);

    static void testThread(Message &message);
    static int n;
};


#endif