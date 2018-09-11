#ifndef I23LC512_H_
#define I23LC512_H_

#include<Sources/Main/STC15W4K48S4.h>
#include<Sources/Universal/Universal.h>
#include<Sources/SPI/SPI.h>

unsigned char i23lc512_writeModeRegister(unsigned char mode);
unsigned char i23lc512_uCharWrite(unsigned int address,unsigned char c);
unsigned char i23lc512_uCharArrayWrite(unsigned int address,unsigned int offset,unsigned char c);
unsigned char i23lc512_uCharRead(unsigned int address);
unsigned char i23lc512_uCharArrayRead(unsigned int address,unsigned int offset);
unsigned int i23lc512_uIntWrite(unsigned int address,unsigned int i);
unsigned int i23lc512_uIntArrayWrite(unsigned int address,unsigned int offset,unsigned int i);
unsigned int i23lc512_uIntRead(unsigned int address);
unsigned int i23lc512_uIntArrayRead(unsigned int address,unsigned int offset);
bit i23lc512_initialize();

unsigned char *i23lc512_uCharSeqRead(unsigned char *destination,unsigned int address,unsigned int length);
unsigned char *i23lc512_uCharSeqWrite(unsigned char *source,unsigned int address,unsigned int length);
unsigned int i23lc512_memset(unsigned int address,unsigned char value,unsigned int num);

#endif
