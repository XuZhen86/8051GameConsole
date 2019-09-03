#ifndef CLOCK_H_
#define CLOCK_H_

// Init system time
void Time_init();

// Set uptime
bit Time_setHMS(unsigned char h,unsigned char m,unsigned char s);

// Get time
unsigned int Time_msec();
unsigned char Time_second();
unsigned char Time_minute();
unsigned char Time_hour();

// Restart timer, return elapsed
unsigned long Time_restart();

// Start timer
void Time_start();

// Get elapsed time
unsigned long Time_elapsed();

#endif
