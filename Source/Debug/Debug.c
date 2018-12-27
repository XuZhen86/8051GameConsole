#include<Debug.h>
#include<stdarg.h>
#include<stdio.h>

static unsigned char debugMask=0x00;

void Debug(unsigned char level,unsigned int line,char *file,char *message,...){
    if(level&debugMask){
        va_list args;
        va_start(args,message);

        switch(level){
            case DEBUG:
                printf("[debug]");
                break;
            case INFO:
                printf("[info]");
                break;
            case WARNING:
                printf("[warning]");
                break;
            case CRITICAL:
                printf("[critical]");
                break;
            case FATAL:
                printf("[fatal]");
                break;
            default:
                printf("[unknown]");
        }

        printf(" (%s:%u) - ",file,line);
        vprintf(message,args);
        putchar('\n');

        va_end(args);
    }
}

void Debug_setEnable(unsigned char mask){
    debugMask|=mask;
}

void Debug_setDisable(unsigned char mask){
    debugMask&=~mask;
}
