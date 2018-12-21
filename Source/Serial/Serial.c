#include<STC15W4K48S4.h>
#include<Serial.h>
#include<Timer.h>

sbit led=P2^4;

char putchar(char c){
    led=1;
    while(!TI);
    SBUF=c;
    TI=0;
    led=0;
    return c;
}

void Serial1_init(){
    led=0;
    Serial1_setMode(1);
    TI=1;

    Timer1_setCounter(0xffb8);
    Timer1_setMode(0);
    Timer1_setX12Enabled(1);
    Timer1_start();
}

void Serial1_setMode(unsigned char mode){
    SCON=SCON&~0xc0|(mode&0x03)<<6;
    AUXR&=~0x01;
}
