#include"ListWidgetConfig.h"
#include"ListWidgetStatic.h"
#include<Debug.h>
#include<Far.h>
#include<LCD.h>
#include<ListWidget.h>
#include<Pushbutton.h>
#include<Time.h>
#include<stdio.h>
#include<string.h>

ListWidget *ListWidget_new(){
    return Far_calloc(1,sizeof(ListWidget));
}

void ListWidget_delete(ListWidget *lw){
    Far_free(lw->title);
    while(lw->count){
        Far_free(lw->items[lw->count-1]);
        lw->count--;
    }

    Far_free(lw);
}

void ListWidget_setTitle(ListWidget *lw,char *title){
    Debug(DEBUG,"ListWidget_setTitle title=%s",title);
    Far_free(lw->title);
    lw->title=Far_malloc(strlen(title)+1);
    strcpy(lw->title,title);
}

void ListWidget_addItem(ListWidget *lw,char *item){
    unsigned char len=strlen(item);
    Debug(DEBUG,"ListWidget_addItem item=%s",item);

    lw->items[lw->count]=Far_malloc(len+1);
    strcpy(lw->items[lw->count],item);
    lw->count++;
}

unsigned char ListWidget_getSelection(ListWidget *lw){
    unsigned char i,selectedRow=0,selectedItem=0;
    LCD_clear();

    LCD_setChar(0,0,POINTER_CHAR_LEFT);
    LCD_setString(0,(21-strlen(lw->title))/2,lw->title);
    LCD_setHLine(7,1);

    for(i=0;i<lw->count&&i<7;i++){
        printItem(i+1,lw->items[i]);
    }

    LCD_setChar(selectedRow+1,0,POINTER_CHAR_RIGHT);

    while(1){
        LCD_flush();

        switch(Pushbutton_getDirectionWait()){
            case PUSHBUTTON_DIRECTION_UP:
                if(selectedItem){   // If not reaching top of the list
                    if(selectedRow){    // If not reaching top of the display
                        LCD_setChar(selectedRow+1,0,' ');
                        LCD_setChar(--selectedRow+1,0,POINTER_CHAR_RIGHT);
                    }else{  // Reached top of the display, move items down one slot
                        for(i=selectedItem-1;i<selectedItem+6&&i<lw->count;i++){
                            printItem(i-selectedItem+2,lw->items[i]);
                        }
                    }
                    selectedItem--;
                }else{  // Reached top of the list
                    LCD_setChar(selectedRow+1,0,' ');
                    if(lw->count<7){    // If have less than 7 items, no need to redraw list
                        LCD_setChar((selectedItem=selectedRow=lw->count-1)+1,0,POINTER_CHAR_RIGHT);
                    }else{  // Have more than 7 items, redraw list
                        LCD_setChar((selectedRow=6)+1,0,POINTER_CHAR_RIGHT);
                        selectedItem=lw->count-1;
                        for(i=lw->count-7;i<lw->count;i++){
                            printItem(i-(lw->count-8),lw->items[i]);
                        }
                    }
                }
                break;
            case PUSHBUTTON_DIRECTION_DOWN:
                if(selectedItem!=lw->count-1){  // If not reaching end of the list
                    if(selectedRow!=6){ // If not reaching end of the display
                        LCD_setChar(selectedRow+1,0,' ');
                        LCD_setChar(++selectedRow+1,0,POINTER_CHAR_RIGHT);
                    }else{  // Reached end of the display, move items up one slot
                        for(i=selectedItem-5;i<selectedItem+2&&i<lw->count;i++){
                            printItem(i-selectedItem+6,lw->items[i]);
                        }
                    }
                    selectedItem++;
                }else{  // Reached end of the list
                    LCD_setChar(selectedRow+1,0,' ');
                    LCD_setChar((selectedRow=0)+1,0,POINTER_CHAR_RIGHT);
                    for(i=0;i<lw->count&&i<7;i++){
                        printItem(i+1,lw->items[i]);
                    }
                    selectedItem=0;
                }
                break;
            case PUSHBUTTON_DIRECTION_FORWARD:  // Confirm selection
                Pushbutton_directionReleaseWait();
                return selectedItem;
            case PUSHBUTTON_DIRECTION_BACK: // Cancel selection
                Pushbutton_directionReleaseWait();
                return lw->count;
        }

        if(lw->sigCurrentItemChanged!=NULL){
            lw->sigCurrentItemChanged(selectedItem);
        }

        Pushbutton_directionReleaseWait();
    }
}

void ListWidget_setSigCurrentItemChanged(ListWidget *lw,void (*sigCurrentItemChanged)(unsigned char)){
    lw->sigCurrentItemChanged=sigCurrentItemChanged;
}

static void printItem(unsigned char row,unsigned char *item){
    LCD_setString(row,2,"                   ");
    LCD_setString(row,2,item);
}
