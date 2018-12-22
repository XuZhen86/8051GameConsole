#include"TimeStatic.h"
#include<Time.h>
#include<Timer.h>

static unsigned long data timeDelta=0,timeElapsed=0;
static unsigned int millisecond=0;
static unsigned char second=0,minute=0,hour=0;

void Time_init(){
    Timer0_setCounter(0x7e66);
    Timer0_setX12Enabled(1);
    Timer0_setInterruptEnabled(1);
    Timer0_start();
}

bit Time_setHMS(unsigned char h,unsigned char m,unsigned char s){
    if(h>=24||m>=60||s>=60){
        return 0;
    }

    flush();
    hour=h;
    minute=m;
    second=s;
    return 1;
}

unsigned int Time_msec(){
    flush();
    return millisecond;
}

unsigned char Time_second(){
    flush();
    return second;
}

unsigned char Time_minute(){
    flush();
    return minute;
}

unsigned char Time_hour(){
    flush();
    return hour;
}

unsigned long Time_restart(){
    unsigned long tElapsed=timeElapsed;
    timeElapsed=0;
    return tElapsed;
}

void Time_start(){
    timeElapsed=0;
}

unsigned long Time_elapsed(){
    return timeElapsed;
}

static void tick() interrupt 1{
    timeDelta++;
    timeElapsed++;
}

static void flush(){
    unsigned long delta=timeDelta;
    timeDelta=0;

    if(delta==0){
        return;
    }

    millisecond=(delta+millisecond)%1000;
    delta/=1000;

    if(delta){
        second=(delta+second)%60;
        delta/=60;

        if(delta){
            minute=(delta+minute)%60;
            delta/=60;

            if(delta){
                hour=(delta+hour)%24;
            }
        }
    }
}
