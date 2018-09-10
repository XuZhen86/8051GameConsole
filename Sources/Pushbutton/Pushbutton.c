#include"Sources/Pushbutton/Pushbutton.h"

#include<stdio.h>

static unsigned char code stdValues[15]={
    241,225,210,195,
    180,168,152,137,
    120,103,87,70,
    48,32,15
};
static unsigned char code tolerance=8;

static unsigned char lastPressed=0;
static bit lastPressedRead=0;

unsigned char pushbutton_get(){
    unsigned char v=adc_get(6)>>2,i;

    printf("[%u]\n",(unsigned int)v);

    if(v==255){
        return -1;
    }else if(v==0){
        lastPressedRead=0;
        return lastPressed=15;
    }

    for(i=0;i<15;i++){
        if(stdValues[i]-tolerance<v&&v<stdValues[i]+tolerance){
            lastPressedRead=0;
            return lastPressed=i;
        }
    }
    return -1;
}

unsigned char pushbutton_lastPressedGet(){
    return lastPressed;
}

bit pushbutton_lastPressedRead(){
    return lastPressedRead;
}
