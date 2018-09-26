#include"Sources/Main/STC15W4K48S4.h"
#include"Sources/ADC/ADC.h"
#include"Sources/Joystick/Joystick.h"

#include<math.h>

static unsigned char code analogPins[2]={6,7};  // 0=x, 1=y
static unsigned int code calibratedValues[2]={522,513};
static char code tolerance=4;

void joystick_initialize(){
}

unsigned int joystick_getX(){
    return adc_get(analogPins[0]);
}

unsigned int joystick_getY(){
    return adc_get(analogPins[0]);
}

unsigned char joystick_getDirection(){
    int x=joystick_getX(),
        y=joystick_getY();
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
