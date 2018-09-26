#ifndef UNIVERSAL_H_
#define UNIVERSAL_H_

bit delay(unsigned char i,unsigned char j,unsigned char k);
bit delayBusy(unsigned char i,unsigned char j,unsigned char k);

char* uctoa(unsigned char value,char *str);
char* uitoa(unsigned int value,char *str);
char* ultoa(unsigned long int value,char *str);

#endif
