#include"Sources/Main/STC15W4K48S4.h"
#include"Sources/XRAM/XRAM.h"

#include"Sources/IAP/IAP.h"

enum IAP_RAM_CONFIG{
    BUFFER_ADDR=0xdc00,
    BUFFER_SIZE=512,
};

enum IAP_SECTOR_CONFIG{
    SECTOR_MAX=20,
    SECTOR_ERASE_WAIT=0x0
};

enum IAP_COMMAND{
    STANDBY=0x0,
    BYTE_READ=0x1,
    BYTE_WRITE=0x2,
    SECTOR_ERASE=0x3,
    TRIGGER_0=0x5a,
    TRIGGER_1=0xa5
};

enum IAP_MOVC{
    MOVC_ADDR=0xc000
};

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
            xRam_uCharSeqWrite(buffer,BUFFER_ADDR+i&0xffe0,32);
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
            xRam_uCharSeqRead(buffer,BUFFER_ADDR+i,32);
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
