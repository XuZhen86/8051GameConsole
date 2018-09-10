#ifndef TIMER4_H_
#define TIMER4_H_

#include<Sources/Main/STC15W4K48S4.h>
#include<Sources/InterruptRoutine/InterruptRoutine.h>
#include<Sources/Universal/SystemClock.h>

void timer_4_initialize(bit x12Mode,bit enableInterrupt,unsigned char th,unsigned char tl);
void timer_4_start();
void timer_4_stop();

static void interruptRoutine();

#endif
