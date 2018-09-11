#ifndef LCD12864_H_
#define LCD12864_H_

#include<Sources/Main/STC15W4K48S4.h>
#include<Sources/PWM/PWM.h>
#include<Sources/Universal/Universal.h>
#include<Sources/SPI/SPI.h>
#include<Sources/I23LC512/I23LC512.h>

#include<stdlib.h>
#include<string.h>

void lcd12864_spi_send(bit b,unsigned char c);
void lcd12864_spi_send2Bytes(bit b,unsigned char c1,unsigned char c2);
void lcd12864_hwReset();
void lcd12864_pwmInitialize();
void lcd12864_brightnessSet(unsigned int brightness);
unsigned int lcd12864_brightnessGet();

void lcd12864_spi_initialize();
bit lcd12864_flush(bit forceFlush);
void lcd12864_charSet(unsigned char row,unsigned char col,unsigned char c);
void lcd12864_stringSet(unsigned char row,unsigned char col,unsigned char *str);
void lcd12864_pixelSet(unsigned char row,unsigned char col,bit lightUp);

#endif
