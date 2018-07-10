#ifndef __ADC_H_
#define __ADC_H_

#include<Sources/Main/STC15W4K48S4.h>
#include<Sources/InterruptRoutine/InterruptRoutine.h>
#include<Sources/Joystick/Joystick.h>

void adcInitialize();
void adcInputPinSet(unsigned char pin);
void adcInterruptCleanup();
void adcResultProcess();
void adcStart();
unsigned int adcResultGet(unsigned char i);

static void interruptRoutine();

#endif
