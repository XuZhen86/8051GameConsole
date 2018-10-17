#include"Sources/Timer/Timer4.h"

#include"Sources/Universal/SystemClock.h"

static unsigned int data delta=0;

static unsigned long int data clock=0;
static unsigned int data millisecond=0;
static unsigned char second=0,minute=0,hour=0,day=0;

static bit timerIsRunning;
static unsigned int data timerRemains;

enum SYSTEMCLOCK_TIMER_4_CONFIG{
    X12MODE=0,
    TH=0x7e,
    TL=0x66,
    TICK_INTERVAL=1
};

void systemClock_initialize(){
    timer_4_initialize(X12MODE,1,TH,TL);
    timer_4_start();
}

void systemClock_tick(){
    delta+=TICK_INTERVAL;
}

void systemClock_flush(){
    unsigned int data deltaCopy=delta;
    delta=0;

    if(deltaCopy==0){
        return;
    }

    if(timerIsRunning){
        if(timerRemains>deltaCopy){
            timerRemains-=deltaCopy;
        }else{
            timerRemains=0;
            timerIsRunning=0;
        }
    }

    clock+=deltaCopy;

    millisecond=(deltaCopy+millisecond)%1000;
    deltaCopy/=1000;

    if(deltaCopy){
        second=(deltaCopy+second)%60;
        deltaCopy/=60;

        if(deltaCopy){
            minute=(deltaCopy+minute)%60;
            deltaCopy/=60;

            if(deltaCopy){
                hour=(deltaCopy+hour)%24;
                deltaCopy/=24;

                if(deltaCopy){
                    day+=deltaCopy;
                }
            }
        }
    }
}

unsigned long int systemClock_get(){
    systemClock_flush();
    return clock;
}

unsigned int systemClock_mSecGet(){
    systemClock_flush();
    return millisecond;
}

unsigned char systemClock_secGet(){
    systemClock_flush();
    return second;
}

unsigned char systemClock_minGet(){
    systemClock_flush();
    return minute;
}

unsigned char systemClock_hurGet(){
    systemClock_flush();
    return hour;
}

unsigned char systemClock_dayGet(){
    systemClock_flush();
    return day;
}

void systemClock_timerStart(unsigned int time){
    systemClock_flush();
    timerRemains=time;
    timerIsRunning=1;
}

void systemClock_timerPause(){
    systemClock_flush();
    timerIsRunning=0;
}

void systemClock_timerResume(){
    systemClock_flush();
    timerIsRunning=1;
}

void systemClock_timerCancel(){
    systemClock_flush();
    timerRemains=0;
    timerIsRunning=0;
}

bit systemClock_timerIsTimeUp(){
    systemClock_flush();
    return !timerRemains;
}
