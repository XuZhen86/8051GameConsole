#ifndef TIMER_H_
#define TIMER_H_

#include"Sources/Timer/Timer.h"

void timer0_setup(unsigned char imm8Th,unsigned char imm8Tl,unsigned char mode,bit x12,bit enIrq,bit prIrq);
void timer0_start();
void timer0_stop();

void timer1_setup(unsigned char imm8Th,unsigned char imm8Tl,unsigned char mode,bit x12,bit enIrq,bit prIrq);
void timer1_start();
void timer1_stop();

#endif
