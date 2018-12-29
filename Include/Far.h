#ifndef FAR_H_
#define FAR_H_

void Far_init();

void *Far_malloc(unsigned int size);
void *Far_calloc(unsigned int num,unsigned int size);
void *Far_realloc(void *ptr,unsigned int size);
void Far_free(void *ptr);

#endif
