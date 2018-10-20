#ifndef STACK_H_
#define STACK_H_

void stack_initialize(unsigned int m16Max);

unsigned int push16(unsigned int imm16);
unsigned char push8(unsigned char imm8);

unsigned int pop16();
unsigned char pop8();

unsigned int r16(unsigned int offs16);
unsigned int r16a(unsigned int offs16,unsigned int idx);
unsigned char r8(unsigned int offs16);
unsigned char r8a(unsigned int offs16,unsigned int idx);

unsigned int w16(unsigned int offs16,unsigned int imm16);
unsigned int w16a(unsigned int offs16,unsigned int idx,unsigned int imm16);
unsigned char w8(unsigned int offs16,unsigned char imm8);
unsigned char w8a(unsigned int offs16,unsigned int idx,unsigned char imm8);

unsigned int inc16(unsigned int offs16);
unsigned int dec16(unsigned int offs16);
unsigned char inc8(unsigned int offs16);
unsigned char dec8(unsigned int offs16);

unsigned int stack_memset(unsigned int offs16,unsigned char imm8,unsigned int len);

void enter(unsigned int imm16);
void leave();

// void stack_dump();

#endif
