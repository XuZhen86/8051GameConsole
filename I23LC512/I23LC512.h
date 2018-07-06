#ifndef __I23LC512_H_
#define __I23LC512_H_

#include<Main/STC15W4K48S4.h>
#include<Main/Universal.h>
#include<SPI/SPI.h>

unsigned char code
    I23LC512_READ=0x03,
    I23LC512_WRITE=0x02,
    I23LC512_RDMR=0x05,
    I23LC512_WRMR=0x01;

sbit i23lc512ChipSelect=P2^5;

unsigned int addressPointer=0;

void i23lc512WriteModeRegister(unsigned char mode){
    while(spiGetOccupied());
    spiSetOccupied(1);
    spiInitialize(0,0);

    i23lc512ChipSelect=0;
    spiSend(mode);
    while(!spiTransmissionComplete());
    i23lc512ChipSelect=1;

    spiSetOccupied(0);
}

void i23lc512ByteWrite(unsigned int address,unsigned char c){
    while(spiGetOccupied());
    spiSetOccupied(1);
    spiSetup(0,0,0);

    i23lc512ChipSelect=0;

    spiSend(I23LC512_WRITE);
    spiSend(address>>8);
    spiSend(address);
    spiSend(c);

    while(!spiTransmissionComplete());
    i23lc512ChipSelect=1;
    spiSetOccupied(0);
}

unsigned char i23lc512ByteRead(unsigned int address){
    unsigned char buffer;

    while(spiGetOccupied());
    spiSetOccupied(1);
    spiSetup(0,0,0);

    i23lc512ChipSelect=0;
    spiSend(I23LC512_READ);
    spiSend(address>>8);
    spiSend(address);
    buffer=spiRead();

    i23lc512ChipSelect=1;
    spiSetOccupied(0);

    return buffer;
}

void i23lc512Initialize(){
    i23lc512ChipSelect=1;
    // i23lc512WriteModeRegister(0x); // set operation mode, default sequential mode
}



#endif
