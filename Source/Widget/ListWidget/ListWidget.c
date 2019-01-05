#include"ListWidgetConfig.h"
#include"ListWidgetStatic.h"
#include<Debug.h>
#include<Far.h>
#include<LCD.h>
#include<ListWidget.h>
#include<Pushbutton.h>
#include<string.h>

ListWidget *ListWidget_new(){
    ListWidget *lw=fcalloc(1,sizeof(ListWidget));
    lw->items=Vector_new();

    return lw;
}

void ListWidget_delete(ListWidget *lw){
    unsigned int i;

    ffree(lw->title);

    for(i=0;i<Vector_size(lw->items);i++){
        ListWidgetItem_delete(Vector_get(lw->items,i));
    }
    Vector_delete(lw->items);

    ffree(lw);
}

void ListWidget_setTitle(ListWidget *lw,const char *title){
    ffree(lw->title);
    lw->title=fmalloc(strlen(title));
    strcpy(lw->title,title);
}

void ListWidget_addItem(ListWidget *lw,ListWidgetItem *item){
    Vector_add(lw->items,item);
}

void ListWidget_insertItem(ListWidget *lw,unsigned int row,ListWidgetItem *item){
    Vector_insert(lw->items,row,item);
}

void ListWidget_enableAll(ListWidget *lw){
    unsigned int i;

    for(i=0;i<Vector_size(lw->items);i++){
        ListWidgetItem_setEnabled(Vector_get(lw->items,i),1);
    }
}

ListWidgetItem *ListWidget_item(ListWidget *lw,unsigned int row){
    return Vector_get(lw->items,row);
}

ListWidgetItem *ListWidget_currrentItem(ListWidget *lw){
    return ListWidget_item(lw,lw->selected);
}

unsigned int ListWidget_count(ListWidget *lw){
    return Vector_size(lw->items);
}

unsigned int ListWidget_getSelection(ListWidget *lw){
    LCD_clear();

    LCD_setChar(0,0,POINTER_CHAR_LEFT);
    LCD_setString(0,(21-strlen(lw->title))/2,lw->title);
    LCD_setHLine(7,1);

    ListWidgetItem_setSelected(Vector_get(lw->items,lw->selected),1);

    while(1){
        printItems(lw);
        LCD_flush();

        switch(Pushbutton_getDirectionWait()){
            case PUSHBUTTON_DIRECTION_UP:
                if(lw->selected){
                    ListWidgetItem_setSelected(Vector_get(lw->items,lw->selected),0);
                    lw->selected--;
                    ListWidgetItem_setSelected(Vector_get(lw->items,lw->selected),1);

                    if(lw->itemShowStart>lw->selected){
                        lw->itemShowStart=lw->selected;
                    }
                }else{
                    ListWidgetItem_setSelected(Vector_get(lw->items,lw->selected),0);
                    lw->selected=ListWidget_count(lw)-1;
                    ListWidgetItem_setSelected(Vector_get(lw->items,lw->selected),1);

                    if(ListWidget_count(lw)>=7){
                        lw->itemShowStart=ListWidget_count(lw)-7;
                    }
                }

                break;
            case PUSHBUTTON_DIRECTION_DOWN:
                if(lw->selected!=ListWidget_count(lw)-1){
                    ListWidgetItem_setSelected(Vector_get(lw->items,lw->selected),0);
                    lw->selected++;
                    ListWidgetItem_setSelected(Vector_get(lw->items,lw->selected),1);

                    if(lw->selected>6&&lw->itemShowStart<lw->selected-6){
                        lw->itemShowStart=lw->selected-6;
                    }
                }else{
                    ListWidgetItem_setSelected(Vector_get(lw->items,lw->selected),0);
                    lw->selected=0;
                    ListWidgetItem_setSelected(Vector_get(lw->items,lw->selected),1);

                    lw->itemShowStart=0;
                }

                break;
            case PUSHBUTTON_DIRECTION_FORWARD:
                Pushbutton_directionReleaseWait();
                return lw->selected;
            case PUSHBUTTON_DIRECTION_BACK:
                Pushbutton_directionReleaseWait();
                return ListWidget_count(lw);
            default:
                Debug(WARNING,"Unexpected button direction");
        }

        Pushbutton_directionReleaseWait();
    }
}

static void printItems(ListWidget *lw){
    unsigned char i;

    for(i=0;i<7;i++){
        ListWidgetItem_show(Vector_get(lw->items,lw->itemShowStart+i),i+1);
    }
}
