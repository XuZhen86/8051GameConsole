#ifndef SYSTEMCLOCK_H_
#define SYSTEMCLOCK_H_

void systemClock_tick();

unsigned long int systemClock_get();
unsigned int systemClock_mSecGet();
unsigned char systemClock_secGet();
unsigned char systemClock_minGet();
unsigned char systemClock_hurGet();
unsigned char systemClock_dayGet();

#endif
