#ifndef TIMER_H_
#define TIMER_H_

// Timer 0 control
void Timer0_setCounter(unsigned int imm16);
void Timer0_setMode(unsigned char mode);
void Timer0_setX12Enabled(unsigned char x12);
void Timer0_setInterruptEnabled(unsigned char enable);
void Timer0_setPriority(unsigned char priority);
void Timer0_start();
void Timer0_stop();
unsigned int Timer0_elapsed();

// Timer1 control
void Timer1_setCounter(unsigned int imm16);
void Timer1_setMode(unsigned char mode);
void Timer1_setX12Enabled(unsigned char x12);
void Timer1_setInterruptEnabled(unsigned char enable);
void Timer1_setPriority(unsigned char priority);
void Timer1_start();
void Timer1_stop();
unsigned int Timer1_elapsed();

// Timer 2 control
void Timer2_setCounter(unsigned int imm16);
void Timer2_setX12Enabled(unsigned char x12);
void Timer2_setInterruptEnabled(unsigned char enable);
void Timer2_start();
void Timer2_stop();
unsigned int Timer2_elapsed();

#endif
