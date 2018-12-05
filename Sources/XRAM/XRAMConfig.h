#ifndef XRAMCONFIG_H_
#define XRAMCONFIG_H_

enum XRAM_COMMAND{
    READ=0x03,
    WRITE=0x02,
    EDIO=0x3b,
    EQIO=0x38,
    RSTIO=0xff,
    RDMR=0x05,
    WRMR=0x01
};

enum XRAM_SPI{
    CPOL=0,
    CPHA=0,
    CLKDIV=0
};

enum XRAM_MODE{
    BYTE_MODE=0x00,
    PAGE_MODE=0x80,
    SEQUENTIAL_MODE=0x40
};

#endif
