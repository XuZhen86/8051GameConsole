#ifndef IAP_H_
#define IAP_H_

void IAP_init();

unsigned char IAP_read(unsigned int addr16);
unsigned char IAP_write(unsigned int addr16,unsigned char imm8);
void IAP_flush();
const unsigned char *IAP_getPtr(unsigned int addr16);

#endif
