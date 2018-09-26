#ifndef TIMER3_H_
#define TIMER3_H_

void timer_3_initialize(bit x12Mode,unsigned char th,unsigned char tl);
void timer_3_start();
void timer_3_stop();

static void interruptRoutine();

#endif
