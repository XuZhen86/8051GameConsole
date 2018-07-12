#include"Pushbutton.h"

#include<stdio.h>

static unsigned char code stdValues[15]={
    241,226,213,201,
    189,177,166,154,
    139,123,104,85,
    49,32,16
};
static unsigned char code tolerance=4;

static unsigned char lastPressed=0;
static bit lastPressedRead=0;

unsigned char pushbuttonGet(){
    unsigned char v=adcGet(2)>>2,i;

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

unsigned char pushbuttonLastPressedGet(){
    return lastPressed;
}

bit pushbuttonLastPressedRead(){
    return lastPressedRead;
}
