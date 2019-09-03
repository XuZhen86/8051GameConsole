#ifndef LCD_H_
#define LCD_H_

// Init LCD module
void LCD_init();

// Hw reset
void LCD_hwReset();

// Set LCD backlight by using DAC
void LCD_setBrightness(int b);

// Prompt user to select a brightness
void LCD_adjustBrightness();

// Flush changed pixels to LCD
void LCD_flush();

// Flush all pixels to LCD, regardless if changed
void LCD_forceFlush();

// Print a char
void LCD_setChar(unsigned char row,unsigned char col,char c);

// Print a string
void LCD_setString(unsigned char row,unsigned char col,const char *str);

// Modify a pixel
void LCD_setPixel(unsigned char row,unsigned char col,unsigned char lightUp);

// Draw a horizontal line
void LCD_setHLine(unsigned char row,unsigned char lightUp);

// Draw a vertical line
void LCD_setVLine(unsigned char col,unsigned char lightUp);

// Invert a horizontal line
void LCD_invertHLine(unsigned char row);

// Clear all, does not flush immediately
void LCD_clear();

#endif
