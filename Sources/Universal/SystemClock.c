#include"SystemClock.h"

static unsigned long int data sysClock=0;

void systemClockTick(){
    sysClock++;
}

unsigned long int systemClockGet(){
    return sysClock;
}

// unsigned long int systemClockGetSec(){
//     return sysClockSec;
// }

// unsigned long int systemClockGetMin(){
//     return sysClockMin;
// }

// unsigned long int systemClockGetHur(){
//     return sysClockHur;
// }

// unsigned long int systemClockGetDay(){
//     return sysClockDay;
// }
