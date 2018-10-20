#ifndef XRAM_H_
#define XRAM_H_

void xRam_initialize();
unsigned char xRam_writeModeRegister(unsigned char mode);

unsigned char xRam_uCharWrite(unsigned int m16,unsigned char imm8);
unsigned char xRam_uCharRead(unsigned int m16);
unsigned int xRam_uIntWrite(unsigned int m16,unsigned int imm16);
unsigned int xRam_uIntRead(unsigned int m16);

unsigned char *xRam_uCharReadSeq(unsigned char *dst,unsigned int m16,unsigned int len);
unsigned char *xRam_uCharWriteSeq(unsigned char *src,unsigned int m16,unsigned int len);

unsigned int xRam_memset(unsigned int m16Dst,unsigned char imm8,unsigned int len);
unsigned int xRam_memcpy(unsigned int m16Dst,unsigned int m16Src,unsigned int len);

#endif
