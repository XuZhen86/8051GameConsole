#include"InterruptRoutine.h"

sbit interruptBusyLed=P2^1;

void interruptInitialize(){
    EA=1;
    interruptBusyLed=0;
    printf("{[interruptInitialize] end}\n");
}

void adcInterruptRoutine() interrupt 5{
    interruptBusyLed=1;

    adcResultProcess();
    joystickAdcToggle();
    adcInterruptCleanup();

    interruptBusyLed=0;
}

// unsigned char timer3AdditionalCounter=0;
// void timer3InterruptRoutine() interrupt 19{
//     interruptBusyLed=1;

//     timer3AdditionalCounter++;
//     if(timer3AdditionalCounter==50){
//         timer3AdditionalCounter=0;
//         adcStart();
//     }

//     interruptBusyLed=0;
// }

static unsigned char code TIMER4_LCD12864_COUNTER_MAX=50;
static unsigned char timer4Lcd12864Counter=1,timer4Lcd12864ForceFlushCounter=1,timer4AdcCounter=0;
void timer4InterruptRoutine() interrupt 20{
    interruptBusyLed=1;

    timer4Lcd12864Counter--;
    timer4Lcd12864ForceFlushCounter--;
    if(!timer4Lcd12864Counter&&!spiGetIsOccupied()){
        timer4Lcd12864Counter=TIMER4_LCD12864_COUNTER_MAX;
        lcd12864GdramFlush(!timer4Lcd12864ForceFlushCounter);

        if(!timer4Lcd12864ForceFlushCounter){
            puts("    forceflush\n");
        }
    }else if(!timer4Lcd12864Counter){
        timer4Lcd12864Counter=rand()%8+1;
    }

    timer4AdcCounter++;
    if(timer4AdcCounter==75){
        timer4AdcCounter=0;
        adcStart();
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
