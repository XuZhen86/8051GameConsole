#include<Sources/Main/STC15W4K48S4.h>

#include"Sources/SPI/SPI.h"

static bit isOccupied=0;

void spi_isOccupiedSet(bit o){
    isOccupied=o;
}

bit spi_isOccupiedGet(){
    return isOccupied;
}

void spi_initialize(bit cpol,bit cpha,unsigned char clkDiv){
    SPCTL=(0xd0|(0x08*cpol)|(0x04*cpha)|(0x03&clkDiv));
    P_SW1|=0x04;
    SPSTAT=0xc0;
    SPDAT=0x00;
}

void spi_setup(bit cpol,bit cpha,unsigned char clkDiv){
    while(!(SPSTAT&0x80));
    SPCTL=(0xd0|(0x08*cpol)|(0x04*cpha)|(0x03&clkDiv));
}

unsigned char spi_send(unsigned char c){
    while(!(SPSTAT&0x80));
    SPSTAT|=0x80;
    SPDAT=c;
    return c;
}

unsigned char spi_recv(){
    while(!(SPSTAT&0x80));
    SPSTAT|=0x80;
    SPDAT=0x00;
    while(!(SPSTAT&0x80));
    return SPDAT;
}

unsigned char *spi_seqRecv(unsigned char *destination,unsigned int length){
    unsigned int data i;
    unsigned char data buffer;

    while(!(SPSTAT&0x80));
    SPSTAT|=0x80;
    SPDAT=0x00;

    for(i=0;i<length-1;i++){
        while(!(SPSTAT&0x80));
        buffer=SPDAT;
        SPSTAT|=0x80;
        SPDAT=0x00;
        destination[i]=buffer;
    }

    while(!(SPSTAT&0x80));
    buffer=SPDAT;
    destination[length-1]=buffer;

    return destination;
}

bit spi_transmissionCompleteGet(){
    return (SPSTAT&0x80)!=0;
}
