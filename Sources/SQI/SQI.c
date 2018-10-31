#include"Sources/Main/STC15W4K48S4.h"
#include"Sources/Delay/Delay.h"
#include"Sources/SQI/SQI.h"

#include<stdio.h>

sfr IO=0x80;
sbit SCLK=P1^0;

void sqi_initialize(){
    P0M0=0x00;
    SCLK=0;
}

unsigned char sqi_send(unsigned char imm8){
    #pragma asm
        mov P0M1,#000h

        mov IO,A
        setb SCLK
        clr SCLK
        swap A

        mov IO,A
        setb SCLK
        clr SCLK
        swap A
    #pragma endasm
}

unsigned char *sqi_sendSeq(unsigned char *src,unsigned int len){
    unsigned int i;
    for(i=0;i<len;i++){
        sqi_send(src[i]);
    }
    return src;
}

unsigned char sqi_sendN(unsigned char imm8,unsigned int count){
    while(count--){
        sqi_send(imm8);
    }
    return imm8;
}

unsigned char sqi_recv(){
    #pragma asm
        mov P0M1,#0ffh

        setb SCLK
        mov A,IO
        clr SCLK
        swap A

        setb SCLK
        orl A,IO
        clr SCLK
        swap A

        mov R7,A
    #pragma endasm
}

unsigned char *sqi_recvSeq(unsigned char *dst,unsigned int len){
    unsigned int i;

    for(i=0;i<len-1;i++){
        dst[i]=sqi_recv();
    }

    return dst;
}
