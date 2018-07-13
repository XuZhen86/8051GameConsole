#ifndef __LCD12864_H_
#define __LCD12864_H_

#include<Sources/Main/STC15W4K48S4.h>
#include<Sources/PWM/PWM.h>
#include<Sources/Universal/Universal.h>
#include<Sources/SPI/SPI.h>
#include<Sources/I23LC512/I23LC512.h>

#include<stdlib.h>

void lcd12864SpiSend(bit b,unsigned char c);
void lcd12864SpiSend2Bytes(bit b,unsigned char c1,unsigned char c2);
void lcd12864HwReset();
void lcd12864PwmInitialize();
void lcd12864BrightnessSet(unsigned int brightness);
unsigned int lcd12864BrightnessGet();

void lcd12864SpiInitialize();
bit lcd12864Flush(bit forceFlush);
void lcd12864CharSet(unsigned char row,unsigned char col,unsigned char c);
void lcd12864StringSet(unsigned char row,unsigned char col,unsigned char *str);
void lcd12864PixelSet(unsigned char row,unsigned char col,bit lightUp);

#endif
