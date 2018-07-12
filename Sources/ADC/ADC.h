#ifndef __ADC_H_
#define __ADC_H_

#include<Sources/Main/STC15W4K48S4.h>

void adcInitialize(unsigned char speed);
unsigned int adcGet(unsigned char channel);

#endif
