#include"../Main/STC15W4K48S4.h"
#include"../Timer/Timer.h"
#include"../Power/Power.h"
#include"./Delay.h"

void delay(unsigned int ms) small{
    unsigned char t=TH0;
    while(ms--){
        power_idle();
    }
    while(TH0<t);
}

void delayLoop(unsigned char i,unsigned char j,unsigned char k) small{
    do{
        do{
            while(k--);
        }while(j--);
    }while(i--);
}
