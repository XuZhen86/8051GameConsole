#ifndef LCD_H_
#define LCD_H_

void LCD_init();
void LCD_hwReset();

void LCD_setBrightness(unsigned char b);
void LCD_adjustBrightness();

void LCD_flush();
void LCD_forceFlush();

void LCD_setChar(unsigned char row,unsigned char col,char c);
void LCD_setString(unsigned char row,unsigned char col,const char *str);
void LCD_setPixel(unsigned char row,unsigned char col,unsigned char lightUp);

void LCD_setHLine(unsigned char row,unsigned char lightUp);
void LCD_setVLine(unsigned char col,unsigned char lightUp);
void LCD_invertHLine(unsigned char row);

void LCD_clear();

#endif
