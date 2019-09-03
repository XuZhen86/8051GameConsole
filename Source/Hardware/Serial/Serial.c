#include"SerialStatic.h"
#include<STC15W4K48S4.h>
#include<Serial.h>
#include<Timer.h>

// Uses a buffer to reduce blocking to main function
// Buffer is in pdata to take advantage of 8051 pdata mechanism
static char pdata buf[256];
// Ring buffer
static unsigned char data bufHead,bufTail,bufData;

char putchar(char c){
    // Have to wait until there is space for new data
    while((unsigned char)(bufTail+1)==bufHead){}

    // Put in the new data
    bufTail++;
    buf[bufTail]=c;

    // Start timer to periodically interrupt main function
    Timer2_start();

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

// Called by hw interruption
static void s1SendNext() interrupt 12{
    // If there is pending data, send it
    if(bufHead!=bufTail){
        bufHead++;
        bufData=(unsigned char)buf[bufHead];
        while(!TI){}
        SBUF=bufData;
        TI=0;
    }else{
        // Otherwise the timer can be stopped to save energy
        Timer2_stop();
    }
}
