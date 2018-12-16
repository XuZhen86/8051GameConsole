#include"../Main/STC15W4K48S4.h"
#include"./Timer.h"

void timer0_setup(unsigned char imm8Th,unsigned char imm8Tl,unsigned char attr){
    TMOD=TMOD&~0x03|attr&0x03;
    TH0=imm8Th;
    TL0=imm8Tl;
    ET0=attr&TIMER_ENIRQ;
    PT0=attr&TIMER_ENIRQ;
    if(attr&TIMER_X12){AUXR|=0x80;}
    else{AUXR&=~0x80;}
}

void timer0_start(){
    TR0=1;
}

void timer0_stop(){
    TR0=0;
}

void timer1_setup(unsigned char imm8Th,unsigned char imm8Tl,unsigned char attr){
    TMOD=TMOD&~0x30|(attr&0x03)<<4;
    TH1=imm8Th;
    TL1=imm8Tl;
    ET1=attr&TIMER_ENIRQ;
    PT1=attr&TIMER_ENIRQ;
    if(attr&TIMER_X12){AUXR|=0x40;}
    else{AUXR&=~0x40;}
}

void timer1_start(){
    TR1=1;
}

void timer1_stop(){
    TR1=0;
}
