#include"Sources/SPI/SPI.h"

#include"Sources/XRAM/XRAM.h"
#include"Sources/XRAM/XRAMConfig.h"

void xRam_initialize(){
    CS=1;
    xRam_writeModeRegister(SEQUENTIAL_MODE);
}

unsigned char xRam_writeModeRegister(unsigned char mode){
    spi_setup(CPOL,CPHA,CLKDIV);
    CS=0;

    spi_send(WRMR);
    spi_send(mode);

    spi_waitFinish();
    CS=1;
    return mode;
}

unsigned char xRam_uCharWrite(unsigned int m16,unsigned char imm8){
    spi_setup(CPOL,CPHA,CLKDIV);
    CS=0;

    spi_send(WRITE);
    spi_send(m16>>8);
    spi_send(m16);
    spi_send(imm8);

    spi_waitFinish();
    CS=1;
    return imm8;
}

unsigned char xRam_uCharRead(unsigned int m16){
    unsigned char idata buffer;
    spi_setup(CPOL,CPHA,CLKDIV);
    CS=0;

    spi_send(READ);
    spi_send(m16>>8);
    spi_send(m16);
    buffer=spi_recv();

    CS=1;
    return buffer;
}

unsigned int xRam_uIntWrite(unsigned int m16,unsigned int imm16){
    spi_setup(CPOL,CPHA,CLKDIV);
    CS=0;

    spi_send(WRITE);
    spi_send(m16>>8);
    spi_send(m16);
    spi_send(imm16>>8);
    spi_send(imm16);

    spi_waitFinish();
    CS=1;
    return imm16;
}

unsigned int xRam_uIntRead(unsigned int m16){
    unsigned char idata buffer[2];
    spi_setup(CPOL,CPHA,CLKDIV);
    CS=0;

    spi_send(READ);
    spi_send(m16>>8);
    spi_send(m16);

    buffer[0]=spi_recv();
    buffer[1]=spi_recv();

    CS=1;
    return *(unsigned int*)buffer;
}

unsigned char *xRam_uCharReadSeq(unsigned char *dst,unsigned int m16,unsigned int len){
    spi_setup(CPOL,CPHA,CLKDIV);
    CS=0;

    spi_send(READ);
    spi_send(m16>>8);
    spi_send(m16);
    spi_recvSeq(dst,len);

    CS=1;
    return dst;
}

unsigned char *xRam_uCharWriteSeq(unsigned char *src,unsigned int m16,unsigned int len){
    unsigned int idata i;
    spi_setup(CPOL,CPHA,CLKDIV);
    CS=0;

    spi_send(WRITE);
    spi_send(m16>>8);
    spi_send(m16);

    for(i=0;i<len;i++){
        spi_send(src[i]);
    }

    spi_waitFinish();
    CS=1;
    return src;
}

unsigned int xRam_memset(unsigned int m16Dst,unsigned char imm8,unsigned int len){
    spi_setup(CPOL,CPHA,CLKDIV);
    CS=0;

    spi_send(WRITE);
    spi_send(m16Dst>>8);
    spi_send(m16Dst);

    while(len--){
        spi_send(imm8);
    }

    spi_waitFinish();
    CS=1;
    return m16Dst;
}

unsigned int xRam_memcpy(unsigned int m16Dst,unsigned int m16Src,unsigned int len){
    unsigned char buffer[32];
    unsigned int idata i;

    for(i=0;i+32>i&&i+32<len;i+=32){
        xRam_uCharReadSeq(buffer,m16Src+i,32);
        xRam_uCharWriteSeq(buffer,m16Dst+i,32);
    }

    xRam_uCharReadSeq(buffer,m16Src+i,len-i);
    xRam_uCharWriteSeq(buffer,m16Dst+i,len-i);

    return m16Dst;
}
