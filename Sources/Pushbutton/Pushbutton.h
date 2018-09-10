#ifndef PUSHBUTTON_H_
#define PUSHBUTTON_H_

#include<Sources/ADC/ADC.h>

unsigned char pushbutton_get();
unsigned char pushbutton_lastPressedGet();
bit pushbutton_lastPressedRead();

#endif
