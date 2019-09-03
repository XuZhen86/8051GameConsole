#ifndef Analog_H_
#define Analog_H_

// Initialize analog module
void Analog_init();

// ADC
unsigned int Analog_read(unsigned char channel);

// DAC
void Analog_write(unsigned char channel,unsigned int value,unsigned char portSelect);

#endif
