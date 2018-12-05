#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"./FarMem.h"

struct FarMemBlock{
    unsigned int size;
    unsigned char attr;
    struct FarMemBlock *next;
    unsigned char pad[2];
};

struct FarMemBlock far head _at_ 0x020000;

void farMem_Initialize(){
    head.size=0xffff-sizeof(struct FarMemBlock);
    head.attr=0x00;
    head.next=NULL;
}

void *farMalloc(unsigned int size){
    struct FarMemBlock *p,*np;

    for(p=&head;p!=NULL;p=p->next){
        if(p->size>=size+sizeof(struct FarMemBlock)){
            np=p+sizeof(struct FarMemBlock)+size;
            np->attr=0x00;
            np->next=p->next;
            np->size=p->size-size-sizeof(struct FarMemBlock);

            p->attr=0x01;
            p->next=np;
            p->size=size;

            // printf("[farMalloc() 0x%x]\n",(unsigned int)(p+sizeof(struct FarMemBlock)));
            return p+sizeof(struct FarMemBlock);
        }
    }

    return NULL;
}

void *farCalloc(unsigned int size){
    void *p=farMalloc(size);
    if(p!=NULL){
        memset(p,0x00,size);
    }
    return p;
}

void farFree(void *ptr){
    struct FarMemBlock *p=ptr;

    p-=sizeof(struct FarMemBlock);
    p->attr=0x00;
    defragFreeBlocks();

    // printf("[farFree() 0x%x]\n",(unsigned int)p);
}

static void defragFreeBlocks(){
    struct FarMemBlock *p=&head;

    while(p!=NULL){
        if(p->attr==0x00&&p->next!=NULL&&p->next->attr==0x00){
            p->size+=sizeof(struct FarMemBlock)+p->next->size;
            p->next=p->next->next;
        }else{
            p=p->next;
        }
    }
}
