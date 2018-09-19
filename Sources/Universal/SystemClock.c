#include"Sources/Universal/SystemClock.h"

static unsigned int data delta=0;

static unsigned long int clock=0;
static unsigned int millisecond=0;
static unsigned char second=0,minute=0,hour=0,day=0;

static bit timerIsRunning;
static unsigned int timerRemains;

void systemClock_tick(){
    delta+=10;
}

void systemClock_flush(){
    unsigned int data deltaCopy=delta;
    delta=0;

    if(timerIsRunning){
        if(timerRemains>deltaCopy){
            timerRemains-=deltaCopy;
        }else{
            timerRemains=0;
            timerIsRunning=0;
        }
    }

    clock+=deltaCopy;

    deltaCopy+=millisecond;
    millisecond=deltaCopy%1000;
    deltaCopy/=1000;

    if(deltaCopy){
        deltaCopy+=second;
        second=deltaCopy%60;
        deltaCopy/=60;

        if(deltaCopy){
            deltaCopy+=minute;
            minute=deltaCopy%60;
            deltaCopy/=60;

            if(deltaCopy){
                deltaCopy+=hour;
                hour=deltaCopy%24;
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
    puts("systemClock_timerStart()");
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
