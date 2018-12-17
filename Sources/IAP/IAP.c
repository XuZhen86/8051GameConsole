#include<string.h>
#include<stdio.h>
#include"../Main/STC15W4K48S4.h"
#include"../FarMem/FarMem.h"
#include"./IAP.h"
#include"./IAPConfig.h"

static unsigned char sectorBuffer[512],currentSector;

void Iap_initialize(){
    IAP_CONTR=0x80|SECTOR_ERASE_WAIT;
    currentSector=0;
    loadSector(0);
}

unsigned char Iap_read(unsigned int addr16) small{
    addr16%=ADDR_MAX;

    if(addr16/512!=currentSector){
        return *(unsigned char code*)(addr16+MOVC_ADDR_OFFS);
    }else{
        return sectorBuffer[addr16%512];
    }
}

unsigned char Iap_write(unsigned int addr16,unsigned char imm8) small{
    addr16%=ADDR_MAX;

    if(addr16/512!=currentSector){
        unloadSector(currentSector);
        currentSector=addr16/512;
        loadSector(currentSector);
    }
    sectorBuffer[addr16%512]=imm8;
    return imm8;
}

void Iap_flush(){
    eraseSector(currentSector);
    writeSector(currentSector);
}

static void loadSector(unsigned char sector) small{
    sector%=SECTOR_MAX;
    memcpy(sectorBuffer,(unsigned char code*)(sector*512+MOVC_ADDR_OFFS),512);
}

static void unloadSector(unsigned char sector) small{
    sector%=SECTOR_MAX;

    eraseSector(sector);
    writeSector(sector);
}

static void eraseSector(unsigned char sector) small{
    sector%=SECTOR_MAX;

    IAP_CMD=SECTOR_ERASE;
    IAP_ADDRH=(sector*512)>>8;
    IAP_ADDRL=(sector*512);
    IAP_TRIG=TRIGGER_0;
    IAP_TRIG=TRIGGER_1;
}

static void writeSector(unsigned char sector) small{
    unsigned int i,addr=sector%SECTOR_MAX*512;

    for(i=0;i<512;i++,addr++){
        IAP_DATA=sectorBuffer[i];
        IAP_CMD=BYTE_WRITE;
        IAP_ADDRH=addr>>8;
        IAP_ADDRL=addr;
        IAP_TRIG=TRIGGER_0;
        IAP_TRIG=TRIGGER_1;
    }
}

void Iap_test(){
    unsigned int i;

    for(i=0;i<16;i++){
        Iap_write(i,i);
        Iap_write(i+1024,i);
    }

    for(i=0;i<16;i++){
        printf("iap[%x]=%bx\n",i,Iap_read(i));
    }
    for(i=0;i<16;i++){
        printf("iap[%x]=%bx\n",i+1024,Iap_read(i+1024));
    }
}

void Iap_dump(unsigned int startAddr,unsigned int endAddr){
    unsigned char c;

    printf("Iap_dump(0x%x,0x%x)\n",startAddr,endAddr);
    for(;startAddr<endAddr;startAddr++){
        c=Iap_read(startAddr);
        printf("iap[0x%04x]=0x%02bx ",startAddr,c);

        if(' '<=c&&c<='~'){
            printf("[%c]\n",c);
        }else{
            printf("      [%bd]\n",c);
        }
    }
}
