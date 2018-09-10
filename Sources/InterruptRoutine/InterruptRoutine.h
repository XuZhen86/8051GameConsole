#ifndef INTERRUPTROUTINE_H_
#define INTERRUPTROUTINE_H_

#include<Sources/Main/STC15W4K48S4.h>

sbit interrupt_busyLed=P2^4;

void interrupt_initialize();

#endif
