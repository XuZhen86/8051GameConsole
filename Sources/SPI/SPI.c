#include"SPI.h"

static bit isOccupied=0;

void spiSetIsOccupied(bit o){
    isOccupied=o;
}

bit spiGetIsOccupied(){
    return isOccupied;
}

void spiInitialize(bit cpol,bit cpha,unsigned char clkDiv){
    SPCTL=(0xd0|(0x08*cpol)|(0x04*cpha)|(0x03&clkDiv));
    P_SW1|=0x04;
    SPSTAT=0xc0;
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

unsigned char spiRecv(){
    while(!(SPSTAT&0x80));
    SPSTAT|=0x80;
    SPDAT=0x00;
    while(!(SPSTAT&0x80));
    return SPDAT;
}

unsigned char *spiSeqRecv(unsigned char *destination,unsigned int length){
    unsigned int data i;
    unsigned char data buffer;

    while(!(SPSTAT&0x80));
    SPSTAT|=0x80;
    SPDAT=0x00;

    for(i=0;i<length;i++){
        while(!(SPSTAT&0x80));
        buffer=SPDAT;
        SPSTAT|=0x80;
        SPDAT=0x00;
        destination[i]=buffer;
    }

    return destination;
}

bit spiTransmissionComplete(){
    return (SPSTAT&0x80)!=0;
}
