#include"Sources/Main/STC15W4K48S4.h"
#include"Sources/InterruptRoutine/InterruptRoutine.h"
#include"Sources/Universal/SystemClock.h"

#include"Sources/Timer/Timer4.h"

void timer_4_initialize(bit x12Mode,bit enableInterrupt,unsigned char th,unsigned char tl){
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
    T4H=th;
    T4L=tl;
}

void timer_4_start(){
    T4T3M|=0x80;
}

void timer_4_stop(){
    T4T3M&=~0x80;
}

static void interruptRoutine() interrupt 20{
    interrupt_busyLed=1;
    systemClock_tick();
    interrupt_busyLed=0;
}
