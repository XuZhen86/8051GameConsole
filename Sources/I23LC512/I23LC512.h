#ifndef __I23LC512_H_
#define __I23LC512_H_

#include<Sources/Main/STC15W4K48S4.h>
#include<Sources/Universal/Universal.h>
#include<Sources/SPI/SPI.h>

unsigned char i23lc512WriteModeRegister(const unsigned char mode);
unsigned char i23lc512UCharWrite(const unsigned int address,const unsigned char c);
unsigned char i23lc512UCharArrayWrite(const unsigned int address,const unsigned int offset,const unsigned char c);
unsigned char i23lc512UCharRead(const unsigned int address);
unsigned char i23lc512UCharArrayRead(const unsigned int address,const unsigned int offset);
unsigned int i23lc512UIntWrite(const unsigned int address,const unsigned int i);
unsigned int i23lc512UIntArrayWrite(const unsigned int address,const unsigned int offset,const unsigned int i);
unsigned int i23lc512UIntRead(const unsigned int address);
unsigned int i23lc512UIntArrayRead(const unsigned int address,const unsigned int offset);
bit i23lc512Initialize();

unsigned char *i23lc512UCharSeqRead(unsigned char *destination,unsigned int address,unsigned int length);
unsigned char *i23lc512UCharSeqWrite(unsigned char *source,unsigned int address,unsigned int length);
unsigned int i23lc512Memset(unsigned int address,unsigned char value,unsigned int num);

#endif
