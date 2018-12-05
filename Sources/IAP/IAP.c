#include"../Main/STC15W4K48S4.h"
#include"../XRAM/XRAM.h"
#include"./IAP.h"
#include"./IAPConfig.h"

void iap_initialize(){
    IAP_CONTR=0x80|SECTOR_ERASE_WAIT;
}

#define SECTOR_READ_BYTE_USE_MOVX
unsigned char iap_sectorReadByte(unsigned int address){
    #ifdef SECTOR_READ_BYTE_USE_MOVX
        return *(unsigned char code*)(address+MOVC_ADDR);
    #else
        IAP_CMD=BYTE_READ;
        IAP_ADDRH=address>>8;
        IAP_ADDRL=address;
        IAP_TRIG=TRIGGER_0;
        IAP_TRIG=TRIGGER_1;
        return IAP_DATA;
    #endif
}

void iap_sectorRead(unsigned char sector){
    unsigned char buffer[32];
    unsigned int data i,baseAddr;

    sector%=SECTOR_MAX;
    baseAddr=sector*512;

    for(i=0;i<512;i++){
        buffer[i%32]=iap_sectorReadByte(baseAddr+i);
        if(i%32==31){
            xRam_uCharWriteSeq(buffer,BUFFER_ADDR+i&0xffe0,32);
        }
    }
}

unsigned char iap_sectorWriteByte(unsigned int address,unsigned char c){
    IAP_DATA=c;
    IAP_CMD=BYTE_WRITE;
    IAP_ADDRH=address>>8;
    IAP_ADDRL=address;
    IAP_TRIG=TRIGGER_0;
    IAP_TRIG=TRIGGER_1;
    return c;
}

void iap_sectorWrite(unsigned char sector){
    unsigned char buffer[32];
    unsigned int data i,baseAddr;

    sector%=SECTOR_MAX;
    baseAddr=sector*512;

    iap_sectorErase(sector);
    for(i=0;i<512;i++){
        if(i%32==0){
            xRam_uCharReadSeq(buffer,BUFFER_ADDR+i,32);
        }
        iap_sectorWriteByte(baseAddr+i,buffer[i%32]);
    }
}

void iap_sectorErase(unsigned char sector){
    sector%=SECTOR_MAX;
    IAP_CMD=SECTOR_ERASE;
    IAP_ADDRH=(sector*512)>>8;
    IAP_ADDRL=(sector*512);
    IAP_TRIG=TRIGGER_0;
    IAP_TRIG=TRIGGER_1;
}

unsigned char iap_uCharGet(unsigned int offset){
    return xRam_uCharRead(BUFFER_ADDR+offset%512);
}

unsigned char iap_uCharSet(unsigned int offset,unsigned char c){
    return xRam_uCharWrite(BUFFER_ADDR+offset%512,c);
}

unsigned int iap_uIntGet(unsigned int offset){
    return xRam_uIntRead(BUFFER_ADDR+offset%512);
}

unsigned int iap_uIntSet(unsigned int offset,unsigned int i){
    return xRam_uIntWrite(BUFFER_ADDR+offset%512,i);
}
