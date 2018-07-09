#ifndef __ADC_H_
#define __ADC_H_

#include<Sources/Main/STC15W4K48S4.h>

void adcInitialize();
void adcInputPinSet(unsigned char pin);
void adcInterruptCleanup();
void adcResultProcess();
void adcStart();

unsigned int adcResultGet(unsigned char i);

#endif
