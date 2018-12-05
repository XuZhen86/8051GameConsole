#ifndef FARMEM_H_
#define FARMEM_H_

void farMem_Initialize();
void *farMalloc(unsigned int size);
void *farCalloc(unsigned int size);
void farFree(void *ptr);

static void defragFreeBlocks();

#endif
