#ifndef __ADC_H_
#define __ADC_H_

unsigned int xdata adcResults[8];
unsigned char adcInputPin;

void adcInitialize();
void adcInputPinSet(unsigned char pin);
void adcInterruptCleanup();
void adcResultProcess();

void adcInitialize(){
    CLK_DIV|=0x20;  // ADC_RES[1,0]|ADC_RESL[7,0]
    EA=1;
    EADC=1;
}

void adcInputPinSet(unsigned char pin){
    pin%=8;
    adcInputPin=pin;
    P1ASF=(0x01<<pin);
}

void adcStart(){
    ADC_CONTR=0x88|adcInputPin;
}

void adcInterruptCleanup(){
    ADC_CONTR=0x80|adcInputPin;
}

void adcResultProcess(){
    adcResults[adcInputPin]=ADC_RES;
    adcResults[adcInputPin]<<=8;
    adcResults[adcInputPin]|=ADC_RESL;
}

#endif
