#ifndef CLOCK_H_
#define CLOCK_H_

void clock_initialize();
void clock_tick();
void clock_flush() small;

unsigned long int clock_get();
unsigned int clock_mSecGet();
unsigned char clock_secGet();
unsigned char clock_minGet();
unsigned char clock_hurGet();
unsigned char clock_dayGet();

void clock_timerStart(unsigned int time);
void clock_timerPause();
void clock_timerResume();
void clock_timerCancel();
unsigned char clock_timerIsTimeUp();

#endif
