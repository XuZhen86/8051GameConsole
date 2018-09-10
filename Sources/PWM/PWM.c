#include"Sources/PWM/PWM.h"
#include"Sources/PWM/PWM_XSFR.h"

void pwn_initialize(bit clkSrc,unsigned char clkDiv){
    P_SW2|=0x80;
    PWMC=0x7fff;
    PWMCKS=(0x10*clkSrc)|(0x0f&clkDiv);
    PWMFDCR=0;
}

void pwm_xsfrFlush(){
    PWMCxsfr=PWMC;
    PWM2T1xsfr=PWM2T1;
    PWM2T2xsfr=PWM2T2;
    PWM3T1xsfr=PWM3T1;
    PWM3T2xsfr=PWM3T2;
    PWM4T1xsfr=PWM4T1;
    PWM4T2xsfr=PWM4T2;
    PWM5T1xsfr=PWM5T1;
    PWM5T2xsfr=PWM5T2;
    PWM6T1xsfr=PWM6T1;
    PWM6T2xsfr=PWM6T2;
    PWM7T1xsfr=PWM7T1;
    PWM7T2xsfr=PWM7T2;
}

void pwm_3_initialize(bit initVolt,bit portSelect,bit t1Intr,bit t2Intr,unsigned int t1InitV,unsigned int t2InitV){
    PWMCFG|=initVolt*0x02;      // pwm3 initial v
    PWMCR|=0x02;                // enable pwm3 output
    pwm_3_timerValueSet(t1InitV,t2InitV);
    PWM3CR=portSelect*0x08|0x04|t2Intr*0x02|t1Intr*0x01;
        // Output P21/P45 | En interr | En t2 interr | En t1 interr
}

void pwm_3_timerValueSet(unsigned int t1InitV,unsigned int t2InitV){
    PWM3T1=t1InitV;
    PWM3T2=t2InitV;
    pwm_xsfrFlush();
}

void pwm_4_initialize(bit initVolt,bit portSelect,bit t1Intr,bit t2Intr,unsigned int t1InitV,unsigned int t2InitV){
    PWMCFG|=initVolt*0x04;      // pwm4 initial v
    PWMCR|=0x04;                // enable pwm4 output
    pwm_4_timerValueSet(t1InitV,t2InitV);
    PWM4CR=portSelect*0x08|0x04|t2Intr*0x02|t1Intr*0x01;
}

void pwm_4_timerValueSet(unsigned int t1InitV,unsigned int t2InitV){
    PWM4T1=t1InitV;
    PWM4T2=t2InitV;
    pwm_xsfrFlush();
}

void pwm_5_initialize(bit initVolt,bit portSelect,bit t1Intr,bit t2Intr,unsigned int t1InitV,unsigned int t2InitV){
    PWMCFG|=initVolt*0x08;      // pwm5 initial v
    PWMCR|=0x08;                // enable pwm5 output
    pwm_5_timerValueSet(t1InitV,t2InitV);
    PWM5CR=portSelect*0x08|0x04|t2Intr*0x02|t1Intr*0x01;
        // Output P23/P42 | En interr | En t2 interr | En t1 interr
}

void pwm_5_timerValueSet(unsigned int t1InitV,unsigned int t2InitV){
    PWM5T1=t1InitV;
    PWM5T2=t2InitV;
    pwm_xsfrFlush();
}

void pwm_start(){
    PWMCR|=0x80;    // disable pwm master clock interr, enable pwm module
}

static void interruptRoutine() interrupt 22{
    interrupt_busyLed=1;

    if(PWMIF&0x01){
        PWMIF&=~0x01;
    }else if(PWMIF&0x02){
        PWMIF&=~0x02;
    }else if(PWMIF&0x04){
        PWMIF&=~0x04;
    }else if(PWMIF&0x08){
        PWMIF&=~0x08;
    }else if(PWMIF&0x10){
        PWMIF&=~0x10;
    }else if(PWMIF&0x20){
        PWMIF&=~0x20;
    }else if(PWMIF&0x40){
        PWMIF&=~0x40;
    }

    interrupt_busyLed=0;
}
