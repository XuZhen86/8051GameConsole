#ifndef __INTERRUPTROUTINE_H_
#define __INTERRUPTROUTINE_H_

#include<Main/STC15W4K48S4.h>
#include<Main/Universal.h>
#include<LCD12864/LCD12864.h>
#include<SPI/SPI.h>
#include<PWM/PWM.h>
#include<ADC/ADC.h>
#include<Joystick/Joystick.h>

sbit interruptBusyLed=P2^1;

void interruptInitialize();
void adcInterruptRoutine();
void timer3InterruptRoutine();
void timer4InterruptRoutine();
void pwmInterruptRoutine();

void interruptInitialize(){
    EA=1;
    interruptBusyLed=0;
}

void adcInterruptRoutine() interrupt 5{
    interruptBusyLed=1;

    adcResultProcess();
    joystickAdcToggle();
    adcInterruptCleanup();

    interruptBusyLed=0;
}

unsigned char timer3AdditionalCounter=0;
void timer3InterruptRoutine() interrupt 19{
    interruptBusyLed=1;

    timer3AdditionalCounter++;
    if(timer3AdditionalCounter==10){
        timer3AdditionalCounter=0;
        adcStart();
    }

    interruptBusyLed=0;
}

unsigned char timer4AdditionalCounter=0;
void timer4InterruptRoutine() interrupt 20{
    interruptBusyLed=1;

    timer4AdditionalCounter++;
    if(timer4AdditionalCounter==15&&!spiGetOccupied()){
        timer4AdditionalCounter=0;
        lcd12864GdramFlush();
    }else if(timer4AdditionalCounter==15){
        timer4AdditionalCounter=10;
    }

    interruptBusyLed=0;
}

void pwmInterruptRoutine() interrupt 22{
    interruptBusyLed=1;

    if(PWMIF&0x01){
        PWMIF&=~0x01;
    }else if(PWMIF&0x02){
        PWMIF&=~0x02;
    }else if(PWMIF&0x04){
        PWMIF&=~0x04;
    }else if(PWMIF&0x08){
        PWMIF&=~0x08;
    }else if(PWMIF&0x10){
        PWMIF&=~0x10;
    }else if(PWMIF&0x20){
        PWMIF&=~0x20;
    }else if(PWMIF&0x40){
        PWMIF&=~0x40;
    }

    interruptBusyLed=0;
}

#endif
