#ifndef TIMER_H_
#define TIMER_H_

enum TIMER_ATTRIBUTES{
    TIMER_MODE_0=0x00,
    TIMER_MODE_1=0x01,
    TIMER_MODE_2=0x02,
    TIMER_MODE_3=0x03,
    TIMER_X12=0x04,
    TIMER_ENIRQ=0x08,
    TIMER_PRIRQ=0x10
};

void timer0_setup(unsigned char imm8Th,unsigned char imm8Tl,unsigned char attr);
void timer0_start();
void timer0_stop();

void timer1_setup(unsigned char imm8Th,unsigned char imm8Tl,unsigned char attr);
void timer1_start();
void timer1_stop();

#endif
