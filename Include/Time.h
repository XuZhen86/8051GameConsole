#ifndef CLOCK_H_
#define CLOCK_H_

void Time_init();

bit Time_setHMS(unsigned char h,unsigned char m,unsigned char s);
unsigned int Time_msec();
unsigned char Time_second();
unsigned char Time_minute();
unsigned char Time_hour();

unsigned long Time_restart();
void Time_start();
unsigned long Time_elapsed();

#endif
