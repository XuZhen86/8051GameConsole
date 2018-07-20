#ifndef __INTERRUPTROUTINE_H_
#define __INTERRUPTROUTINE_H_

#include<Sources/Main/STC15W4K48S4.h>

sbit interruptBusyLed=P2^4;

void interruptInitialize();

#endif
