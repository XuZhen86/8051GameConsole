#include"Sources/I23LC512/I23LC512.h"

#include"Sources/Main/STC15W4K48S4.h"
#include"Sources/Universal/Universal.h"
#include"Sources/SPI/SPI.h"

enum I23LC512_COMMAND{
    READ=0x03,
    WRITE=0x02,
    EDIO=0x3b,
    EQIO=0x38,
    RSTIO=0xff,
    RDMR=0x05,
    WRMR=0x01
};

enum I23LC512_SPI_CONFIG{
    SPI_CPOL=0,
    SPI_CPHA=0,
    SPI_CLKDIV=0
};

sbit chipSelect=P2^6;

unsigned char i23lc512_writeModeRegister(unsigned char mode){
    spi_isOccupiedSet(1);
    spi_initialize(SPI_CPOL,SPI_CPHA,SPI_CLKDIV);
    chipSelect=0;

    spi_send(mode);

    while(!spi_transmissionCompleteGet());
    chipSelect=1;
    spi_isOccupiedSet(0);
    return mode;
}

unsigned char i23lc512_uCharWrite(unsigned int address,unsigned char c){
    spi_isOccupiedSet(1);
    spi_setup(SPI_CPOL,SPI_CPHA,SPI_CLKDIV);
    chipSelect=0;

    spi_send(WRITE);
    spi_send(address>>8);
    spi_send(address);
    spi_send(c);

    while(!spi_transmissionCompleteGet());
    chipSelect=1;
    spi_isOccupiedSet(0);
    return c;
}

unsigned char i23lc512_uCharArrayWrite(unsigned int address,unsigned int offset,unsigned char c){
    return i23lc512_uCharWrite(address+offset*sizeof(unsigned char),c);
}

unsigned char i23lc512_uCharRead(unsigned int address){
    unsigned char idata buffer;
    spi_isOccupiedSet(1);
    spi_setup(SPI_CPOL,SPI_CPHA,SPI_CLKDIV);
    chipSelect=0;

    spi_send(READ);
    spi_send(address>>8);
    spi_send(address);
    buffer=spi_recv();

    chipSelect=1;
    spi_isOccupiedSet(0);
    return buffer;
}

unsigned char i23lc512_uCharArrayRead(unsigned int address,unsigned int offset){
    return i23lc512_uCharRead(address+offset*sizeof(unsigned char));
}

unsigned int i23lc512_uIntWrite(unsigned int address,unsigned int i){
    spi_isOccupiedSet(1);
    spi_setup(SPI_CPOL,SPI_CPHA,SPI_CLKDIV);
    chipSelect=0;

    spi_send(WRITE);
    spi_send(address>>8);
    spi_send(address);
    spi_send(i>>8);
    spi_send(i);

    while(!spi_transmissionCompleteGet());
    chipSelect=1;
    spi_isOccupiedSet(0);
    return i;
}

unsigned int i23lc512_uIntArrayWrite(unsigned int address,unsigned int offset,unsigned int i){
    return i23lc512_uIntWrite(address+offset*sizeof(unsigned int),i);
}

unsigned int i23lc512_uIntRead(unsigned int address){
    unsigned char idata buffer[2];

    spi_isOccupiedSet(1);
    spi_setup(SPI_CPOL,SPI_CPHA,SPI_CLKDIV);
    chipSelect=0;

    spi_send(READ);
    spi_send(address>>8);
    spi_send(address);

    buffer[0]=spi_recv();
    buffer[1]=spi_recv();

    chipSelect=1;
    spi_isOccupiedSet(0);
    return *(unsigned int*)buffer;
}

unsigned int i23lc512_uIntArrayRead(unsigned int address,unsigned int offset){
    return i23lc512_uIntRead(address+offset*sizeof(unsigned int));
}

bit i23lc512_initialize(){
    chipSelect=1;
    // i23lc512_writeModeRegister(0x); // set operation mode, default sequential mode
    return 1;
}

unsigned char *i23lc512_uCharSeqRead(unsigned char *destination,unsigned int address,unsigned int length){
    spi_isOccupiedSet(1);
    spi_setup(SPI_CPOL,SPI_CPHA,SPI_CLKDIV);
    chipSelect=0;

    spi_send(READ);
    spi_send(address>>8);
    spi_send(address);
    spi_seqRecv(destination,length);

    chipSelect=1;
    spi_isOccupiedSet(0);
    return destination;
}

unsigned char *i23lc512_uCharSeqWrite(unsigned char *source,unsigned int address,unsigned int length){
    unsigned int idata i;

    spi_isOccupiedSet(1);
    spi_setup(SPI_CPOL,SPI_CPHA,SPI_CLKDIV);
    chipSelect=0;

    spi_send(WRITE);
    spi_send(address>>8);
    spi_send(address);

    for(i=0;i<length;i++){
        spi_send(source[i]);
    }

    while(!spi_transmissionCompleteGet());
    chipSelect=1;
    spi_isOccupiedSet(0);
    return source;
}

unsigned int i23lc512_memset(unsigned int address,unsigned char value,unsigned int length){
    unsigned int idata i;

    spi_isOccupiedSet(1);
    spi_setup(SPI_CPOL,SPI_CPHA,SPI_CLKDIV);
    chipSelect=0;

    spi_send(WRITE);
    spi_send(address>>8);
    spi_send(address);

    for(i=0;i<length;i++){
        spi_send(value);
    }

    while(!spi_transmissionCompleteGet());
    spi_isOccupiedSet(0);
    chipSelect=1;
    return address;
}

unsigned int i23lc512_memcpy(unsigned int destination,unsigned int source,unsigned int length){
    unsigned char buffer[32];
    unsigned int idata i;

    for(i=0;i+32>i&&i+32<length;i+=32){
        i23lc512_uCharSeqRead(buffer,source+i,32);
        i23lc512_uCharSeqWrite(buffer,destination+i,32);
    }

    i23lc512_uCharSeqRead(buffer,source+i,length-i);
    i23lc512_uCharSeqWrite(buffer,destination+i,length-i);

    return destination;
}
