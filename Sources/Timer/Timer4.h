#ifndef __TIMER4_H_
#define __TIMER4_H_

#include<Sources/Main/STC15W4K48S4.h>
#include<Sources/InterruptRoutine/InterruptRoutine.h>
#include<Sources/LCD12864/LCD12864.h>
#include<Sources/ADC/ADC.h>

void timer4Initialize(bit x12Mode,bit enableInterrupt,unsigned char th,unsigned char tl);
void timer4Start();
void timer4Stop();

static void interruptRoutine();

#endif
