#ifndef FARMEM_H_
#define FARMEM_H_

void farMem_Initialize();
void *farMalloc(unsigned int size) small;
void *farCalloc(unsigned int num,unsigned int size) small;
void farFree(void *ptr) small;

static void defragFreeBlock() small;
static bit verifyFarMemBlock(void *ptr) small;
static unsigned char calculateFarMemBlockPad0(void *ptr) small;
static unsigned char calculateFarMemBlockPad1(void *ptr) small;

void farDump();

#endif
