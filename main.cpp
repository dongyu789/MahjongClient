

#include <stdio.h>
#include "MyPrint.h"
#include "Controller.h"
#include "Mahjong.h"
int main() {
    system("color 8e");
    Controller *controller = new Controller();
    controller->start();
    
    
    Sleep(100000);

    return 0;
}