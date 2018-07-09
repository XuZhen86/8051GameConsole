#include"Joystick.h"

static unsigned char code analogPins[2]={6,7};
static bit currentPin=0;   // 0=x, 1=y

void joystickInitialize(){
    adcInitialize();
    adcInputPinSet(analogPins[currentPin]);
    // timer3Initialize(1,0x3c,0xb0);  // 20ms @ 30MHz
    // timer3Start();
}

void joystickAdcToggle(){
    // currentPin=!currentPin;
    adcInputPinSet(analogPins[currentPin=!currentPin]);
}

unsigned int joystickGetX(){
    return adcResultGet(analogPins[0]);
}

unsigned int joystickGetY(){
    return adcResultGet(analogPins[1]);
}
