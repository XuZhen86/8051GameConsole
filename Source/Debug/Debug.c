#include<Debug.h>
#include<STC15W4K48S4.h>
#include<Time.h>
#include<stdarg.h>
#include<stdio.h>

static unsigned char debugMask=0x00;

static unsigned int currentLine;
static char *currentFile;

void Debug_print(unsigned char level,char *message,...){
    if(level&debugMask){
        va_list args;
        va_start(args,message);

        printf("[%02bu:%02bu:%02bu.%03u] ",Time_hour(),Time_minute(),Time_second(),Time_msec());

        switch(level){
            case DEBUG:
                printf("[dbug]");
                break;
            case INFO:
                printf("[info]");
                break;
            case WARNING:
                printf("[warn]");
                break;
            case CRITICAL:
                printf("[crit]");
                break;
            case FATAL:
                printf("[ftal]");
                break;
            default:
                printf("[    ]");
        }

        printf("%3bu:%s[%4u]: ",(SP-0x19)/2-5,currentFile+7,currentLine);

        vprintf(message,args);
        putchar('\n');

        va_end(args);
    }
}

void Debug_setCurrentFileLine(char *file,unsigned int line){
    currentFile=file;
    currentLine=line;
}

void Debug_setEnable(unsigned char mask){
    debugMask|=mask;
}

void Debug_setDisable(unsigned char mask){
    debugMask&=~mask;
}
