#include"Sources/Main/STC15W4K48S4.h"

#include"Sources/Power/Power.h"

sbit systemBusyLed=P2^5;

void power_idle(){
    systemBusyLed=0;
    PCON|=0x01;
    systemBusyLed=1;
}
