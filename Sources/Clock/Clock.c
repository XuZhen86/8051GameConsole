#include"../Timer/Timer.h"
#include"./Clock.h"
#include"./ClockConfig.h"

static unsigned int data delta=0;

static unsigned long int data clock=0;
static unsigned int millisecond=0;
static unsigned char second=0,minute=0,hour=0,day=0;

static bit timerIsRunning;
static unsigned int data timerRemains;

void clock_initialize(){
    timer0_setup(TH,TL,TIMER_MODE_0|(X12?TIMER_X12:0)|TIMER_ENIRQ|TIMER_PRIRQ);
    timer0_start();
}

void clock_tick() interrupt 1{
    delta+=TICK_INTERVAL;
}

void clock_flush() small{
    unsigned int deltaCopy=delta;
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

unsigned long int clock_get(){
    clock_flush();
    return clock;
}

unsigned int clock_mSecGet(){
    clock_flush();
    return millisecond;
}

unsigned char clock_secGet(){
    clock_flush();
    return second;
}

unsigned char clock_minGet(){
    clock_flush();
    return minute;
}

unsigned char clock_hurGet(){
    clock_flush();
    return hour;
}

unsigned char clock_dayGet(){
    clock_flush();
    return day;
}

void clock_timerStart(unsigned int time){
    clock_flush();
    timerRemains=time;
    timerIsRunning=1;
}

void clock_timerPause(){
    clock_flush();
    timerIsRunning=0;
}

void clock_timerResume(){
    clock_flush();
    timerIsRunning=1;
}

void clock_timerCancel(){
    clock_flush();
    timerRemains=0;
    timerIsRunning=0;
}

unsigned char clock_timerIsTimeUp(){
    clock_flush();
    return timerRemains==0;
}
