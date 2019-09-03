#ifndef IAPFILE_H_
#define IAPFILE_H_

typedef struct IAPFile_{
    unsigned char fileId;
    unsigned char position;
    char *fileName;
} IAPFile;

// Format IAP fs
void IAPFile_format(char *magicWord);

// Allocate fp
IAPFile *IAPFile_new();

// Deallocate fp
void IAPFile_delete(IAPFile *f);

// Open file by name to be associated to fp
bit IAPFile_open(IAPFile *f,char *fileName);

// Close fp
void IAPFile_close(IAPFile *f);

// Get 1 char from fp, return 1 if successful
bit IAPFile_getChar(IAPFile *f,char *c);

// Write 1 char to fp, return 1 if successful
bit IAPFile_putChar(IAPFile *f,char c);

// Seek to pos
bit IAPFile_seek(IAPFile *f,unsigned char pos);

// Get current pos
unsigned char IAPFile_pos(IAPFile *f);

// Read chars from fp
unsigned char IAPFile_read(IAPFile *f,char *dst,unsigned char maxSize);

// Read 1 line from fp
unsigned char IAPFile_readLine(IAPFile *f,char *dst,unsigned char maxSize);

// Write chars to fp
unsigned char IAPFile_write(IAPFile *f,char *src,unsigned char maxSize);

// Force resize file
bit IAPFile_resize(IAPFile *f,unsigned char sz);

// Get file size
unsigned char IAPFile_size(IAPFile *f);

#endif
