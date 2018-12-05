#include"../Main/STC15W4K48S4.h"
#include"./PWM.h"

void pwm_initialize(unsigned char clkDiv){
    P_SW2|=0x80;
    PWMC=0x7fff;
    PWMCKS=0x0f&clkDiv;
    PWMFDCR=0x00;
    PWMCR|=0x80;
}

void pwm3_initialize(unsigned int imm16T1,unsigned int imm16T2,bit initVolt,bit portSelect){
    PWMCFG|=initVolt*0x02;
    PWMCR|=0x02;
    PWM3CR=portSelect*0x08|0x04;
    pwm3_timerValueSet(imm16T1,imm16T2);
}

void pwm3_timerValueSet(unsigned int imm16T1,unsigned int imm16T2){
    PWMCR&=~0x80;
    PWM3T1=imm16T1;
    PWM3T2=imm16T2;
    PWMCR|=0x80;
}

void pwm4_initialize(unsigned int imm16T1,unsigned int imm16T2,bit initVolt,bit portSelect){
    PWMCFG|=initVolt*0x04;
    PWMCR|=0x04;
    PWM4CR=portSelect*0x08|0x04;
    pwm4_timerValueSet(imm16T1,imm16T2);
}

void pwm4_timerValueSet(unsigned int imm16T1,unsigned int imm16T2){
    PWMCR&=~0x80;
    PWM4T1=imm16T1;
    PWM4T2=imm16T2;
    PWMCR|=0x80;
}

void pwm5_initialize(unsigned int imm16T1,unsigned int imm16T2,bit initVolt,bit portSelect){
    PWMCFG|=initVolt*0x08;
    PWMCR|=0x08;
    PWM5CR=portSelect*0x08|0x04;
    pwm5_timerValueSet(imm16T1,imm16T2);
}

void pwm5_timerValueSet(unsigned int imm16T1,unsigned int imm16T2){
    PWMCR&=~0x80;
    PWM5T1=imm16T1;
    PWM5T2=imm16T2;
    PWMCR|=0x80;
}
