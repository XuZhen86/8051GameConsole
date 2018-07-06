#ifndef __SPI_H_
#define __SPI_H_

#include<Main/STC15W4K48S4.h>

bit spiOccupied=0;

bit spiTransmissionComplete();

void spiSetOccupied(bit occupied){
    spiOccupied=occupied;
}

bit spiGetOccupied(){
    return spiOccupied;
}

void spiInitialize(bit cpol,bit cpha){
    SPSTAT=0xc0;    //清中断标志
    SPCTL=(0xd3|(0x08*cpol)|(0x04*cpha));     // spi clk div 32
    P_SW1|=0x00;
    SPDAT=0x00;     // reset spi data buffer
}

void spiSetup(bit cpol,bit cpha,unsigned char clkDiv){
    while(!(SPSTAT&0x80));
    SPCTL=(0xd3|(0x08*cpol)|(0x04*cpha)|(0x03&clkDiv));     // spi clk div 32
}

void spiSend(unsigned char c){
    while(!(SPSTAT&0x80));
    SPSTAT|=0x80;
    SPDAT=c;
}

unsigned char spiRead(){
    while(!(SPSTAT&0x80));
    SPSTAT|=0x80;
    SPDAT=0x00;
    while(!(SPSTAT&0x80));
    return SPDAT;
}

bit spiTransmissionComplete(){
    return (SPSTAT&0x80)!=0;
}

// void spiSendString(unsigned char *str,unsigned char len){
//     unsigned char i;
//     for(i=0;i<len;i++){
//         SPDAT=str[i];
//         while(!(SPSTAT&0x80));
//         SPSTAT|=0x80;
//     }
// }

#endif
