#ifndef TIMER3_H_
#define TIMER3_H_

#include<Sources/Main/STC15W4K48S4.h>
#include<Sources/LCD12864/LCD12864.h>

void timer_3_initialize(bit x12Mode,unsigned char th,unsigned char tl);
void timer_3_start();
void timer_3_stop();

static void interruptRoutine();

#endif
