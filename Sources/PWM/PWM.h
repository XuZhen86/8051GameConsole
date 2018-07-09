#ifndef __PWM_h_
#define __PWM_h_

#include<Sources/Main/STC15W4K48S4.h>

void pwnInitialize(bit clkSrc,unsigned char clkDiv);
void pwmEnable();
void pwmXsfrFlush();
void pwm3Initialize(bit initVolt,bit portSelect,bit t1Intr,bit t2Intr,unsigned int t1InitV,unsigned int t2InitV);
void pwm3TimerValueSet(unsigned int t1InitV,unsigned int t2InitV);
// void pwm4Initialize(bit initVolt,bit portSelect,bit t1Intr,bit t2Intr,unsigned int t1InitV,unsigned int t2InitV);
// void pwm4TimerValueSet(unsigned int t1InitV,unsigned int t2InitV);
// void pwm5Initialize(bit initVolt,bit portSelect,bit t1Intr,bit t2Intr,unsigned int t1InitV,unsigned int t2InitV);
// void pwm5TimerValueSet(unsigned int t1InitV,unsigned int t2InitV);
void pwmStart();

#endif
