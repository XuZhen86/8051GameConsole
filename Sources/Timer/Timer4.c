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

static unsigned char code LCD12864_COUNTER_MAX=100;
static unsigned char lcd12864Counter=1;

static void interruptRoutine() interrupt 20{
    interruptBusyLed=1;
    timer4Stop();

    lcd12864Counter--;
    if(!lcd12864Counter&&!spiGetIsOccupied()){
        lcd12864Counter=LCD12864_COUNTER_MAX;
        lcd12864GdramFlush(0);
    }else if(!lcd12864Counter){
        lcd12864Counter=rand()%8+1;
    }

    timer4Start();
    interruptBusyLed=0;
}
