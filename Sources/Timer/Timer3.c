#include"Timer3.h"

void timer3Initialize(bit x12Mode,unsigned char th,unsigned char tl){
    if(x12Mode){
        T4T3M&=0xFD;
    }
    T3L=th;
    T3H=tl;
}

void timer3Start(){
    IE2|=0x20;
    T4T3M|=0x08;
}

void timer3Stop(){
    T4T3M&=~0x08;
}

static void interruptRoutine() interrupt 19 {
}
