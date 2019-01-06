#include"SerialStatic.h"
#include<STC15W4K48S4.h>
#include<Serial.h>
#include<Timer.h>

static char pdata buf[256];
static unsigned char data bufHead,bufTail,bufData;

char putchar(char c){
    while((unsigned char)(bufTail+1)==bufHead){}
    bufTail++;
    buf[bufTail]=c;
    AUXR|=0x10; // Timer2_start();
    return c;
}

void Serial1_init(){
    Serial1_setMode(1);
    TI=1;

    Timer1_setCounter(0xffdc);
    Timer1_setMode(0);
    Timer1_setX12Enabled(0);
    Timer1_start();

    Timer2_setCounter(0xfa60);
    Timer2_setX12Enabled(0);
    Timer2_setInterruptEnabled(1);

    bufHead=0;
    bufTail=0;
}

void Serial1_setMode(unsigned char mode){
    SCON=SCON&~0xc0|(mode&0x03)<<6;
    AUXR&=~0x01;
}

static void s1SendNext() interrupt 12{
    if(bufHead!=bufTail){
        bufHead++;
        bufData=(unsigned char)buf[bufHead];
        while(!TI){}
        SBUF=bufData;
        TI=0;
    }else{
        AUXR&=~0x10;    // Timer2_stop();
    }
}
