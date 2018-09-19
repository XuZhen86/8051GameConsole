#include"Sources/Pushbutton/Pushbutton.h"

#include<stdio.h>

static unsigned char code numberStdVal[15]={
    241,225,210,195,
    180,168,152,137,
    120,103,87,70,
    48,32,15
};
static unsigned char code directionStdVal[5]={
    214,172,131,87,43
};

static unsigned char code tolerance=8;

unsigned char pushbutton_numberGet(){
    unsigned char v=adc_get(6)>>2,i;

    if(v==255){
        return 255;
    }else if(v==0){
        return 15;
    }

    for(i=0;i<15;i++){
        if(numberStdVal[i]-tolerance<v&&v<numberStdVal[i]+tolerance){
            return i;
        }
    }
    return 255;
}

unsigned char pushbutton_directionGet(){
    unsigned char v=adc_get(7)>>2,i;

    if(v==255){
        return 255;
    }else if(v==0){
        return 5;
    }

    for(i=0;i<5;i++){
        if(directionStdVal[i]-tolerance<v&&v<directionStdVal[i]+tolerance){
            return i;
        }
    }
    return 255;
}

void _pushbutton_directionButtonTest(){
    unsigned char i;

    while(1){
        i=pushbutton_directionGet();
        if(i==PUSHBUTTON_DIRECTION_BACK){
            puts("Back");
        }else if(i==PUSHBUTTON_DIRECTION_DOWN){
            puts("Down");
        }else if(i==PUSHBUTTON_DIRECTION_FORWARD){
            puts("Forward");
        }else if(i==PUSHBUTTON_DIRECTION_LEFT){
            puts("Left");
        }else if(i==PUSHBUTTON_DIRECTION_RIGHT){
            puts("Right");
        }else if(i==PUSHBUTTON_DIRECTION_UP){
            puts("Up");
        }
    }
}
