#include"Sources/SQI/SQI.h"

#include"Sources/XRAM2/XRAM2.h"
#include"Sources/XRAM2/XRAM2Config.h"

void xRam2_enterSqiMode(){
    unsigned char i;
    CS=0;
    for(i=0;i<8;i++){
        SI=EQIO&1<<7-i;
        SCLK=1;
        SCLK=0;
    }
    CS=1;
}

void xRam2_initialize(){
    SCLK=0;
    CS=1;
    SI=0;
    xRam2_enterSqiMode();

    SIOM0=0x00;
    xRam2_writeModeRegister(SEQUENTIAL_MODE);
}

unsigned char xRam2_writeModeRegister(unsigned char mode){
    CS=0;

    SIOM1=0x00;
    sqi_send(WRMR);
    sqi_send(mode);

    CS=1;
    return mode;
}

unsigned char xRam2_uCharWrite(unsigned int m16,unsigned char imm8){
    CS=0;

    SIOM1=0x00;
    sqi_send(WRITE);
    sqi_send(m16>>8);
    sqi_send(m16);
    sqi_send(imm8);

    CS=1;
    return imm8;
}

unsigned char xRam2_uCharRead(unsigned int m16){
    unsigned char buffer;
    CS=0;

    SIOM1=0x00;
    sqi_send(READ);
    sqi_send(m16>>8);
    sqi_send(m16);

    SIO=0x00;SIOM1=0xff;
    sqi_recv();
    buffer=sqi_recv();

    CS=1;
    return buffer;
}

unsigned int xRam2_uIntWrite(unsigned int m16,unsigned int imm16){
    CS=0;

    SIOM1=0x00;
    sqi_send(WRITE);
    sqi_send(m16>>8);
    sqi_send(m16);
    sqi_send(imm16>>8);
    sqi_send(imm16);

    CS=1;
    return imm16;
}

unsigned int xRam2_uIntRead(unsigned int m16){
    unsigned char buffer[2];
    CS=0;

    SIOM1=0x00;
    sqi_send(READ);
    sqi_send(m16>>8);
    sqi_send(m16);
    SIO=0x00;SIOM1=0xff;
    sqi_recv();

    buffer[0]=sqi_recv();
    buffer[1]=sqi_recv();

    CS=1;
    return *(unsigned int*)buffer;
}

unsigned char *xRam2_uCharReadSeq(unsigned char *dst,unsigned int m16,unsigned int len){
    CS=0;

    SIOM1=0x00;
    sqi_send(READ);
    sqi_send(m16>>8);
    sqi_send(m16);

    SIO=0x00;SIOM1=0xff;
    sqi_recv();
    sqi_recvSeq(dst,len);

    CS=1;
    return dst;
}

unsigned char *xRam2_uCharWriteSeq(unsigned char *src,unsigned int m16,unsigned int len){
    CS=0;

    SIOM1=0x00;
    sqi_send(WRITE);
    sqi_send(m16>>8);
    sqi_send(m16);
    sqi_sendSeq(src,len);

    CS=1;
    return src;
}

unsigned int xRam2_memset(unsigned int m16Dst,unsigned char imm8,unsigned int len){
    CS=0;

    SIOM1=0x00;
    sqi_send(WRITE);
    sqi_send(m16Dst>>8);
    sqi_send(m16Dst);
    sqi_sendN(imm8,len);

    CS=1;
    return m16Dst;
}

unsigned int xRam2_memcpy(unsigned int m16Dst,unsigned int m16Src,unsigned int len){
    unsigned char buffer[32];
    unsigned int i;

    for(i=0;i+32>i&&i+32<len;i+=32){
        xRam2_uCharReadSeq(buffer,m16Src+i,32);
        xRam2_uCharWriteSeq(buffer,m16Dst+i,32);
    }

    xRam2_uCharReadSeq(buffer,m16Src+i,len-i);
    xRam2_uCharWriteSeq(buffer,m16Dst+i,len-i);

    return m16Dst;
}

// #include<stdlib.h>
// #include<stdio.h>
// #include"Sources/Clock/Clock.h"

// void _xRam2_test(){
//     unsigned char data ucVar;
//     unsigned int data i,j,errCount,uiVar;

//     unsigned long int time=clock_get();

//     errCount=0;
//     for(i=0;i<0xffff;i++){
//         ucVar=rand();
//         xRam2_uCharWrite(i,ucVar);
//         if(xRam2_uCharRead(i)!=ucVar){
//             errCount++;
//         }
//     }
//     printf("[%5u ",errCount);

//     errCount=0;
//     for(i=0;i<0xffff;i++){
//         ucVar=rand();
//         uiVar=rand();
//         xRam2_uCharWrite(uiVar,ucVar);
//         if(xRam2_uCharRead(uiVar)!=ucVar){
//             errCount++;
//         }
//     }
//     printf("%5u ",errCount);

//     errCount=0;
//     for(i=0;i<0xffff-2;i+=2){
//         uiVar=rand();
//         xRam2_uIntWrite(i,uiVar);
//         if(xRam2_uIntRead(i)!=uiVar){
//             errCount++;
//         }
//     }
//     printf("%5u ",errCount);

//     errCount=0;
//     for(i=0;i<0xffff-31;i+=32){
//         ucVar=rand();
//         xRam2_memset(i,ucVar,32);
//         for(j=0;j<32;j++){
//             if(xRam2_uCharRead(i+j)!=ucVar){
//                 errCount++;
//             }
//         }
//     }
//     printf("%5u] ",errCount);

//     printf("%u\n",(unsigned int)(clock_get()-time));
// }
