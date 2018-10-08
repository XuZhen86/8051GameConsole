#ifndef IAP_H_
#define IAP_H_

void iap_initialize();

unsigned char iap_sectorReadByte(unsigned int address);
void iap_sectorRead(unsigned char sector);

unsigned char iap_sectorWriteByte(unsigned int address,unsigned char c);
void iap_sectorWrite(unsigned char sector);
void iap_sectorErase(unsigned char sector);

unsigned char iap_uCharGet(unsigned int offset);
unsigned char iap_uCharSet(unsigned int offset,unsigned char c);
unsigned int iap_uIntGet(unsigned int offset);
unsigned int iap_uIntSet(unsigned int offset,unsigned int i);

// void _iap_sectorTest(unsigned char sector);

#endif
