#ifndef __JOYSTICK_H_
#define __JOYSTICK_H_

#include<Sources/Main/STC15W4K48S4.h>
#include<Sources/ADC/ADC.h>
#include<Sources/Timer/Timer.h>
#include<Sources/InterruptRoutine/InterruptRoutine.h>

void joystickInitialize();
void joystickAdcToggle();
unsigned int joystickGetX();
unsigned int joystickGetY();

#endif
