#ifndef DEBUG_H_
#define DEBUG_H_

enum DEBUG_LEVEL{
    DEBUG=0x01,
    INFO=0x02,
    WARNING=0x04,
    CRITICAL=0x08,
    FATAL=0x10,
    TIME_STAMP=0x20,
};

void Debug_print(unsigned char level,const char *message,...);
void Debug_setCurrentFileLine(char *file,unsigned int line);

void Debug_setEnable(unsigned char mask);
void Debug_setDisable(unsigned char mask);

void Debug_show();

#ifdef NO_DEBUG_OUTPUT
    #define Debug if(0)Debug_print
#else
    #define Debug Debug_setCurrentFileLine(__FILE__,__LINE__);Debug_print
#endif

#endif
