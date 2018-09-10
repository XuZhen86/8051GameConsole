#ifndef ADC_H_
#define ADC_H_

#include<Sources/Main/STC15W4K48S4.h>

void adc_initialize(unsigned char speed);
unsigned int adc_get(unsigned char channel);

#endif
