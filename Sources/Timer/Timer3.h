#ifndef __TIMER3_H_
#define __TIMER3_H_

#include<Sources/Main/STC15W4K48S4.h>
#include<Sources/LCD12864/LCD12864.h>

void timer3Initialize(bit x12Mode,unsigned char th,unsigned char tl);
void timer3Start();
void timer3Stop();

static void interruptRoutine();

#endif
