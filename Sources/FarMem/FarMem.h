#ifndef FARMEM_H_
#define FARMEM_H_

void farMem_Initialize();
void *farMalloc(unsigned int size);
void *farCalloc(unsigned int num,unsigned int size);
void farFree(void *ptr);

static void defragFreeBlock();
static bit verifyFarMemBlock(void *ptr);
static unsigned char calculateFarMemBlockPad0(void *ptr);
static unsigned char calculateFarMemBlockPad1(void *ptr);

void farDump();

#endif
