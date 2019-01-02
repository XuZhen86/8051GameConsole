#include<STC15W4K48S4.h>
#include<Timer.h>

void Timer0_setCounter(unsigned int imm16){
    TH0=imm16>>8;
    TL0=imm16;
}

void Timer0_setMode(unsigned char mode){
    TMOD=TMOD&0xf0|(mode%4);
}

void Timer0_setX12Enabled(unsigned char x12){
    if(x12){
        AUXR|=0x80;
    }else{
        AUXR&=~0x80;
    }
}

void Timer0_setInterruptEnabled(unsigned char enable){
    if(enable){
        EA=1;
        ET0=1;
    }else{
        ET0=0;
    }
}

void Timer0_setPriority(unsigned char priority){
    if(priority){
        PT0=1;
    }else{
        PT0=0;
    }
}

void Timer0_start(){
    TR0=1;
}

void Timer0_stop(){
    TR0=0;
}

unsigned int Timer0_elapsed(){
    return ((unsigned int)TH0<<8)|TL0;
}

void Timer1_setCounter(unsigned int imm16){
    TH1=imm16>>8;
    TL1=imm16;
}

void Timer1_setMode(unsigned char mode){
    TMOD=TMOD&0x0f|((mode%3)<<4);
}

void Timer1_setX12Enabled(unsigned char x12){
    if(x12){
        AUXR|=0x40;
    }else{
        AUXR&=~0x40;
    }
}

void Timer1_setInterruptEnabled(unsigned char enable){
    if(enable){
        EA=1;
        ET1=1;
    }else{
        ET1=0;
    }
}

void Timer1_setPriority(unsigned char priority){
    if(priority){
        PT1=1;
    }else{
        PT1=0;
    }
}

void Timer1_start(){
    TR1=1;
}

void Timer1_stop(){
    TR1=0;
}

unsigned int Timer1_elapsed(){
    return ((unsigned int)TH1<<8)|TL1;
}
