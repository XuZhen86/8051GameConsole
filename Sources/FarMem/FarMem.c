#include<stdio.h>
#include<string.h>
#include"./FarMem.h"
#include"./FarMemBlock.h"
#include"./FarMemConfig.h"
#include"../XRAM/XRAM.h"

FarMemBlock far head _at_ 0x020000;
unsigned int data
    usedSpace,
    fragmentedFreeByte,
    fragmentedFreeBlock;

void farMem_Initialize(){
    head.size=0x10000-sizeof(FarMemBlock);
    head.attr=0x00;
    head.next=NULL;
    head.pad[0]=calculateFarMemBlockPad0(&head);
    head.pad[1]=calculateFarMemBlockPad1(&head);

    usedSpace=0;
    fragmentedFreeByte=0;
    fragmentedFreeBlock=0;
}

void *farMalloc(unsigned int size){
    FarMemBlock *p,*np;

    for(p=&head;p!=NULL;p=p->next){
        if(!verifyFarMemBlock(p)){
            printf("[farMalloc() Memory Corruption p=0x%0x]\n",(unsigned int)p);
            while(1);
        }

        if(p->size>=size+sizeof(FarMemBlock)&&p->attr==0x00){
            np=(void *)p+sizeof(FarMemBlock)+size;
            np->attr=0x00;
            np->next=p->next;
            np->size=p->size-size-sizeof(FarMemBlock);
            np->pad[0]=calculateFarMemBlockPad0(np);
            np->pad[1]=calculateFarMemBlockPad1(np);

            p->attr=0x01;
            p->next=np;
            p->size=size;
            p->pad[0]=calculateFarMemBlockPad0(p);
            p->pad[1]=calculateFarMemBlockPad1(p);

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
    }else if(!verifyFarMemBlock(p)){
        printf("[farFree() Memory Corruption p=0x%0x]\n",(unsigned int)p);
        while(1);
    }

    p->attr=0x00;
    p->pad[0]=calculateFarMemBlockPad0(p);
    p->pad[1]=calculateFarMemBlockPad1(p);
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
        if(!verifyFarMemBlock(p)){
            printf("[defragFreeBlock() Memory Corruption p=0x%0x]\n",(unsigned int)p);
            while(1);
        }

        if(p->attr==0x00&&p->next!=NULL&&p->next->attr==0x00){
            p->size+=sizeof(FarMemBlock)+p->next->size;
            p->next=p->next->next;
            p->pad[0]=calculateFarMemBlockPad0(p);
            p->pad[1]=calculateFarMemBlockPad1(p);
        }else{
            p=p->next;
        }
    }
}

static bit verifyFarMemBlock(void *ptr){
    FarMemBlock *p=ptr;

    if(calculateFarMemBlockPad0(ptr)!=p->pad[0]){
        return 0;
    }
    if(calculateFarMemBlockPad1(ptr)!=p->pad[1]){
        return 0;
    }

    return 1;
}

static unsigned char calculateFarMemBlockPad0(void *ptr){
    unsigned char *bytes=ptr;
    unsigned char pad;
    unsigned char i;

    pad=0;
    for(i=0;i<6;i++){
        pad^=bytes[i];
    }

    // printf("[pad0=0x%0x]\n",(unsigned int)pad);
    return pad;
}

static unsigned char calculateFarMemBlockPad1(void *ptr){
    unsigned char *bytes=ptr;
    unsigned char pad;
    unsigned char i;

    pad=0;
    for(i=0;i<6;i++){
        pad+=bytes[i];
    }

    // printf("[pad1=0x%0x]\n",(unsigned int)pad);
    return pad;
}

void farDump(){
    unsigned int i;
    for(i=0;i<usedSpace;i++){
        printf("far[0x%04x]=%3bu 0x%02bx\n",i,xRam_uCharRead(i),xRam_uCharRead(i));
    }
}
