#ifndef __TIMER_H_
#define __TIMER_H_

#include<Sources/Main/STC15W4K48S4.h>
#include<Sources/LCD12864/LCD12864.h>

// void timer3Initialize(bit x12Mode,unsigned char t3h,unsigned char t3l);
// void timer3Start();
// void timer3Stop();
void timer4Initialize(bit x12Mode,unsigned char t4h,unsigned char t4l);
void timer4Start();
// void timer4Stop();

#endif
