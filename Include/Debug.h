#ifndef DEBUG_H_
#define DEBUG_H_

enum DEBUG_LEVEL{
    DEBUG=0x01,
    INFO=0x02,
    WARNING=0x04,
    CRITICAL=0x08,
    FATAL=0x10,
};

void Debug(unsigned char level,unsigned int line,char *file,char *message,...);
void Debug_setEnable(unsigned char mask);
void Debug_setDisable(unsigned char mask);

#define HERE __LINE__,__FILE__

#endif
