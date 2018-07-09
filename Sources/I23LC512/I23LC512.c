#include"I23LC512.h"

static unsigned char code
    READ=0x03,
    WRITE=0x02,
    EDIO=0x3b,
    EQIO=0x38,
    RSTIO=0xff,
    RDMR=0x05,
    WRMR=0x01;

static unsigned char code
    SPI_CPOL=0,
    SPI_CPHA=0,
    SPI_CLKDIV=0;

sbit chipSelect=P2^5;

unsigned char i23lc512WriteModeRegister(const unsigned char mode){
    spiSetIsOccupied(1);
    spiInitialize(SPI_CPOL,SPI_CPHA,SPI_CLKDIV);

    chipSelect=0;
    spiSend(mode);

    while(!spiTransmissionComplete());
    chipSelect=1;
    spiSetIsOccupied(0);

    return mode;
}

unsigned char i23lc512UCharWrite(const unsigned int address,const unsigned char c){
    spiSetIsOccupied(1);
    spiSetup(SPI_CPOL,SPI_CPHA,SPI_CLKDIV);
    chipSelect=0;

    spiSend(WRITE);
    spiSend(address>>8);
    spiSend(address);
    spiSend(c);

    while(!spiTransmissionComplete());
    chipSelect=1;
    spiSetIsOccupied(0);

    return c;
}

unsigned char i23lc512UCharArrayWrite(const unsigned int address,const unsigned int offset,const unsigned char c){
    return i23lc512UCharWrite(address+offset*sizeof(unsigned char),c);
}

unsigned char i23lc512UCharRead(const unsigned int address){
    unsigned char buffer;
    spiSetIsOccupied(1);
    spiSetup(SPI_CPOL,SPI_CPHA,SPI_CLKDIV);

    chipSelect=0;
    spiSend(READ);
    spiSend(address>>8);
    spiSend(address);
    buffer=spiRead();

    spiSetIsOccupied(0);

    chipSelect=1;
    return buffer;
}

unsigned char i23lc512UCharArrayRead(const unsigned int address,const unsigned int offset){
    return i23lc512UCharRead(address+offset*sizeof(unsigned char));
}

unsigned int i23lc512UIntWrite(const unsigned int address,const unsigned int i){
    spiSetIsOccupied(1);
    spiSetup(SPI_CPOL,SPI_CPHA,SPI_CLKDIV);
    chipSelect=0;

    spiSend(WRITE);
    spiSend(address>>8);
    spiSend(address);
    spiSend(i>>8);
    spiSend(i);

    while(!spiTransmissionComplete());
    chipSelect=1;
    spiSetIsOccupied(0);

    return i;
}

unsigned int i23lc512UIntArrayWrite(const unsigned int address,const unsigned int offset,const unsigned int i){
    return i23lc512UIntWrite(address+offset*sizeof(unsigned int),i);
}

unsigned int i23lc512UIntRead(const unsigned int address){  // ???
    unsigned int buffer;
    spiSetIsOccupied(1);
    spiSetup(SPI_CPOL,SPI_CPHA,SPI_CLKDIV);

    chipSelect=0;
    spiSend(READ);
    spiSend(address>>8);
    spiSend(address);
    buffer=spiRead();
    buffer<<=8;
    buffer|=spiRead();

    spiSetIsOccupied(0);

    chipSelect=1;
    return buffer;
}

unsigned int i23lc512UIntArrayRead(const unsigned int address,const unsigned int offset){
    return i23lc512UIntRead(address+offset*sizeof(unsigned int));
}

bit i23lc512Initialize(){
    chipSelect=1;
    // i23lc512WriteModeRegister(0x); // set operation mode, default sequential mode
    return 1;
}
