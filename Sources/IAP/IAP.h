#ifndef IAP_H_
#define IAP_H_

void Iap_initialize();

unsigned char Iap_read(unsigned int addr16) small;
unsigned char Iap_write(unsigned int addr16,unsigned char imm8) small;
void Iap_flush();

static void loadSector(unsigned char sector) small;
static void unloadSector(unsigned char sector) small;
static void eraseSector(unsigned char sector) small;
static void writeSector(unsigned char sector) small;

void Iap_test();
void Iap_dump(unsigned int startAddr,unsigned int endAddr);

#endif
