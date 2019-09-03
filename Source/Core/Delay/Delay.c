#include"Delay.h"
#include<Power.h>
#include<Timer.h>

void delay(unsigned int ms){
    /*
        0     1     2     3     4     5ms
        |-----|-----|-----|-----|-----|
           |--|-----|-----|+++|        delay(3)

        -Idle sleep
        +Busy sleep
    */

    // The first ms will be incomplete
    // Record time offset and will delay after
    unsigned char t=(unsigned char)Timer0_elapsed();

    while(ms--){
        // Delay by enter CPU idle state
        Power_idle();
    }

    // Offset the first ms
    while((unsigned char)Timer0_elapsed()<t){}
}

void delayLoop(unsigned char i,unsigned char j,unsigned char k){
    do{
        do{
            while(k--){}
        }while(j--);
    }while(i--);
}
