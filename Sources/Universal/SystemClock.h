#ifndef __SYSTEMCLOCK_H_
#define __SYSTEMCLOCK_H_

void systemClockTick();

unsigned long int systemClockGet();
unsigned int systemClockGetMis();
unsigned char systemClockGetSec();
unsigned char systemClockGetMin();
unsigned char systemClockGetHur();
unsigned char systemClockGetDay();

#endif
