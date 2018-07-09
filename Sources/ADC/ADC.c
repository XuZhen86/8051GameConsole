#include"ADC.h"

static unsigned int xdata results[8];
static unsigned char inputPin;

static unsigned int aaa;

void adcInitialize(){
    CLK_DIV|=0x20;  // ADC_RES[1,0]|ADC_RESL[7,0]
    EA=1;
    EADC=1;
}

void adcInputPinSet(unsigned char pin){
    pin%=8;
    inputPin=pin;
    P1ASF=(0x01<<pin);
}

void adcStart(){
    ADC_CONTR=0x88|inputPin;
}

void adcInterruptCleanup(){
    ADC_CONTR=0x80|inputPin;
}

void adcResultProcess(){
    results[inputPin]=ADC_RES;
    results[inputPin]<<=8;
    results[inputPin]|=ADC_RESL;
}

unsigned int adcResultGet(unsigned char i){
    return results[i%8];
}
