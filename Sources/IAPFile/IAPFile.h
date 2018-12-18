#ifndef IAPFILE_H_
#define IAPFILE_H_

typedef struct IapFile_{
    unsigned char fileId;
    unsigned char position;
} IapFile;

void IapFile_initialize();
void IapFile_format();

IapFile *IapFile_new();
void IapFile_delete(IapFile *f);
bit IapFile_open(IapFile *f,unsigned char *fileName);
void IapFile_close(IapFile *f);

bit IapFile_getChar(IapFile *f,char *c) small;
bit IapFile_putChar(IapFile *f,char c) small;
bit IapFile_seek(IapFile *f,unsigned char pos);
unsigned char IapFile_pos(IapFile *f);
unsigned char IapFile_read(IapFile *f,unsigned char *dst,unsigned char maxSize) small;
unsigned char IapFile_readLine(IapFile *f,unsigned char *dst,unsigned char maxSize) small;
unsigned char IapFile_write(IapFile *f,unsigned char *src,unsigned char maxSize) small;
bit IapFile_resize(IapFile *f,unsigned char sz);
unsigned char IapFile_size(IapFile *f);

static unsigned char getFileSize(IapFile *f) small;
static unsigned char setFileSize(IapFile *f,unsigned char sz) small;

void IapFile_test();

#endif
