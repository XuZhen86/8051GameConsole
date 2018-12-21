#ifndef FARSTATIC_H_
#define FARSTATIC_H_

static void defragFreeBlock();
static bit verifyFarMemBlock(void *ptr);
static unsigned int calculateFarMemBlockPad(void *ptr);

#endif
