#include"Joystick.h"

static unsigned char code analogPins[2]={6,7};
static unsigned int code calibratedValues[2]={522,513};

static bit currentPin=0;   // 0=x, 1=y

void joystickInitialize(){
    adcInitialize();
    adcInputPinSet(analogPins[currentPin]);
}

void joystickAdcToggle(){
    adcInputPinSet(analogPins[currentPin=!currentPin]);
}

unsigned int joystickGetX(){
    return adcResultGet(analogPins[0]);
}

unsigned int joystickGetY(){
    return adcResultGet(analogPins[1]);
}

unsigned char joystickGetDir(){
    return 0;
}
