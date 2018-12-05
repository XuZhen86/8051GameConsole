#include"../Main/STC15W4K48S4.h"
#include"./ADC.h"

static unsigned char adcSpeed=0;

void adc_initialize(unsigned char speed){
    CLK_DIV|=0x20;
    ADC_CONTR=0x80;
    adcSpeed=(speed&0x03)<<5;
}

unsigned int adc_get(unsigned char channel){
    unsigned int result;

    P1ASF|=(1<<channel);
    ADC_CONTR=0x88|adcSpeed|channel%8;
    while((ADC_CONTR&0x10)==0);
    P1ASF=0x00;

    result=ADC_RES;
    result<<=8;
    result|=ADC_RESL;

    return result;
}
