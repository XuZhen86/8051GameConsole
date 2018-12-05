#include"../ADC/ADC.h"
#include"../Universal/Universal.h"
#include"../Delay/Delay.h"
#include"./Pushbutton.h"

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
    TRIAL_COUNT=8,
    TOLERANCE_NUMBER=4,
    TOLERANCE_DIRECTION=4
};

static unsigned char
    lastPressedNumber=PUSHBUTTON_DIRECTION_INVALID,
    lastPressedDirection=PUSHBUTTON_DIRECTION_INVALID;

unsigned char pushbutton_numberGet(){
    unsigned char buffer[TRIAL_COUNT];
    unsigned char i=0,j,diff;
    unsigned char testPassed=0;

    for(j=0;j<TRIAL_COUNT;j++){
        buffer[j]=adc_get(6)>>2;
    }

    while(!testPassed){
        buffer[i++]=adc_get(6)>>2;
        i%=TRIAL_COUNT;

        testPassed=1;
        for(j=1;j<TRIAL_COUNT;j++){
            if(buffer[0]>buffer[j]){
                diff=buffer[0]-buffer[j];
            }else{
                diff=buffer[j]-buffer[0];
            }

            if(diff>=TOLERANCE_NUMBER){
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
            return lastPressedNumber=i;
        }
    }

    return PUSHBUTTON_NUMBER_INVALID;
}

unsigned char pushbutton_directionGet(){
    unsigned char buffer[TRIAL_COUNT];
    unsigned char i=0,j,diff;
    unsigned char testPassed=0;

    for(j=0;j<TRIAL_COUNT;j++){
        buffer[j]=adc_get(7)>>2;
    }

    while(!testPassed){
        buffer[i++]=adc_get(7)>>2;
        i%=TRIAL_COUNT;

        testPassed=1;
        for(j=1;j<TRIAL_COUNT;j++){
            if(buffer[0]>buffer[j]){
                diff=buffer[0]-buffer[j];
            }else{
                diff=buffer[j]-buffer[0];
            }

            if(diff>=TOLERANCE_DIRECTION){
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
            return lastPressedDirection=i;
        }
    }

    return PUSHBUTTON_DIRECTION_INVALID;
}

unsigned char pushbutton_waitNumberGet(){
    while(pushbutton_numberGet()==PUSHBUTTON_NUMBER_INVALID){
        delay(50);
    }
    return lastPressedNumber;
}

unsigned char pushbutton_waitDirectionGet(){
    while(pushbutton_directionGet()==PUSHBUTTON_DIRECTION_INVALID){
        delay(50);
    }
    return lastPressedDirection;
}

void pushbutton_waitNumberRelease(){
    while(pushbutton_numberGet()!=PUSHBUTTON_NUMBER_INVALID){
        delay(50);
    }
}
void pushbutton_waitDirectionRelease(){
    while(pushbutton_directionGet()!=PUSHBUTTON_DIRECTION_INVALID){
        delay(50);
    }
}

unsigned char pushbutton_lastPressedNumberGet(){
    return lastPressedNumber;
}

unsigned char pushbutton_lastPressedDirectionGet(){
    return lastPressedDirection;
}

// void _pushbutton_directionButtonTest(){
//     unsigned char i;
//     while(1){
//         i=pushbutton_directionGet();
//         if(i==PUSHBUTTON_DIRECTION_BACK){
//             puts("Back");
//         }else if(i==PUSHBUTTON_DIRECTION_DOWN){
//             puts("Down");
//         }else if(i==PUSHBUTTON_DIRECTION_FORWARD){
//             puts("Forward");
//         }else if(i==PUSHBUTTON_DIRECTION_LEFT){
//             puts("Left");
//         }else if(i==PUSHBUTTON_DIRECTION_RIGHT){
//             puts("Right");
//         }else if(i==PUSHBUTTON_DIRECTION_UP){
//             puts("Up");
//         }else{
//             // puts("Invalid");
//         }
//     }
// }
