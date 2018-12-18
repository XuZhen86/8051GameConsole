#include<string.h>
#include<stdio.h>
#include"./IAPFile.h"
#include"./IAPFileConfig.h"
#include"../IAP/IAP.h"
#include"../FarMem/FarMem.h"

void IapFile_initialize(){
}

void IapFile_format(){
    unsigned int i;
    for(i=0;i<512;i++){
        Iap_write(i,0);
    }
    Iap_flush();
}

IapFile *IapFile_new(){
    IapFile *file=farMalloc(sizeof(IapFile));
    file->fileId=FILE_ID_MAX;
    return file;
}

void IapFile_delete(IapFile *f){
    farFree(f);
}

bit IapFile_open(IapFile *f,unsigned char *fileName){
    unsigned int i;
    unsigned char j,found;

    for(i=0;i<FILE_ID_MAX;i++){
        found=1;
        for(j=0;j<15&&fileName[j];j++){
            if(Iap_read(i*16+j)!=fileName[j]){
                found=0;
                break;
            }
        }

        if(found){   // Found existing file
            f->fileId=i;
            f->position=0;
            return 1;
        }else if(Iap_read(i*16)==0){    // Create new file
            for(j=0;j<15&&fileName[j];j++){
                Iap_write(i*16+j,fileName[j]);
            }
            for(j=0;j<255;j++){
                Iap_write((i+1)*256+j,0);
            }
            Iap_write((i+1)*256+255,0);

            f->fileId=i;
            f->position=0;
            setFileSize(f,0);

            return 1;
        }
    }

    return 0;
}

void IapFile_close(IapFile *f){
    Iap_flush();
    f->fileId=FILE_ID_MAX;
}

bit IapFile_getChar(IapFile *f,char *c) small{
    if(f->fileId==FILE_ID_MAX||f->position==getFileSize(f)){ // Reached end of file
        return 0;
    }

    (*c)=Iap_read((f->fileId+1)*256+f->position);
    f->position++;

    return 1;
}

bit IapFile_putChar(IapFile *f,char c) small{
    if(f->fileId==FILE_ID_MAX||f->position==FILE_SIZE_MAX){  // Reached max file size
        return 0;
    }

    Iap_write((f->fileId+1)*256+f->position,c);
    f->position++;

    if(getFileSize(f)<f->position){  // Refresh file size
        setFileSize(f,f->position);
    }

    return 1;
}

bit IapFile_seek(IapFile *f,unsigned char pos){
    if(f->fileId==FILE_ID_MAX||pos>getFileSize(f)){
        return 0;
    }

    f->position=pos;
    return 1;
}

unsigned char IapFile_pos(IapFile *f){
    if(f->fileId==FILE_ID_MAX){
        return 0;
    }

    return f->position;
}

unsigned char IapFile_read(IapFile *f,unsigned char *dst,unsigned char maxSize) small{
    unsigned char i,c;

    if(f->fileId==FILE_ID_MAX){
        return 0;
    }

    for(i=0;i<maxSize;i++){
        if(IapFile_getChar(f,&c)){
            dst[i]=c;
        }else{
            dst[i]=0;
            break;
        }
    }

    return i;
}

unsigned char IapFile_readLine(IapFile *f,unsigned char *dst,unsigned char maxSize) small{
    unsigned char i,c;

    if(f->fileId==FILE_ID_MAX){
        return 0;
    }

    for(i=0;i<maxSize;i++){
        if(IapFile_getChar(f,&c)){
            dst[i]=c;
            if(c=='\n'){
                dst[i+1]=0;
                break;
            }
        }else{
            dst[i]=0;
            break;
        }
    }

    return i;
}

unsigned char IapFile_write(IapFile *f,unsigned char *src,unsigned char maxSize) small{
    unsigned char i;

    if(f->fileId==FILE_ID_MAX){
        return 0;
    }

    for(i=0;i<maxSize;i++){
        if(!IapFile_putChar(f,src[i])){
            break;
        }
    }

    return i;
}

bit IapFile_resize(IapFile *f,unsigned char sz){
    if(f->fileId==FILE_ID_MAX||sz>FILE_SIZE_MAX){
        return 0;
    }

    setFileSize(f,sz);
    return 1;
}

unsigned char IapFile_size(IapFile *f){
    return getFileSize(f);
}

static unsigned char getFileSize(IapFile *f) small{
    return Iap_read(f->fileId*16+15);
}

static unsigned char setFileSize(IapFile *f,unsigned char sz) small{
    Iap_write(f->fileId*16+15,sz);
    return sz;
}

void IapFile_test(){
    unsigned int i;
    unsigned char buffer[16];

    IapFile *file=IapFile_new();

    IapFile_open(file,"FileTest.txt");
    IapFile_write(file,"Test Line 1\n",strlen("Test Line 1\n"));
    IapFile_write(file,"Test Line 2\n",strlen("Test Line 2\n"));
    IapFile_write(file,"Test Line 3\n",strlen("Test Line 3\n"));

    IapFile_open(file,"FileTest2.txt");
    IapFile_write(file,"Test Line 4\n",strlen("Test Line 4\n"));
    IapFile_write(file,"Test Line 5\n",strlen("Test Line 5\n"));
    IapFile_write(file,"Test Line 6\n",strlen("Test Line 6\n"));

    IapFile_open(file,"FileTest3.txt");
    for(i=0;i<256;i++){
        sprintf(buffer,"%d ",i);
        IapFile_write(file,buffer,strlen(buffer));
    }

    IapFile_delete(file);
}
