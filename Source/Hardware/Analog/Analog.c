#include<Analog.h>
#include<STC15W4K48S4.h>

void Analog_init(){
    CLK_DIV|=0x20;
    ADC_CONTR=0x80;

    P_SW2|=0x80;
    PWMC=0x7fff;
    PWMCKS=0x00;
    PWMFDCR=0x00;
    PWMCR|=0x80;
}

unsigned int Analog_read(unsigned char channel){
    unsigned int result;

    P1ASF|=(1<<channel);
    ADC_CONTR=0x88|channel%8;
    while((ADC_CONTR&0x10)==0);
    P1ASF=0x00;

    result=ADC_RES;
    result<<=8;
    result|=ADC_RESL;

    return result;
}

void Analog_write(unsigned char channel,unsigned int value,unsigned char portSelect){
    switch(channel){
        case 2:
            PWMCFG|=0x01;
            PWMCR|=0x01;
            if(portSelect){PWM2CR=0x08;}
            else{PWM2CR=0x00;}

            PWMCR&=~0x80;
            PWM2T1=0x0000;
            if(value){PWM2T2=value;}
            else{PWM2T2=1;}
            PWMCR|=0x80;

            break;
        case 3:
            PWMCFG|=0x02;
            PWMCR|=0x02;
            if(portSelect){PWM3CR=0x08;}
            else{PWM3CR=0x00;}

            PWMCR&=~0x80;
            PWM3T1=0x0000;
            if(value){PWM3T2=value;}
            else{PWM3T2=1;}
            PWMCR|=0x80;

            break;
        case 4:
            PWMCFG|=0x04;
            PWMCR|=0x04;
            if(portSelect){PWM4CR=0x08;}
            else{PWM4CR=0x00;}

            PWMCR&=~0x80;
            PWM4T1=0x0000;
            if(value){PWM4T2=value;}
            else{PWM4T2=1;}
            PWMCR|=0x80;

            break;
        case 5:
            PWMCFG|=0x08;
            PWMCR|=0x08;
            if(portSelect){PWM5CR=0x08;}
            else{PWM5CR=0x00;}

            PWMCR&=~0x80;
            PWM5T1=0x0000;
            if(value){PWM5T2=value;}
            else{PWM5T2=1;}
            PWMCR|=0x80;

            break;
        case 6:
            PWMCFG|=0x10;
            PWMCR|=0x10;
            if(portSelect){PWM6CR=0x08;}
            else{PWM6CR=0x00;}

            PWMCR&=~0x80;
            PWM6T1=0x0000;
            if(value){PWM6T2=value;}
            else{PWM6T2=1;}
            PWMCR|=0x80;

            break;
        case 7:
            PWMCFG|=0x20;
            PWMCR|=0x20;
            if(portSelect){PWM7CR=0x08;}
            else{PWM7CR=0x00;}

            PWMCR&=~0x80;
            PWM7T1=0x0000;
            if(value){PWM7T2=value;}
            else{PWM7T2=1;}
            PWMCR|=0x80;

            break;
    }
}
