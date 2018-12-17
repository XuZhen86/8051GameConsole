#include<stdio.h>
#include<string.h>
#include"./FarMem.h"
#include"./FarMemBlock.h"
#include"./FarMemConfig.h"
#include"../XRAM/XRAM.h"

static FarMemBlock far head _at_ 0x020000;
static unsigned int
    numFarMemBlock,
    netUsedSpace,
    fragmentedFreeByte,
    fragmentedFreeBlock;

void farMem_Initialize(){
    head.size=0x10000-sizeof(FarMemBlock);
    head.attr=0x00;
    head.next=NULL;
    head.pad=calculateFarMemBlockPad(&head);

    numFarMemBlock=1;
    netUsedSpace=0;
    fragmentedFreeByte=0;
    fragmentedFreeBlock=0;
}

void *farMalloc(unsigned int size) small{
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

void *farCalloc(unsigned int num,unsigned int size) small{
    void *p=farMalloc(num*size);
    if(p!=NULL){
        memset(p,0x00,num*size);
    }
    return p;
}

void farFree(void *ptr) small{
    FarMemBlock *p=ptr-sizeof(FarMemBlock);

    if(ptr==NULL){
        return;
    }else if(!verifyFarMemBlock(p)){
        printf("[farFree() Memory Corruption p=0x%0x]\n",(unsigned int)p);
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

static void defragFreeBlock() small{
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

static bit verifyFarMemBlock(void *ptr) small{
    FarMemBlock *p=ptr;

    if(calculateFarMemBlockPad(ptr)!=p->pad){
        return 0;
    }
    return 1;
}

static unsigned int calculateFarMemBlockPad(void *ptr) small{
    unsigned char *bytes=ptr,pad0=0x00,pad1=0x00,i;

    for(i=0;i<sizeof(FarMemBlock)-2;i++){
        pad0^=bytes[i];
        pad1+=bytes[i];
    }

    // printf("[pad0=0x%x pad1=0x%x]\n",(unsigned int)pad0,(unsigned int)pad1);
    return ((unsigned int)pad0<<8)|pad1;
}

void farDump(){
    unsigned int i;
    for(i=0;i<netUsedSpace+numFarMemBlock*sizeof(FarMemBlock);i++){
        printf("far[0x%04x]=%3bu 0x%02bx\n",i,xRam_uCharRead(i),xRam_uCharRead(i));
    }
}
