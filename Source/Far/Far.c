#include"FarConfig.h"
#include"FarMemBlock.h"
#include"FarStatic.h"
#include<Debug.h>
#include<Far.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

static FarMemBlock far head _at_ 0x020000;
static unsigned int data numFarMemBlock,netUsedSpace;

void Far_init(){
    head.size=0x10000-sizeof(FarMemBlock);
    head.attr=0x00;
    head.next=NULL;
    head.prev=NULL;
    head.pad=calculateFarMemBlockPad(&head);

    numFarMemBlock=1;
    netUsedSpace=0;
}

void *Far_malloc(unsigned int size){
    FarMemBlock *p,*np;

    Debug(DEBUG,HERE,"Far_malloc size=%u",size);

    for(p=&head;p!=NULL;p=p->next){
        verifyFarMemBlock(p);

        if(p->size>=size+sizeof(FarMemBlock)&&!(p->attr&ALLOCATED)){
            np=(void *)p+p->size-size;
            np->attr|=ALLOCATED;
            np->next=p->next;
            np->prev=p;
            np->size=size;
            np->pad=calculateFarMemBlockPad(np);

            if(p->next!=NULL){
                p->next->prev=np;
            }

            p->next=np;
            p->size=p->size-sizeof(FarMemBlock)-size;
            p->pad=calculateFarMemBlockPad(p);

            numFarMemBlock++;
            netUsedSpace+=size;

            Debug(DEBUG,HERE,
                "np=0x%x size=%u numFarMemBlock=%u netUsedSpace=%u usedSpace=%u",
                (unsigned int)np,
                size,
                numFarMemBlock,
                netUsedSpace,
                netUsedSpace+numFarMemBlock*sizeof(FarMemBlock));

            return (void *)np+sizeof(FarMemBlock);
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

void *Far_realloc(void *ptr,unsigned int size){
    void *newPtr;

    Debug(DEBUG,HERE,"Far_realloc p=0x%x size=%u",(unsigned int)ptr,size);

    if(ptr==NULL){
        return Far_malloc(size);
    }
    verifyFarMemBlock(ptr);

    if(size==0){
        Far_free(ptr);
        return NULL;
    }

    newPtr=Far_malloc(size);
    if(newPtr!=NULL){
        memcpy(newPtr,ptr,size);
        Far_free(ptr);
    }
    return newPtr;
}

void Far_free(void *ptr){
    FarMemBlock *p=ptr-sizeof(FarMemBlock);

    if(ptr==NULL){
        Debug(DEBUG,HERE,"Far_free p=0x%x size=%u NULL",(unsigned int)p,p->size);
        return;
    }
    verifyFarMemBlock(p);

    Debug(DEBUG,HERE,"Far_free p=0x%x size=%u",(unsigned int)p,p->size);

    p->attr&=~ALLOCATED;
    netUsedSpace-=p->size;

    while(p->prev!=NULL&&!(p->prev->attr&ALLOCATED)){
        Debug(DEBUG,HERE,
            "Merge p=0x%x prev=0x%x",
            (unsigned int)p,
            (unsigned int)(p->prev));

        p->prev->size+=sizeof(FarMemBlock)+p->size;

        p->prev->next=p->next;
        p->prev->pad=calculateFarMemBlockPad(p->prev);

        if(p->next!=NULL){
            p->next->prev=p->prev;
            p->next->pad=calculateFarMemBlockPad(p->next);
        }

        numFarMemBlock--;
        p=p->prev;
    }

    while(p->next!=NULL&&!(p->next->attr&ALLOCATED)){
        Debug(DEBUG,HERE,
            "Merge p=0x%x next=0x%x",
            (unsigned int)p,
            (unsigned int)(p->next));

        p->size+=sizeof(FarMemBlock)+p->next->size;

        if(p->next->next!=NULL){
            p->next->next->prev=p;
            p->next->next->pad=calculateFarMemBlockPad(p->next->next);
        }

        p->next=p->next->next;
        p->pad=calculateFarMemBlockPad(p);

        numFarMemBlock--;
    }

    Debug(DEBUG,HERE,
        "numFarMemBlock=%u netUsedSpace=%u usedSpace=%u",
        numFarMemBlock,
        netUsedSpace,
        netUsedSpace+numFarMemBlock*sizeof(FarMemBlock));
}

static bit verifyFarMemBlock(void *ptr){
    FarMemBlock *p=ptr;

    if(calculateFarMemBlockPad(ptr)!=p->pad){
        Debug(CRITICAL,HERE,
            "Memory Corruption p=0x%04x pad=0x%04x expect=0x%04x",
            (unsigned int)p,
            p->pad,
            calculateFarMemBlockPad(ptr));

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

    Debug(DEBUG,HERE,
        "p=0x%x pad0=0x%x pad1=0x%x",
        (unsigned int)ptr,
        (unsigned int)pad0,
        (unsigned int)pad1);
    return ((unsigned int)pad0<<8)|pad1;
}
