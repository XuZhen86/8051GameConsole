#include<stdio.h>
#include<string.h>
#include"./FarMem.h"
#include"./FarMemBlock.h"
#include"./FarMemConfig.h"
#include"../XRAM/XRAM.h"
#include"../Delay/Delay.h"

FarMemBlock far head _at_ 0x020000;
unsigned int data
    usedSpace,
    fragmentedFreeByte,
    fragmentedFreeBlock;

void farMem_Initialize(){
    head.size=0x10000-sizeof(FarMemBlock);
    head.attr=0x00;
    head.next=NULL;

    usedSpace=0;
    fragmentedFreeByte=0;
    fragmentedFreeBlock=0;
}

void *farMalloc(unsigned int size){
    FarMemBlock *p,*np;

    for(p=&head;p!=NULL;p=p->next){
        if(p->size>=size+sizeof(FarMemBlock)&&p->attr==0x00){
            np=(void *)p+sizeof(FarMemBlock)+size;
            np->attr=0x00;
            np->next=p->next;
            np->size=p->size-size-sizeof(FarMemBlock);

            p->attr=0x01;
            p->next=np;
            p->size=size;

            usedSpace+=(size+sizeof(FarMemBlock));
            printf("[farMalloc() usedSpace=%u]\n",usedSpace);
            return (void *)p+sizeof(FarMemBlock);
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
    FarMemBlock *p=ptr-sizeof(FarMemBlock);

    if(ptr==NULL){
        return;
    }

    p->attr=0x00;
    usedSpace-=(p->size+sizeof(FarMemBlock));

    fragmentedFreeByte+=p->size;
    fragmentedFreeBlock++;

    if(fragmentedFreeByte>=FRAGMENTED_FREE_BYTE_MAX||fragmentedFreeBlock>=FRAGMENTED_FREE_BLOCK_MAX){
        defragFreeBlock();
        fragmentedFreeByte=0;
        fragmentedFreeBlock=0;
    }

    printf("[farFree() usedSpace=%u]\n",usedSpace);
}

static void defragFreeBlock(){
    FarMemBlock *p=&head;

    // printf("[defragFreeBlock()]");

    while(p!=NULL){
        if(p->attr==0x00&&p->next!=NULL&&p->next->attr==0x00){
            p->size+=sizeof(FarMemBlock)+p->next->size;
            p->next=p->next->next;
        }else{
            p=p->next;
        }
    }
}

void farDump(){
    unsigned int i;
    for(i=0;i<usedSpace;i++){
        printf("far[0x%04x]=%3bu 0x%02bx\n",i,xRam_uCharRead(i),xRam_uCharRead(i));
    }
}
