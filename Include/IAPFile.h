#ifndef IAPFILE_H_
#define IAPFILE_H_

typedef struct IAPFile_{
    unsigned char fileId;
    unsigned char position;
    unsigned char *fileName;
} IAPFile;

void IAPFile_init();
void IAPFile_format();

IAPFile *IAPFile_new();
void IAPFile_delete(IAPFile *f);

bit IAPFile_open(IAPFile *f,unsigned char *fileName);
void IAPFile_close(IAPFile *f);

bit IAPFile_getChar(IAPFile *f,char *c);
bit IAPFile_putChar(IAPFile *f,char c);

bit IAPFile_seek(IAPFile *f,unsigned char pos);
unsigned char IAPFile_pos(IAPFile *f);

unsigned char IAPFile_read(IAPFile *f,unsigned char *dst,unsigned char maxSize);
unsigned char IAPFile_readLine(IAPFile *f,unsigned char *dst,unsigned char maxSize);
unsigned char IAPFile_write(IAPFile *f,unsigned char *src,unsigned char maxSize);

bit IAPFile_resize(IAPFile *f,unsigned char sz);
unsigned char IAPFile_size(IAPFile *f);

#endif
