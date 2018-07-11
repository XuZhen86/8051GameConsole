#include"Joystick.h"

static unsigned char code analogPins[2]={6,7};
static unsigned int code calibratedValues[2]={522,513};
static char code tolerance=10;

static bit currentPin=0;   // 0=x, 1=y

void joystickInitialize(){
    adcInitialize();
    adcInputPinSet(analogPins[currentPin]);
}

void joystickAdcToggle(){
    adcInputPinSet(analogPins[currentPin=!currentPin]);
}

unsigned int joystickGetX(){
    return adcResultGet(analogPins[0])-calibratedValues[0];
}

unsigned int joystickGetY(){
    return adcResultGet(analogPins[1])-calibratedValues[1];
}

unsigned char joystickGetDirection(){
    int x=joystickGetX(),
        y=joystickGetY();
    char c=((x>0)*0x04)|((y>0)*0x02)|((abs(x)>abs(y)*0x01));

    if(-tolerance<x&&x<tolerance&&-tolerance<y&&y<tolerance){
        return 0;
    }

    if(c==6||c==2){
        return 1;
    }else if(c==3||c==1){
        return 2;
    }else if(c==0||c==4){
        return 3;
    }else if(c==5||c==7){
        return 4;
    }else{
        return 5;
    }
}
