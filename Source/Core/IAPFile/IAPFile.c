#include"IAPFileConfig.h"
#include"IAPFileStatic.h"
#include<Debug.h>
#include<IAP.h>
#include<IAPFile.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void IAPFile_format(char *magicWord){
    unsigned int i;

    // Format only if magicWord is correct
    if(strcmp(magicWord,"Glitch!")!=0){
        Debug(WARNING,"Trying to format IAP with wrong magic word");
        return;
    }

    for(i=0;i<512;i++){
        IAP_write(i,0);
    }
    IAP_flush();

    Debug(WARNING,"IAP formatted");
}

IAPFile *IAPFile_new(){
    IAPFile *file=malloc(sizeof(IAPFile));
    file->fileId=FILE_ID_MAX;
    file->fileName=NULL;
    return file;
}

void IAPFile_delete(IAPFile *f){
    free(f->fileName);
    free(f);
}

bit IAPFile_open(IAPFile *f,char *fileName){
    unsigned int i;
    unsigned char j/*,found*/;

    for(i=0;i<FILE_ID_MAX;i++){
        if(!strcmp((const char *)IAP_getPtr(i*16),fileName)){   // Found existing file
            f->fileId=i;
            f->position=0;
            f->fileName=malloc(strlen(fileName)+1);
            strcpy(f->fileName,fileName);
            return 1;
        }else if(IAP_read(i*16)==0){                            // Create new file
            Debug(WARNING,"Create new file \"%s\"",fileName);

            // Record file name in table
            for(j=0;j<15&&fileName[j];j++){
                IAP_write(i*16+j,(unsigned char)fileName[j]);
            }

            // Clear file content, 256 bytes in total
            for(j=0;j<255;j++){
                IAP_write((i+1)*256+j,0);
            }
            IAP_write((i+1)*256+255,0);

            f->fileId=i;
            f->position=0;
            f->fileName=malloc(strlen(fileName)+1);
            strcpy(f->fileName,fileName);
            setFileSize(f,0);

            return 1;
        }
    }

    // If no file in table has the same file name
    // And there is no space to create a file
    Debug(WARNING,"Reached full IAP file capacity");
    return 0;
}

void IAPFile_close(IAPFile *f){
    if(f->fileId>=FILE_ID_MAX){
        Debug(WARNING,"Trying to close File@%p with fileId %bu",f,f->fileId);
        return;
    }

    IAP_flush();
    f->fileId=FILE_ID_MAX;
}

bit IAPFile_getChar(IAPFile *f,char *c){
    if(f->fileId>=FILE_ID_MAX){
        Debug(WARNING,"Trying to read File@%p with fileId %bu",f,f->fileId);
        return 0;
    }else if(f->position==getFileSize(f)){  // Reached end of file
        Debug(WARNING,"Reached EOF of file \"%s\"",f->fileName);
        return 0;
    }

    // Get 1 char and pos++
    (*c)=(char)IAP_read((f->fileId+1)*256+f->position);
    f->position++;

    return 1;
}

bit IAPFile_putChar(IAPFile *f,char c){
    if(f->fileId>=FILE_ID_MAX){
        Debug(WARNING,"Trying to write File@%p with fileId %bu",f,f->fileId);
        return 0;
    }else if(f->position==FILE_SIZE_MAX){   // Reached max file size
        Debug(WARNING,"Reached max file size of file \"%s\"",f->fileName);
        return 0;
    }

    // Write 1 char and pos++
    IAP_write((f->fileId+1)*256+f->position,(unsigned char)c);
    f->position++;

    // Refresh file size
    if(getFileSize(f)<f->position){
        setFileSize(f,f->position);
    }

    return 1;
}

bit IAPFile_seek(IAPFile *f,unsigned char pos){
    if(f->fileId>=FILE_ID_MAX){
        Debug(WARNING,"Trying to seek File@%p with fileId %bu",f,f->fileId);
        return 0;
    }else if(pos>getFileSize(f)){
        Debug(WARNING,"Trying to seek outside of file \"%s\"",f->fileName);
        return 0;
    }

    f->position=pos;
    return 1;
}

unsigned char IAPFile_pos(IAPFile *f){
    if(f->fileId>=FILE_ID_MAX){
        Debug(WARNING,"Trying to get pos of File@%p with fileId %bu",f,f->fileId);
        return 0;
    }

    return f->position;
}

unsigned char IAPFile_read(IAPFile *f,char *dst,unsigned char maxSize){
    unsigned char i;
    char c;

    if(f->fileId>=FILE_ID_MAX){
        Debug(WARNING,"Trying to read File@%p with fileId %bu",f,f->fileId);
        return 0;
    }

    for(i=0;i<maxSize;i++){
        if(IAPFile_getChar(f,&c)){
            dst[i]=c;
        }else{
            dst[i]=0;
            break;
        }
    }

    return i;
}

unsigned char IAPFile_readLine(IAPFile *f,char *dst,unsigned char maxSize){
    unsigned char i;
    char c;

    if(f->fileId>=FILE_ID_MAX){
        Debug(WARNING,"Trying to read File@%p with fileId %bu",f,f->fileId);
        return 0;
    }

    for(i=0;i<maxSize;i++){
        // Read until EOF or line break
        if(IAPFile_getChar(f,&c)){
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

unsigned char IAPFile_write(IAPFile *f,char *src,unsigned char maxSize){
    unsigned char i;

    if(f->fileId>=FILE_ID_MAX){
        Debug(WARNING,"Trying to write File@%p with fileId %bu",f,f->fileId);
        return 0;
    }

    for(i=0;i<maxSize;i++){
        // Put char until max file size
        if(!IAPFile_putChar(f,src[i])){
            break;
        }
    }

    return i;
}

bit IAPFile_resize(IAPFile *f,unsigned char sz){
    if(f->fileId>=FILE_ID_MAX){
        Debug(WARNING,"Trying to resize File@%p with fileId %bu",f,f->fileId);
        return 0;
    }else if(sz>FILE_SIZE_MAX){
        Debug(WARNING,"Invalid new size of file \"%s\"",f->fileName);
        return 0;
    }

    setFileSize(f,sz);
    return 1;
}

unsigned char IAPFile_size(IAPFile *f){
    return getFileSize(f);
}

static unsigned char getFileSize(IAPFile *f){
    return IAP_read(f->fileId*16+15);
}

static unsigned char setFileSize(IAPFile *f,unsigned char sz){
    IAP_write(f->fileId*16+15,sz);
    return sz;
}
