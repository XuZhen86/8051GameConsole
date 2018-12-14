#ifndef FARMEMBLOCK_H_
#define FARMEMBLOCK_H_

typedef struct FarMemBlock_{
    unsigned int size;
    unsigned char attr;
    struct FarMemBlock_ *next;
    unsigned int pad;
} FarMemBlock;

#endif
