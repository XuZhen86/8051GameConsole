#include"Sources/Version/Version.h"
#include"Sources/Version/VersionNumbers.h"

#include"Sources/LCD12864/LCD12864.h"
#include"Sources/Pushbutton/Pushbutton.h"
#include"Sources/Universal/Universal.h"
#include"Sources/Widgets/ListWidget/ListWidget.h"

#include<stdlib.h>

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
    listWidget_selectFromList(TITLE,ITEMS,ITEM_COUNT,NULL,0);
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
