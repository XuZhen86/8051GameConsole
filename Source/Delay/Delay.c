#include"Delay.h"
#include<Power.h>
#include<Timer.h>

void delay(unsigned int ms){
    unsigned char t=Timer0_elapsed();
    while(ms--){
        Power_idle();
    }
    while(Timer0_elapsed()<t);
}

void delayLoop(unsigned char i,unsigned char j,unsigned char k){
    do{
        do{
            while(k--);
        }while(j--);
    }while(i--);
}
