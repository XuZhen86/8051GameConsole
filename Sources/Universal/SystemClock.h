#ifndef SYSTEMCLOCK_H_
#define SYSTEMCLOCK_H_

void systemClock_tick();
void systemClock_flush();

unsigned long int systemClock_get();
unsigned int systemClock_mSecGet();
unsigned char systemClock_secGet();
unsigned char systemClock_minGet();
unsigned char systemClock_hurGet();
unsigned char systemClock_dayGet();

void systemClock_timerStart(unsigned int time);
void systemClock_timerPause();
void systemClock_timerResume();
void systemClock_timerCancel();
bit systemClock_timerIsTimeUp();

#endif
