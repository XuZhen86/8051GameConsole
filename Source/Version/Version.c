#include"VersionNumber.h"
#include<ListWidget.h>
#include<Version.h>
#include<stdlib.h>

#define xStringize(s) stringize(s)
#define stringize(s) #s

unsigned char code *TITLE="Version";
unsigned char code ITEM_COUNT=5;
unsigned char code *ITEMS[]={
    "Ver  "VERSION_STRING,
    "Date "__DATE2__,
    "Time "__TIME__,
    "C51  "xStringize(__C51__),
    "Mem  "xStringize(__MODEL__)
};

void Version_show(){
    ListWidget *lw=ListWidget_new();
    unsigned char i;

    ListWidget_setTitle(lw,TITLE);
    for(i=0;i<ITEM_COUNT;i++){
        ListWidget_addItem(lw,ITEMS[i]);
    }

    ListWidget_getSelection(lw);
    ListWidget_delete(lw);
}

unsigned char Version_getCompileDate(){
    return __DATE2__;
}

unsigned char Version_getCompileTime(){
    return __TIME__;
}

unsigned char Version_getC51Version(){
    return xStringize(__C51__);
}

unsigned char Version_getMemModel(){
    return xStringize(__MODEL__);
}

char *Version_getVersionString(){
    return VERSION_STRING;
}
