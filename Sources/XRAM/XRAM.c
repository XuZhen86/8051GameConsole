#include"Sources/SPI/SPI.h"

#include"Sources/XRAM/XRAM.h"
#include"Sources/XRAM/XRAMConfig.h"

void xRam_initialize(){
    CS=1;
    redLed=0;
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
    unsigned char buffer;
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
    unsigned char buffer[2];
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
    spi_setup(CPOL,CPHA,CLKDIV);
    CS=0;

    spi_send(WRITE);
    spi_send(m16>>8);
    spi_send(m16);
    spi_sendSeq(src,len);

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
    spi_sendN(imm8,len);

    spi_waitFinish();
    CS=1;
    return m16Dst;
}

unsigned int xRam_memcpy(unsigned int m16Dst,unsigned int m16Src,unsigned int len){
    unsigned char buffer[32];
    unsigned int i;

    for(i=0;i+32>i&&i+32<len;i+=32){
        xRam_uCharReadSeq(buffer,m16Src+i,32);
        xRam_uCharWriteSeq(buffer,m16Dst+i,32);
    }

    xRam_uCharReadSeq(buffer,m16Src+i,len-i);
    xRam_uCharWriteSeq(buffer,m16Dst+i,len-i);

    return m16Dst;
}

// #include<stdlib.h>
// #include<stdio.h>
// #include"Sources/Clock/Clock.h"

// void _xRam_test(){
//     unsigned char data ucVar;
//     unsigned int data i,j,errCount,uiVar;

//     unsigned long int time=clock_get();

//     errCount=0;
//     for(i=0;i<0xffff;i++){
//         ucVar=rand();
//         xRam_uCharWrite(i,ucVar);
//         if(xRam_uCharRead(i)!=ucVar){
//             errCount++;
//         }
//     }
//     printf("[%5u ",errCount);

//     errCount=0;
//     for(i=0;i<0xffff;i++){
//         ucVar=rand();
//         uiVar=rand();
//         xRam_uCharWrite(uiVar,ucVar);
//         if(xRam_uCharRead(uiVar)!=ucVar){
//             errCount++;
//         }
//     }
//     printf("%5u ",errCount);

//     errCount=0;
//     for(i=0;i<0xffff-2;i+=2){
//         uiVar=rand();
//         // uiVar=0x3232;
//         xRam_uIntWrite(i,uiVar);
//         if(xRam_uIntRead(i)!=uiVar){
//             errCount++;
//         }
//     }
//     printf("%5u ",errCount);

//     errCount=0;
//     for(i=0;i<0xffff-31;i+=32){
//         ucVar=rand();
//         xRam_memset(i,ucVar,32);
//         for(j=0;j<32;j++){
//             if(xRam_uCharRead(i+j)!=ucVar){
//                 errCount++;
//             }
//         }
//     }
//     printf("%5u] ",errCount);

//     printf("%u\n",(unsigned int)(clock_get()-time));
// }
