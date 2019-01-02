#ifndef FARMEMBLOCK_H_
#define FARMEMBLOCK_H_

typedef struct FarMemBlock_{
    unsigned int size;
    unsigned char attr;
    struct FarMemBlock_ *next,*prev;
    unsigned int pad;
} FarMemBlock;

#endif
