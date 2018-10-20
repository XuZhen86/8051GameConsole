#ifndef LCD_H_
#define LCD_H_

void lcd_spi_send(unsigned char c,bit b);
void lcd_spi_send2Bytes(unsigned char c1,unsigned char c2,bit b);
void lcd_hwReset();
void lcd_pwm_initialize();

void lcd_brightnessAdjust();
unsigned char lcd_brightnessLevelGet();
void lcd_brightnessLevelSet(unsigned char level);

void lcd_iap_read();
void lcd_iap_write();
unsigned int lcd_iap_brightnessRawGet();
unsigned int lcd_iap_brightnessRawSet(unsigned int brightness);

void lcd_spi_initialize();
bit lcd_flush(bit forceFlush);
void lcd_charSet(unsigned char row,unsigned char col,unsigned char c);
void lcd_stringSet(unsigned char row,unsigned char col,unsigned char *str);
void lcd_pixelSet(unsigned char row,unsigned char col,bit lightUp);
void lcd_clear();

bit lcd_bufferStackPush();
bit lcd_bufferStackPop();
void lcd_bufferStackClear();

// void lcd_rowReverse(unsigned char startRow,unsigned char endRow);
void lcd_hLineSet(unsigned char row,bit lightUp);

#endif
