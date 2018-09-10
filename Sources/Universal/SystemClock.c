#include"Sources/Universal/SystemClock.h"

static unsigned long int data
    sysClock=0;
static unsigned int data
    sysClockMis=0;
static unsigned char data
    sysClockSec=0,
    sysClockMin=0,
    sysClockHur=0,
    sysClockDay=0;

void systemClock_tick(){
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

unsigned long int systemClock_get(){
    return sysClock;
}

unsigned int systemClock_mSecGet(){
    return sysClockMis;
}

unsigned char systemClock_secGet(){
    return sysClockSec;
}

unsigned char systemClock_minGet(){
    return sysClockMin;
}

unsigned char systemClock_hurGet(){
    return sysClockHur;
}

unsigned char systemClock_dayGet(){
    return sysClockDay;
}
