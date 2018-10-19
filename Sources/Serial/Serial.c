#include"Sources/Main/STC15W4K48S4.h"
#include"Sources/Timer/Timer.h"

#include"Sources/Serial/Serial.h"

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
    timer1_setup(imm8Th,imm8Tl,0,1,0,0);
    timer1_start();
}
