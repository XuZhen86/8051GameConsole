#include"Sources/Main/STC15W4K48S4.h"

#include"Sources/Timer/Timer3.h"

void timer_3_initialize(bit x12Mode,unsigned char th,unsigned char tl){
    if(x12Mode){
        T4T3M&=0xFD;
    }
    T3H=th;
    T3L=tl;
}

void timer_3_start(){
    IE2|=0x20;
    T4T3M|=0x08;
}

void timer_3_stop(){
    T4T3M&=~0x08;
}

static void interruptRoutine() interrupt 19 {
}
