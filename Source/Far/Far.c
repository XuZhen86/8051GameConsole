#include"FarConfig.h"
#include"FarMemBlock.h"
#include"FarStatic.h"
#include<Far.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

static FarMemBlock far head _at_ 0x020000;
static unsigned int data numFarMemBlock,netUsedSpace,fragmentedFreeByte,fragmentedFreeBlock;

void Far_init(){
    head.size=0x10000-sizeof(FarMemBlock);
    head.attr=0x00;
    head.next=NULL;
    head.pad=calculateFarMemBlockPad(&head);

    numFarMemBlock=1;
    netUsedSpace=0;
    fragmentedFreeByte=0;
    fragmentedFreeBlock=0;
}

void *Far_malloc(unsigned int size){
    FarMemBlock *p,*np;

    for(p=&head;p!=NULL;p=p->next){
        if(!verifyFarMemBlock(p)){
            printf("[Far_malloc() Memory Corruption p=0x%04x pad=0x%04x 0x%04x]\n",(unsigned int)p,p->pad,calculateFarMemBlockPad(p));
            while(1);
        }

        if(p->size>=size+sizeof(FarMemBlock)&&p->attr==0x00){
            np=(void *)p+sizeof(FarMemBlock)+size;
            np->attr=0x00;
            np->next=p->next;
            np->size=p->size-size-sizeof(FarMemBlock);
            np->pad=calculateFarMemBlockPad(np);

            p->attr=0x01;
            p->next=np;
            p->size=size;
            p->pad=calculateFarMemBlockPad(p);

            numFarMemBlock++;
            netUsedSpace+=size;
            // printf("[farMalloc() size=%u numFarMemBlock=%u netUsedSpace=%u usedSpace=%u]\n",size,numFarMemBlock,netUsedSpace,netUsedSpace+numFarMemBlock*sizeof(FarMemBlock));
            return (void *)p+sizeof(FarMemBlock);
        }
    }

    return NULL;
}

void *Far_calloc(unsigned int num,unsigned int size){
    void *p=Far_malloc(num*size);
    if(p!=NULL){
        memset(p,0x00,num*size);
    }
    return p;
}

void Far_free(void *ptr){
    FarMemBlock *p=ptr-sizeof(FarMemBlock);

    if(ptr==NULL){
        return;
    }else if(!verifyFarMemBlock(p)){
        printf("[Far_free() Memory Corruption p=0x%0x]\n",(unsigned int)p);
        while(1);
    }

    p->attr=0x00;
    p->pad=calculateFarMemBlockPad(p);

    netUsedSpace-=p->size;
    fragmentedFreeByte+=p->size;
    fragmentedFreeBlock++;

    if(fragmentedFreeByte>=FRAGMENTED_FREE_BYTE_MAX||fragmentedFreeBlock>=FRAGMENTED_FREE_BLOCK_MAX){
        defragFreeBlock();
        fragmentedFreeByte=0;
        fragmentedFreeBlock=0;
    }

    // printf("[farFree() numFarMemBlock=%u netUsedSpace=%u usedSpace=%u]\n",numFarMemBlock,netUsedSpace,netUsedSpace+numFarMemBlock*sizeof(FarMemBlock));
}

static void defragFreeBlock(){
    FarMemBlock *p=&head;

    while(p!=NULL){
        if(!verifyFarMemBlock(p)){
            printf("[defragFreeBlock() Memory Corruption p=0x%0x]\n",(unsigned int)p);
            while(1);
        }

        if(p->attr==0x00&&p->next!=NULL&&p->next->attr==0x00){
            p->size+=sizeof(FarMemBlock)+p->next->size;
            p->next=p->next->next;
            p->pad=calculateFarMemBlockPad(p);

            numFarMemBlock--;
        }else{
            p=p->next;
        }
    }
}

static bit verifyFarMemBlock(void *ptr){
    FarMemBlock *p=ptr;

    if(calculateFarMemBlockPad(ptr)!=p->pad){
        return 0;
    }
    return 1;
}

static unsigned int calculateFarMemBlockPad(void *ptr){
    unsigned char *bytes=ptr,pad0=0x00,pad1=0x00,i;

    for(i=0;i<sizeof(FarMemBlock)-2;i++){
        pad0^=bytes[i];
        pad1+=bytes[i];
    }

    // printf("[pad0=0x%x pad1=0x%x]\n",(unsigned int)pad0,(unsigned int)pad1);
    return ((unsigned int)pad0<<8)|pad1;
}

