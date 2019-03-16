#include<Debug.h>
#include<STC15W4K48S4.h>
#include<Time.h>
#include<stdarg.h>
#include<stdio.h>
#include<ListWidget.h>

static unsigned char debugMask=0xff;

static unsigned int currentLine;
static char *currentFile;

void Debug_print(unsigned char level,const char *message,...){
    va_list args;

    if(!(level&debugMask)){
        return;
    }

    va_start(args,message);

    if(debugMask&TIME_STAMP){
        printf("[%02bu:%02bu:%02bu.%03u] ",Time_hour(),Time_minute(),Time_second(),Time_msec());
    }

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

void Debug_show(){
    ListWidget *lw=ListWidget_new();
    ListWidgetItem *lwi;
    unsigned char newDebugMask=debugMask;

    ListWidget_setTitle(lw,"Debug Output");

    lwi=ListWidgetItem_new("Debug",ITEM_CHECKABLE);
    ListWidgetItem_setChecked(lwi,newDebugMask&DEBUG);
    ListWidget_addItem(lw,lwi);

    lwi=ListWidgetItem_new("Info",ITEM_CHECKABLE);
    ListWidgetItem_setChecked(lwi,newDebugMask&INFO);
    ListWidget_addItem(lw,lwi);

    lwi=ListWidgetItem_new("Warning",ITEM_CHECKABLE);
    ListWidgetItem_setChecked(lwi,newDebugMask&WARNING);
    ListWidget_addItem(lw,lwi);

    lwi=ListWidgetItem_new("Critical",ITEM_CHECKABLE);
    ListWidgetItem_setChecked(lwi,newDebugMask&CRITICAL);
    ListWidget_addItem(lw,lwi);

    lwi=ListWidgetItem_new("Fatal",ITEM_CHECKABLE);
    ListWidgetItem_setChecked(lwi,newDebugMask&FATAL);
    ListWidget_addItem(lw,lwi);

    lwi=ListWidgetItem_new("Time Stamp",ITEM_CHECKABLE);
    ListWidgetItem_setChecked(lwi,newDebugMask&TIME_STAMP);
    ListWidget_addItem(lw,lwi);

    lwi=ListWidgetItem_new("Save",ITEM_SELECTABLE);
    ListWidget_addItem(lw,lwi);

    ListWidget_enableAll(lw);

    while(1){
        switch(ListWidget_getSelection(lw)){
            case 0:
                ListWidgetItem_check(ListWidget_item(lw,0));
                if(ListWidgetItem_isChecked(ListWidget_item(lw,0))){
                    newDebugMask|=DEBUG;
                }else{
                    newDebugMask&=~DEBUG;
                }
                break;
            case 1:
                ListWidgetItem_check(ListWidget_item(lw,1));
                if(ListWidgetItem_isChecked(ListWidget_item(lw,1))){
                    newDebugMask|=INFO;
                }else{
                    newDebugMask&=~INFO;
                }
                break;
            case 2:
                ListWidgetItem_check(ListWidget_item(lw,2));
                if(ListWidgetItem_isChecked(ListWidget_item(lw,2))){
                    newDebugMask|=WARNING;
                }else{
                    newDebugMask&=~WARNING;
                }
                break;
            case 3:
                ListWidgetItem_check(ListWidget_item(lw,3));
                if(ListWidgetItem_isChecked(ListWidget_item(lw,3))){
                    newDebugMask|=CRITICAL;
                }else{
                    newDebugMask&=~CRITICAL;
                }
                break;
            case 4:
                ListWidgetItem_check(ListWidget_item(lw,4));
                if(ListWidgetItem_isChecked(ListWidget_item(lw,4))){
                    newDebugMask|=FATAL;
                }else{
                    newDebugMask&=~FATAL;
                }
                break;
            case 5:
                ListWidgetItem_check(ListWidget_item(lw,5));
                if(ListWidgetItem_isChecked(ListWidget_item(lw,5))){
                    newDebugMask|=TIME_STAMP;
                }else{
                    newDebugMask&=~TIME_STAMP;
                }
                break;
            case 6:
                debugMask=newDebugMask;
                ListWidget_delete(lw);
                return;
            default:
                ListWidget_delete(lw);
                return;
        }
    }
}
