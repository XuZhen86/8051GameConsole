#include"SPI.h"

static bit isOccupied=0;

void spiSetIsOccupied(bit o){
    isOccupied=o;
}

bit spiGetIsOccupied(){
    return isOccupied;
}

void spiInitialize(bit cpol,bit cpha,unsigned char clkDiv){
    SPSTAT=0xc0;    //清中断标志
    SPCTL=(0xd0|(0x08*cpol)|(0x04*cpha)|(0x03&clkDiv));
    P_SW1|=0x00;
    SPDAT=0x00;
}

void spiSetup(bit cpol,bit cpha,unsigned char clkDiv){
    while(!(SPSTAT&0x80));
    SPCTL=(0xd0|(0x08*cpol)|(0x04*cpha)|(0x03&clkDiv));
}

unsigned char spiSend(unsigned char c){
    while(!(SPSTAT&0x80));
    SPSTAT|=0x80;
    SPDAT=c;
    return c;
}

unsigned char spiRead(){
    spiSend(0x00);
    while(!(SPSTAT&0x80));
    return SPDAT;
}

unsigned char *spiSeqRead(unsigned char *destination,unsigned int length){
    unsigned int i;

    while(!(SPSTAT&0x80));
    SPSTAT|=0x80;
    SPDAT=0x00;

    for(i=0;i<length;i++){
        while(!(SPSTAT&0x80));
        destination[i]=SPDAT;
        SPSTAT|=0x80;
        SPDAT=0x00;
    }

    return destination;
}

bit spiTransmissionComplete(){
    return (SPSTAT&0x80)!=0;
}
