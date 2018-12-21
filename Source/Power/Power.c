#include"Power.h"
#include<STC15W4K48S4.h>

sbit systemBusyLed=P2^5;

void Power_idle(){
    systemBusyLed=0;
    PCON|=0x01;
    systemBusyLed=1;
}

void Power_powerDown(){
    systemBusyLed=0;
    PCON|=0x02;
    systemBusyLed=1;
}
