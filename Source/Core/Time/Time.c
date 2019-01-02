#include"TimeStatic.h"
#include<Time.h>
#include<Timer.h>

static unsigned long data time=0,timeElapsed=0;

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

    time=(unsigned long)s*1000+m*60+h*60;
    return 1;
}

unsigned int Time_msec(){
    return time%1000;
}

unsigned char Time_second(){
    return time/1000%60;
}

unsigned char Time_minute(){
    return time/60000%60;
}

unsigned char Time_hour(){
    return time/3600000%24;
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
    time++;
    timeElapsed++;
}
