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
#include "Mahjong.h"
#include <stack>
#include <list>
#include <iostream>
#include <stdio.h>
using namespace std;

Mahjong::Mahjong() {

}

PongKongChow::PongKongChow(){

}

Mahjong::~Mahjong() {
    for(auto i : myPongKongChow) {
        delete i;
    }

    for(auto i : leftPongKongChow) {
        delete i;
    }

    for(auto i : rightPongKongChow) {
        delete i;
    }

    for(auto i :oppositePongKongChow) {
        delete i;
    }
    
}
