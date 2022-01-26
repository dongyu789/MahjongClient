

#include <stdio.h>
#include "MyPrint.h"
#include "Controller.h"
#include "Mahjong.h"
int main() {
    Controller *controller = new Controller();
    controller->start();
    
    
    Sleep(100000);

    return 0;
}