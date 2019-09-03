#ifndef SERIAL_H_
#define SERIAL_H_

// Put 1 char to serial bus. Used by printf
char putchar(char c);

// Init serial bus 1
void Serial1_init();

// Set mode of serial bus 1
void Serial1_setMode(unsigned char mode);

#endif
