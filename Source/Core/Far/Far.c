#include"FarConfig.h"
#include"FarMemBlock.h"
#include"FarStatic.h"
#include<Debug.h>
#include<Far.h>
#include<Time.h>
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

void *malloc(unsigned int size){
    FarMemBlock *p,*np;

    for(p=&head;p!=NULL;p=p->next){
        verifyFarMemBlock(p);

        if(p->size>=size+sizeof(FarMemBlock)&&!(p->attr&ALLOCATED)){
            np=(void *)p+p->size-size;
            np->attr=ALLOCATED;
            np->next=p->next;
            np->prev=p;
            np->size=size;
            np->pad=calculateFarMemBlockPad(np);

            if(p->next!=NULL){
                p->next->prev=np;
                p->next->pad=calculateFarMemBlockPad(p->next);
            }

            p->next=np;
            p->size=p->size-sizeof(FarMemBlock)-size;
            p->pad=calculateFarMemBlockPad(p);

            numFarMemBlock++;
            netUsedSpace+=size;

            return (void *)np+sizeof(FarMemBlock);
        }
    }

    Debug(CRITICAL,"Cannot allocate memory of size %u",size);
    return NULL;
}

void *calloc(unsigned int num,unsigned int size){
    void *p=malloc(num*size);
    if(p!=NULL){
        memset(p,0x00,num*size);
    }
    return p;
}

void *realloc(void *ptr,unsigned int size){
    void *np;

    if(ptr==NULL){
        return malloc(size);
    }
    verifyFarMemBlock(ptr);

    if(size==0){
        free(ptr);
        return NULL;
    }

    np=malloc(size);
    if(np!=NULL){
        memcpy(np,ptr,size);
        free(ptr);
    }
    return np;
}

void free(void *ptr){
    FarMemBlock *p=ptr-sizeof(FarMemBlock);

    if(ptr==NULL){
        return;
    }
    verifyFarMemBlock(p);

    p->attr&=~ALLOCATED;
    netUsedSpace-=p->size;

    while(p->prev!=NULL&&!(p->prev->attr&ALLOCATED)){
        p->prev->size+=sizeof(FarMemBlock)+p->size;

        p->prev->next=p->next;
        p->prev->pad=calculateFarMemBlockPad(p->prev);

        if(p->next!=NULL){
            p->next->prev=p->prev;
            p->next->pad=calculateFarMemBlockPad(p->next);
        }

        numFarMemBlock--;
        p=p->prev;
        verifyFarMemBlock(p);
    }

    while(p->next!=NULL&&!(p->next->attr&ALLOCATED)){
        p->size+=sizeof(FarMemBlock)+p->next->size;

        if(p->next->next!=NULL){
            p->next->next->prev=p;
            p->next->next->pad=calculateFarMemBlockPad(p->next->next);
        }

        p->next=p->next->next;
        numFarMemBlock--;
    }
    p->pad=calculateFarMemBlockPad(p);
}

void Far_memInfo(){
    Debug(DEBUG,"Mem Info >>>>>>>");
    Debug(DEBUG,"NumFarMemBlock %5u",numFarMemBlock);
    Debug(DEBUG,"NetUsedSpace   %5u B",netUsedSpace);
    Debug(DEBUG,"UsedSpace      %5u B",netUsedSpace+numFarMemBlock*sizeof(FarMemBlock));
    Debug(DEBUG,"Mem Info <<<<<<<");
}

void Far_dumpMemBlock(){
    FarMemBlock *p=&head;

    Debug(DEBUG,"Dump Mem Block >>>>>>>");

    while(p!=NULL){ //lint -e437
        Debug(DEBUG,"addr=%p size=%5u attr=0x%02bx next=%p prev=%p pad=0x%04x",p,*p);
        p=p->next;
    }

    Debug(DEBUG,"Dump Mem Block <<<<<<<");
}

void Far_speedTest(){
    unsigned int j;
    unsigned char i=0;
    char *array;

    Debug(DEBUG,"Far Mem Speed Test >>>>>>>");

    for(j=1;j;j*=2){
        Time_start();
        do{
            array=calloc(j,sizeof(char));
            free(array);
        }while(++i);
        Debug(DEBUG,"fcalloc(%u): %lu",j,Time_elapsed());
    }

    array=malloc((unsigned int)32768);
    Time_start();
    do{
        array[rand()]=j;
    }while(++j);
    Debug(DEBUG,"array[rand()] 32768: %lu",Time_elapsed());
    free(array);

    Debug(DEBUG,"Far Mem Speed Test <<<<<<<");
}

static bit verifyFarMemBlock(FarMemBlock *p){
    if(calculateFarMemBlockPad(p)!=p->pad){
        Debug(CRITICAL,"Memory Corruption p=%p pad=0x%04x expect=0x%04x",p,p->pad,calculateFarMemBlockPad(p));
        return 0;
    }

    return 1;
}

static unsigned int calculateFarMemBlockPad(FarMemBlock *p){
    unsigned char *bytes=(unsigned char *)p,pad0=0x00,pad1=0x00,i;

    for(i=0;i<sizeof(FarMemBlock)-2;i++){
        pad0^=bytes[i];
        pad1+=bytes[i];
    }

    return ((unsigned int)pad0<<8)|pad1;
}
