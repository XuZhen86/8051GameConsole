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

// Print debug message, invoked by Debug() macro
void Debug_print(unsigned char level,const char *message,...);

// Set file & line for next Debug_print(). Invoked by Debug() macro
void Debug_setCurrentFileLine(char *file,unsigned int line);

// Enable certain debug level
void Debug_setEnable(unsigned char mask);

// Disable certain debug level
void Debug_setDisable(unsigned char mask);

// Show menu to select which debug levels to show
void Debug_show();

// Disable all debug output & saves code space
#ifdef NO_DEBUG_OUTPUT
    #define Debug if(0)Debug_print
#else
    #define Debug Debug_setCurrentFileLine(__FILE__,__LINE__);Debug_print
#endif

#endif
