#ifndef IAP_H_
#define IAP_H_

// Init IAP module
void IAP_init();

// Read 1 char from address
unsigned char IAP_read(unsigned int addr16);

// Write 1 char to address, implicitly flushing buffer
unsigned char IAP_write(unsigned int addr16,unsigned char imm8);

// Force flush buffer
void IAP_flush();

// Get read ptr to address. May return a ptr in code or xcode region
const unsigned char *IAP_getPtr(unsigned int addr16);

#endif
