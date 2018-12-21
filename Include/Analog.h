#ifndef Analog_H_
#define Analog_H_

void Analog_init();

unsigned int Analog_read(unsigned char channel);
void Analog_write(unsigned char channel,unsigned int value,unsigned char portSelect);

#endif
