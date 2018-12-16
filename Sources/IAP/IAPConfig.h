#ifndef IAPCONFIG_H_
#define IAPCONFIG_H_

enum IAP_RAM_CONFIG{
    BUFFER_ADDR=0x2000,
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

#endif