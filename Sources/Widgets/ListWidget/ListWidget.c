#include<stdio.h>
#include<string.h>
#include"../../LCD/LCD.h"
#include"../../Pushbutton/Pushbutton.h"
#include"../../Universal/Universal.h"
#include"../../Clock/Clock.h"
#include"../../FarMem/FarMem.h"
#include"./ListWidget.h"
#include"./ListWidgetConfig.h"

ListWidget *ListWidget_new(){
    return farCalloc(1,sizeof(ListWidget));
}

void ListWidget_delete(ListWidget *lw){
    farFree(lw->title);
    while(lw->count){
        farFree(lw->items[lw->count-1]);
        lw->count--;
    }

    farFree(lw);
}

void ListWidget_setTitle(ListWidget *lw,char *title){
    farFree(lw->title);
    lw->title=farMalloc(strlen(title)+1);
    strcpy(lw->title,title);
}

void ListWidget_addItem(ListWidget *lw,char *item){
    unsigned char len=strlen(item);

    lw->items[lw->count]=farMalloc(len+1);
    strcpy(lw->items[lw->count],item);
    lw->count++;
}

unsigned char ListWidget_getSelection(ListWidget *lw){
    unsigned char i,selectedRow=0,selectedItem=0;
    unsigned char buffer[BUFFER_SIZE];
    lcd_clear();

    lcd_charSet(0,0,POINTER_CHAR_LEFT);
    lcd_stringSet(0,(21-strlen(lw->title))/2,lw->title);
    lcd_hLineSet(7,1);

    for(i=0;i<lw->count&&i<7;i++){
        strcpySpaceExtend(buffer,lw->items[i]);
        lcd_stringSet(i+1,2,buffer);
    }

    lcd_charSet(selectedRow+1,0,POINTER_CHAR_RIGHT);

    while(1){
        lcd_flush();

        switch(pushbutton_waitDirectionGet()){
            case PUSHBUTTON_DIRECTION_UP:
                if(selectedItem){   // If not reaching top of the list
                    if(selectedRow){    // If not reaching top of the display
                        lcd_charSet(selectedRow+1,0,' ');
                        lcd_charSet(--selectedRow+1,0,POINTER_CHAR_RIGHT);
                    }else{  // Reached top of the display, move items down one slot
                        for(i=selectedItem-1;i<selectedItem+6&&i<lw->count;i++){
                            strcpySpaceExtend(buffer,lw->items[i]);
                            lcd_stringSet(i-selectedItem+2,2,buffer);
                        }
                    }
                    selectedItem--;
                }else{  // Reached top of the list
                    lcd_charSet(selectedRow+1,0,' ');
                    if(lw->count<7){    // If have less than 7 items, no need to redraw list
                        lcd_charSet((selectedItem=selectedRow=lw->count-1)+1,0,POINTER_CHAR_RIGHT);
                    }else{  // Have more than 7 items, redraw list
                        lcd_charSet((selectedRow=6)+1,0,POINTER_CHAR_RIGHT);
                        selectedItem=lw->count-1;
                        for(i=lw->count-7;i<lw->count;i++){
                            strcpySpaceExtend(buffer,lw->items[i]);
                            lcd_stringSet(i-(lw->count-8),2,buffer);
                        }
                    }
                }
                break;
            case PUSHBUTTON_DIRECTION_DOWN:
                if(selectedItem!=lw->count-1){  // If not reaching end of the list
                    if(selectedRow!=6){ // If not reaching end of the display
                        lcd_charSet(selectedRow+1,0,' ');
                        lcd_charSet(++selectedRow+1,0,POINTER_CHAR_RIGHT);
                    }else{  // Reached end of the display, move items up one slot
                        for(i=selectedItem-5;i<selectedItem+2&&i<lw->count;i++){
                            strcpySpaceExtend(buffer,lw->items[i]);
                            lcd_stringSet(i-selectedItem+6,2,buffer);
                        }
                    }
                    selectedItem++;
                }else{  // Reached end of the list
                    lcd_charSet(selectedRow+1,0,' ');
                    lcd_charSet((selectedRow=0)+1,0,POINTER_CHAR_RIGHT);
                    for(i=0;i<lw->count&&i<7;i++){
                        strcpySpaceExtend(buffer,lw->items[i]);
                        lcd_stringSet(i+1,2,buffer);
                    }
                    selectedItem=0;
                }
                break;
            case PUSHBUTTON_DIRECTION_FORWARD:  // Confirm selection
                pushbutton_waitDirectionRelease();
                return selectedItem;
            case PUSHBUTTON_DIRECTION_BACK: // Cancel selection
                pushbutton_waitDirectionRelease();
                return lw->count;
        }

        if(lw->sigCurrentItemChanged!=NULL){
            lw->sigCurrentItemChanged(selectedItem);
        }

        pushbutton_waitDirectionRelease();
    }
}

void ListWidget_setSigCurrentItemChanged(ListWidget *lw,void (*sigCurrentItemChanged)(unsigned char)){
    lw->sigCurrentItemChanged=sigCurrentItemChanged;
}

void ListWidget_print(ListWidget *lw){
    unsigned char i;

    printf("[ListWidget title=%s count=%bu]\n",lw->title,lw->count);
    for(i=0;i<lw->count;i++){
        printf("[items[%bu]=%s]\n",i,lw->items[i]);
    }
}

static void strcpySpaceExtend(unsigned char *dst,unsigned char *src){
    unsigned char i;
    bit spaceExt=0;

    for(i=0;i<BUFFER_SIZE-1;i++){
        if(spaceExt){
            dst[i]=' ';
        }else{
            if(src[i]!=0){
                dst[i]=src[i];
            }else{
                spaceExt=1;
                dst[i]=' ';
            }
        }
    }
    dst[BUFFER_SIZE-1]=0;
}
