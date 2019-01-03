#include"VersionNumber.h"
#include"VersionStatic.h"
#include<ListWidget.h>
#include<Version.h>

void Version_show(){
    ListWidget *lw=ListWidget_new();
    unsigned char i;

    ListWidget_setTitle(lw,TITLE);
    for(i=0;i<ITEM_COUNT;i++){
        ListWidget_addItem(lw,ListWidgetItem_new(ITEMS[i],0));
    }
    ListWidget_enableAll(lw);

    ListWidget_getSelection(lw);
    ListWidget_delete(lw);
}

char *Version_getCompileDate(){
    return __DATE2__;
}

char *Version_getCompileTime(){
    return __TIME__;
}

char *Version_getC51Version(){
    return xStringize(__C51__);
}

char *Version_getMemModel(){
    return xStringize(__MODEL__);
}

char *Version_getVersionString(){
    return VERSION_STRING;
}
