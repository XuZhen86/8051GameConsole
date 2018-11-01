#include"Sources/Main/STC15W4K48S4.h"

#include"Sources/SQI/SQI.h"

sbit SCLK=P1^0;
sfr SIO=0x80;
sfr SIOM0=0x94;
sfr SIOM1=0x93;

void sqi_initialize(){
    SIOM0=0x00;
    SCLK=0;
}

unsigned char sqi_send(unsigned char imm8){
    // SIOM1=0x00;
    #pragma asm
        mov SIO,A
        setb SCLK
        clr SCLK

        swap A

        mov SIO,A
        setb SCLK
        clr SCLK
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
    // SIO=0x00;
    // SIOM1=0xff;
    #pragma asm
        setb SCLK
        mov A,SIO
        clr SCLK
        swap A

        setb SCLK
        orl A,SIO
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
