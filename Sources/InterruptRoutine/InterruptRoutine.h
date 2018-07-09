#ifndef __INTERRUPTROUTINE_H_
#define __INTERRUPTROUTINE_H_

#include<Sources/Main/STC15W4K48S4.h>
#include<Sources/Universal/Universal.h>
#include<Sources/LCD12864/LCD12864.h>
#include<Sources/SPI/SPI.h>
#include<Sources/PWM/PWM.h>
#include<Sources/ADC/ADC.h>
#include<Sources/Joystick/Joystick.h>

#include<stdio.h>
#include<stdlib.h>

void interruptInitialize();
void adcInterruptRoutine();
// void timer3InterruptRoutine();
void timer4InterruptRoutine();
void pwmInterruptRoutine();


#endif
