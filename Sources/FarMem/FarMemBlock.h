#ifndef FARMEMBLOCK_H_
#define FARMEMBLOCK_H_

static struct FarMemBlock{
    unsigned int size;
    unsigned char attr;
    struct FarMemBlock *next;
    unsigned char pad[2];
};

#endif
