#include"Sources/ADC/ADC.h"

static unsigned char adcSpeed=0;

void adc_initialize(unsigned char speed){
    CLK_DIV|=0x20;  // ADC_RES[1,0]|ADC_RESL[7,0]
    ADC_CONTR=0x80;
    adcSpeed=(speed&0x03)<<5;
}

unsigned int adc_get(unsigned char channel){
    unsigned int result=0;
    channel%=8;

    P1ASF|=(1<<channel);
    ADC_CONTR=0x88|adcSpeed|channel;
    while(!(ADC_CONTR&0x10));
    P1ASF=0;

    result=ADC_RES;
    result<<=8;
    result|=ADC_RESL;

    return result;
}
