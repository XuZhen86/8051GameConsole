#include"Sources/Pushbutton/Pushbutton.h"

#include<stdio.h>

static unsigned char code numberStdVal[16]={
    241,225,210,195,
    180,168,152,137,
    120,103,87,70,
    48,32,15,0
};
static unsigned char code directionStdVal[6]={
    214,172,131,87,43,0
};

enum{
    TRIAL_COUNT=16,
    TOLERANCE_NUMBER=4,
    TOLERANCE_DIRECTION=4
};

unsigned char pushbutton_numberGet(){
    unsigned char buffer[16];
    unsigned char data i=0,j,diff;
    bit testPassed=0;

    for(j=0;j<16;j++){
        buffer[j]=0xff;
    }

    while(!testPassed){
        buffer[i++]=adc_get(6)>>2;
        i%=16;

        testPassed=1;
        for(j=i+1;j<i+16;j++){
            if(buffer[i]>buffer[j%16]){
                diff=buffer[i]-buffer[j%16];
            }else{
                diff=buffer[j%16]-buffer[i];
            }

            if(diff>TOLERANCE_NUMBER){
                testPassed=0;
                break;
            }
        }
    }

    for(i=0;i<16;i++){
        if(buffer[0]>numberStdVal[i]){
            diff=buffer[0]-numberStdVal[i];
        }else{
            diff=numberStdVal[i]-buffer[0];
        }

        if(diff<TOLERANCE_NUMBER){
            return i;
        }
    }

    return 255;
}

unsigned char pushbutton_directionGet(){
    unsigned char buffer[16];
    unsigned char data i=0,j,diff;
    bit testPassed=0;

    for(j=0;j<16;j++){
        buffer[j]=0xff;
    }

    while(!testPassed){
        buffer[i++]=adc_get(7)>>2;
        i%=16;

        testPassed=1;
        for(j=i+1;j<i+16;j++){
            if(buffer[i]>buffer[j%16]){
                diff=buffer[i]-buffer[j%16];
            }else{
                diff=buffer[j%16]-buffer[i];
            }

            if(diff>TOLERANCE_DIRECTION){
                testPassed=0;
                break;
            }
        }
    }

    for(i=0;i<6;i++){
        if(buffer[0]>directionStdVal[i]){
            diff=buffer[0]-directionStdVal[i];
        }else{
            diff=directionStdVal[i]-buffer[0];
        }

        if(diff<TOLERANCE_DIRECTION){
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
