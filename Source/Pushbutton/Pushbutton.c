#include"PushbuttonConfig.h"
#include<Analog.h>
#include<Delay.h>
#include<Pushbutton.h>

static unsigned char code numberStdVal[16]={
    241,225,210,195,
    180,168,152,137,
    120,103,87,70,
    48,32,15,0
};
static unsigned char code directionStdVal[6]={
    214,172,131,87,43,0
};

static unsigned char
    lastNumber=PUSHBUTTON_DIRECTION_INVALID,
    lastDirection=PUSHBUTTON_DIRECTION_INVALID;

unsigned char Pushbutton_getNumber(){
    unsigned char analogVals[TRIAL_COUNT];
    unsigned char i=0,j,diff;
    unsigned char testPassed=0;

    for(j=0;j<TRIAL_COUNT;j++){
        analogVals[j]=Analog_read(6)>>2;
    }

    while(!testPassed){
        analogVals[i++]=Analog_read(6)>>2;
        i%=TRIAL_COUNT;

        testPassed=1;
        for(j=1;j<TRIAL_COUNT;j++){
            if(analogVals[0]>analogVals[j]){
                diff=analogVals[0]-analogVals[j];
            }else{
                diff=analogVals[j]-analogVals[0];
            }

            if(diff>=TOLERANCE_NUMBER){
                testPassed=0;
                break;
            }
        }
    }

    for(i=0;i<16;i++){
        if(analogVals[0]>numberStdVal[i]){
            diff=analogVals[0]-numberStdVal[i];
        }else{
            diff=numberStdVal[i]-analogVals[0];
        }

        if(diff<TOLERANCE_NUMBER){
            return lastNumber=i;
        }
    }

    return PUSHBUTTON_NUMBER_INVALID;
}

unsigned char Pushbutton_getDirection(){
    unsigned char analogVals[TRIAL_COUNT];
    unsigned char i=0,j,diff;
    unsigned char testPassed=0;

    for(j=0;j<TRIAL_COUNT;j++){
        analogVals[j]=Analog_read(7)>>2;
    }

    while(!testPassed){
        analogVals[i++]=Analog_read(7)>>2;
        i%=TRIAL_COUNT;

        testPassed=1;
        for(j=1;j<TRIAL_COUNT;j++){
            if(analogVals[0]>analogVals[j]){
                diff=analogVals[0]-analogVals[j];
            }else{
                diff=analogVals[j]-analogVals[0];
            }

            if(diff>=TOLERANCE_DIRECTION){
                testPassed=0;
                break;
            }
        }
    }

    for(i=0;i<6;i++){
        if(analogVals[0]>directionStdVal[i]){
            diff=analogVals[0]-directionStdVal[i];
        }else{
            diff=directionStdVal[i]-analogVals[0];
        }

        if(diff<TOLERANCE_DIRECTION){
            return lastDirection=i;
        }
    }

    return PUSHBUTTON_DIRECTION_INVALID;
}

unsigned char Pushbutton_getNumberWait(){
    while(Pushbutton_getNumber()==PUSHBUTTON_NUMBER_INVALID){
        delay(50);
    }
    return lastNumber;
}

unsigned char Pushbutton_getDirectionWait(){
    while(Pushbutton_getDirection()==PUSHBUTTON_DIRECTION_INVALID){
        delay(50);
    }
    return lastDirection;
}

void Pushbutton_numberReleaseWait(){
    while(Pushbutton_getNumber()!=PUSHBUTTON_NUMBER_INVALID){
        delay(50);
    }
}
void Pushbutton_directionReleaseWait(){
    while(Pushbutton_getDirection()!=PUSHBUTTON_DIRECTION_INVALID){
        delay(50);
    }
}

unsigned char Pushbutton_getLastNumber(){
    return lastNumber;
}

unsigned char Pushbutton_getLastDirection(){
    return lastDirection;
}
