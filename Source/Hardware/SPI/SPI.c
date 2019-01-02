#include<Delay.h>
#include<SPI.h>
#include<STC15W4K48S4.h>
#include<Serial.h>

void SPI_init(){
    SPCTL=0xd0;
    P_SW1|=0x04;

    SPI_setBitOrder(SPI_MSBFIRST);
    SPI_setClockDivider(SPI_CLOCK_DIV4);
    SPI_setDataMode(SPI_MODE0);
    SPDAT=0x00;
}

void SPI_setBitOrder(unsigned char bitOrder){
    if(bitOrder==SPI_MSBFIRST){
        SPCTL&=~0x20;
    }else{
        SPCTL|=0x20;
    }
}

void SPI_setClockDivider(unsigned char clockDivider){
    SPCTL=SPCTL&0xfc|clockDivider;
}

void SPI_setDataMode(unsigned char dataMode){
    SPCTL=SPCTL&0xf3|dataMode;
}

unsigned char SPI_transfer(unsigned char imm8){
    SPSTAT|=0x80;
    SPDAT=imm8;
    while((SPSTAT&0x80)==0);
    return SPDAT;
}
