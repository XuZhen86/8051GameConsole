#include<stdio.h>
#include<string.h>
#include"./FarMem.h"
#include"./FarMemBlock.h"
#include"./FarMemConfig.h"

struct FarMemBlock far head _at_ 0x020000;
unsigned int data usedSpace,fragmentedFreeSpace;

void farMem_Initialize(){
    head.size=0xffff-sizeof(struct FarMemBlock);
    head.attr=0x00;
    head.next=NULL;

    usedSpace=0;
    fragmentedFreeSpace=0;
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

            usedSpace+=(size+sizeof(struct FarMemBlock));

            printf("[farMalloc() p=0x%0x usedSpace=%u]\n",(unsigned int)(p+sizeof(struct FarMemBlock)),usedSpace);
            return p+sizeof(struct FarMemBlock);
        }
    }

    return NULL;
}

void *farCalloc(unsigned int num,unsigned int size){
    void *p=farMalloc(num*size);
    if(p!=NULL){
        memset(p,0x00,num*size);
    }
    return p;
}

void farFree(void *ptr){
    struct FarMemBlock *p=ptr;

    p-=sizeof(struct FarMemBlock);
    p->attr=0x00;
    usedSpace-=(p->size+sizeof(struct FarMemBlock));

    fragmentedFreeSpace+=p->size;
    if(fragmentedFreeSpace>=FRAGMENTED_FREE_SPACE_MAX){
        defragFreeSpace();
        fragmentedFreeSpace=0;
    }

    printf("[farFree() p=0x%0x usedSpace=%u]\n",(unsigned int)p,usedSpace);
}

static void defragFreeSpace(){
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
