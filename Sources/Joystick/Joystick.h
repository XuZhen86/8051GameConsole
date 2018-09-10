#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include<Sources/Main/STC15W4K48S4.h>
#include<Sources/ADC/ADC.h>

#include<math.h>

void joystick_initialize();
unsigned int joystick_getX();
unsigned int joystick_getY();
unsigned char joystick_getDirection();

#endif
