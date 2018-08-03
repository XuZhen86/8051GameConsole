#include"SystemClock.h"

static unsigned long int data
    sysClock=0;
static unsigned int data
    sysClockMis=0;
static unsigned char data
    sysClockSec=0,
    sysClockMin=0,
    sysClockHur=0,
    sysClockDay=0;

void systemClockTick(){
    sysClock++;
    sysClockMis++;

    if(sysClockMis%1000==0){
        sysClockMis=0;
        sysClockSec++;

        if(sysClockSec==60){
            sysClockSec=0;
            sysClockMin++;

            if(sysClockMin==60){
                sysClockMin=0;
                sysClockHur++;

                if(sysClockHur==24){
                    sysClockHur=0;
                    sysClockDay++;
                }
            }
        }
    }
}

unsigned long int systemClockGet(){
    return sysClock;
}

unsigned int systemClockGetMis(){
    return sysClockMis;
}

unsigned char systemClockGetSec(){
    return sysClockSec;
}

unsigned char systemClockGetMin(){
    return sysClockMin;
}

unsigned char systemClockGetHur(){
    return sysClockHur;
}

unsigned char systemClockGetDay(){
    return sysClockDay;
}
