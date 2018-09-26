#ifndef TIMER4_H_
#define TIMER4_H_

void timer_4_initialize(bit x12Mode,bit enableInterrupt,unsigned char th,unsigned char tl);
void timer_4_start();
void timer_4_stop();

static void interruptRoutine();

#endif
