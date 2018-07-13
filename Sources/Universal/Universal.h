#ifndef __UNIVERSAL_H_
#define __UNIVERSAL_H_

#include<Sources/Main/STC15W4K48S4.h>

bit delay(unsigned char i,unsigned char j,unsigned char k);
bit delayBusy(unsigned char i,unsigned char j,unsigned char k);
char* uitoa(unsigned int value,char *str);
char* ultoa(unsigned long int value,char *str);

#endif
