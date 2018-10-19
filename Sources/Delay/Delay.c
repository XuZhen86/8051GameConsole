#include"Sources/Main/STC15W4K48S4.h"
#include"Sources/Timer/Timer.h"
#include"Sources/Power/Power.h"

#include"Sources/Delay/Delay.h"

void delay(unsigned int ms){
    unsigned char t=TH0;
    while(ms--){
        power_idle();
    }
    while(TH0<t);
}

void delayLoop(unsigned char i,unsigned char j,unsigned char k){
    do{
        do{
            while(k--);
        }while(j--);
    }while(i--);
}
