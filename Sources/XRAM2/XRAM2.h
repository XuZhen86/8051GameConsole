#ifndef XRAM2_H_
#define XRAM2_H_

void xRam2_initialize();
void xRam2_enterSqiMode();
unsigned char xRam2_writeModeRegister(unsigned char mode);

unsigned char xRam2_uCharWrite(unsigned int m16,unsigned char imm8);
unsigned char xRam2_uCharRead(unsigned int m16);
unsigned int xRam2_uIntWrite(unsigned int m16,unsigned int imm16);
unsigned int xRam2_uIntRead(unsigned int m16);

unsigned char *xRam2_uCharReadSeq(unsigned char *dst,unsigned int m16,unsigned int len);
unsigned char *xRam2_uCharWriteSeq(unsigned char *src,unsigned int m16,unsigned int len);

unsigned int xRam2_memset(unsigned int m16Dst,unsigned char imm8,unsigned int len);
unsigned int xRam2_memcpy(unsigned int m16Dst,unsigned int m16Src,unsigned int len);

// void _xRam2_test();

#endif
