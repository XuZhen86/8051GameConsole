#ifndef LCD_H_
#define LCD_H_

void lcd_spi_send(unsigned char c,unsigned char b);
void lcd_spi_send2Bytes(unsigned char c1,unsigned char c2,unsigned char b);
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
void lcd_flush();
void lcd_forceFlush();

void lcd_charSet(unsigned char row,unsigned char col,unsigned char c);
void lcd_stringSet(unsigned char row,unsigned char col,unsigned char *str);
void lcd_pixelSet(unsigned char row,unsigned char col,unsigned char lightUp);
void lcd_clear();

void lcd_hLineSet(unsigned char row,unsigned char lightUp);

#endif
