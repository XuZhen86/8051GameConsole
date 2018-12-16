#include"../Main/STC15W4K48S4.h"
#include"../Timer/Timer.h"
#include"Serial.h"

char putchar(char c){
    while(!TI);
    SBUF=c;
    TI=0;
    return c;
}

void serial1_initialize(unsigned char imm8Th,unsigned char imm8Tl,unsigned char mode){
    SCON=SCON&~0x60|(mode&0x03)<<6;
    AUXR&=~0x01;
    TI=1;
    timer1_setup(imm8Th,imm8Tl,TIMER_MODE_0|TIMER_X12);
    timer1_start();
}
