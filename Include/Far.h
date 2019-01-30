#ifndef FAR_H_
#define FAR_H_

void Far_init();

void *fmalloc(unsigned int size);
void *fcalloc(unsigned int num,unsigned int size);
void *frealloc(void *ptr,unsigned int size);
void ffree(void *ptr);

void Far_memInfo();
void Far_dumpMemBlock();
void Far_speedTest();

#endif
