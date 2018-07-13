#include"Timer4.h"

void timer4Initialize(bit x12Mode,bit enableInterrupt,unsigned char th,unsigned char tl){
    if(x12Mode){
        T4T3M&=~0x20;
    }else{
        T4T3M|=0x20;
    }
    if(enableInterrupt){
        IE2|=0x40;
    }else{
        IE2&=~0x40;
    }
    T4L=th;
    T4H=tl;
}

void timer4Start(){
    T4T3M|=0x80;
}

void timer4Stop(){
    T4T3M&=~0x80;
}

static void interruptRoutine() interrupt 20{
    interruptBusyLed=1;
    systemClockTick();
    interruptBusyLed=0;
}
