#ifndef LCD12864_H_
#define LCD12864_H_

void lcd12864_spi_send(bit b,unsigned char c);
void lcd12864_spi_send2Bytes(bit b,unsigned char c1,unsigned char c2);
void lcd12864_hwReset();
void lcd12864_pwm_initialize();

void lcd12864_brightnessAdjust();
unsigned char lcd12864_brightnessLevelGet();
void lcd12864_brightnessLevelSet(unsigned char level);

void lcd12864_iap_read();
void lcd12864_iap_write();
unsigned int lcd12864_iap_brightnessRawGet();
unsigned int lcd12864_iap_brightnessRawSet(unsigned int brightness);

void lcd12864_spi_initialize();
bit lcd12864_flush(bit forceFlush);
void lcd12864_charSet(unsigned char row,unsigned char col,unsigned char c);
void lcd12864_stringSet(unsigned char row,unsigned char col,unsigned char *str);
void lcd12864_pixelSet(unsigned char row,unsigned char col,bit lightUp);
void lcd12864_clear();

bit lcd12864_bufferStackPush();
bit lcd12864_bufferStackPop();
void lcd12864_bufferStackClear();

// void lcd12864_rowReverse(unsigned char startRow,unsigned char endRow);
void lcd12864_hLineSet(unsigned char row,bit lightUp);

#endif
