#ifndef __JOYSTICK_H_
#define __JOYSTICK_H_

#include<Main/STC15W4K48S4.h>
#include<ADC/ADC.h>
#include<Timer/Timer.h>
#include<Main/InterruptRoutine.h>

unsigned char code xAnalogPin=6,yAnalogPin=7;
bit currentPin=0;   // 0=x, 1=y

void joystickInitialize();
void joystickAdcToggle();
unsigned int joystickGetX();
unsigned int joystickGetY();

void joystickInitialize(){
    adcInitialize();
    adcInputPinSet(xAnalogPin);
    timer3Initialize(1,0x3c,0xb0);  // 20ms @ 30MHz
    timer3Start();
}

void joystickAdcToggle(){
    if(currentPin){
        adcInputPinSet(xAnalogPin);
    }else{
        adcInputPinSet(yAnalogPin);
    }
    currentPin=!currentPin;
}

unsigned int joystickGetX(){
    return adcResults[xAnalogPin];
}

unsigned int joystickGetY(){
    return adcResults[yAnalogPin];
}

#endif
