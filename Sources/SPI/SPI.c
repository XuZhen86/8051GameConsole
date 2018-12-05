#include"../Main/STC15W4K48S4.h"
#include"./SPI.h"

void spi_initialize(){
    SPCTL=0xd0;
    P_SW1|=0x04;
    SPSTAT=0xc0;
    SPDAT=0x00;
}

void spi_setup(unsigned char clkDiv,bit cpol,bit cpha){
    while((SPSTAT&0x80)==0);
    SPCTL=0xd0|0x08*cpol|0x04*cpha|0x03&clkDiv;
}

unsigned char spi_send(unsigned char imm8){
    while((SPSTAT&0x80)==0);
    SPSTAT|=0x80;
    SPDAT=imm8;
    return imm8;
}

unsigned char *spi_sendSeq(unsigned char *src,unsigned int len){
    unsigned int i;
    for(i=0;i<len;i++){
        while((SPSTAT&0x80)==0);
        SPSTAT|=0x80;
        SPDAT=src[i];
    }
    return src;
}

unsigned char spi_sendN(unsigned char imm8,unsigned int count){
    while(count--){
        while((SPSTAT&0x80)==0);
        SPSTAT|=0x80;
        SPDAT=imm8;
    }
    return imm8;
}

unsigned char spi_recv(){
    while((SPSTAT&0x80)==0);
    SPSTAT|=0x80;
    SPDAT=0x00;
    while((SPSTAT&0x80)==0);
    return SPDAT;
}

unsigned char *spi_recvSeq(unsigned char *dst,unsigned int len){
    unsigned int i;
    while((SPSTAT&0x80)==0);
    SPSTAT|=0x80;
    SPDAT=0x00;

    for(i=0;i<len-1;i++){
        while((SPSTAT&0x80)==0);
        dst[i]=SPDAT;
        SPSTAT|=0x80;
        SPDAT=0x00;
    }

    while((SPSTAT&0x80)==0);
    dst[len-1]=SPDAT;
    return dst;
}

void spi_waitFinish(){
    while((SPSTAT&0x80)==0);
}
