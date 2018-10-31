#ifndef XRAM2CONFIG_H_
#define XRAM2CONFIG_H_

#include"Sources/Main/STC15W4K48S4.h"

enum XRAM2_COMMAND{
    READ=0x03,
    WRITE=0x02,
    EDIO=0x3b,
    EQIO=0x38,
    RSTIO=0xff,
    RDMR=0x05,
    WRMR=0x01
};

enum XRAM2_MODE{
    BYTE_MODE=0x00,
    PAGE_MODE=0x80,
    SEQUENTIAL_MODE=0x40
};

sbit SI=P0^4;
sbit SCLK=P1^0;
sbit CS=P1^1;

#endif
