#include"Sources/Version/Version.h"
#include"Sources/Version/VersionNumbers.h"

#include"Sources/LCD/LCD.h"
#include"Sources/Pushbutton/Pushbutton.h"
#include<stdlib.h>
#include"../Universal/Universal.h"
#include"../Widgets/ListWidget/ListWidget.h"
#include"./Version.h"
#include"./VersionNumbers.h"

#define xStringize(s) stringize(s)
#define stringize(s) #s

unsigned char code *TITLE="Version";
unsigned char code ITEM_COUNT=5;
unsigned char code *ITEMS[]={
    "Ver  "VERSION_NUMBER,
    "Date "__DATE2__,
    "Time "__TIME__,
    "C51  "xStringize(__C51__),
    "Mem  "xStringize(__MODEL__)
};

void version_showVersion(){
    ListWidget *lw=ListWidget_new();
    unsigned char i;

    ListWidget_setTitle(lw,TITLE);
    for(i=0;i<ITEM_COUNT;i++){
        ListWidget_addItem(lw,ITEMS[i]);
    }

    ListWidget_getSelection(lw);
    ListWidget_delete(lw);
}

unsigned char version_compileDateGet(){
    return __DATE2__;
}

unsigned char version_compileTimeGet(){
    return __TIME__;
}

unsigned char version_c51VersionGet(){
    return xStringize(__C51__);
}

unsigned char version_memModelGet(){
    return xStringize(__MODEL__);
}
