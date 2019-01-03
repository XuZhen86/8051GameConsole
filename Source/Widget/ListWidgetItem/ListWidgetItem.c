#include"ListWidgetItemStatic.h"
#include<Far.h>
#include<LCD.h>
#include<ListWidgetItem.h>
#include<string.h>

ListWidgetItem *ListWidgetItem_new(const char *text,unsigned char flags){
    ListWidgetItem *lwi=Far_calloc(1,sizeof(ListWidgetItem));
    ListWidgetItem_setText(lwi,text);
    ListWidgetItem_setFlags(lwi,flags);

    return lwi;
}

void ListWidgetItem_delete(ListWidgetItem *lwi){
    Far_free(lwi->text);
    Far_free(lwi);
}

void ListWidgetItem_setText(ListWidgetItem *lwi,const char *text){
    Far_free(lwi->text);
    lwi->text=Far_malloc(strlen(text)+1+2);
    strcpy(lwi->text+2,text);
}

void ListWidgetItem_setFlags(ListWidgetItem *lwi,unsigned char flags){
    lwi->flags=flags;
}

char *ListWidgetItem_text(ListWidgetItem *lwi){
    return lwi->text+2;
}

unsigned char ListWidgetItem_flags(ListWidgetItem *lwi){
    return lwi->flags;
}

bit ListWidgetItem_isSelectable(ListWidgetItem *lwi){
    return lwi->flags&ITEM_SELECTABLE;
}

bit ListWidgetItem_isSelected(ListWidgetItem *lwi){
    return lwi->flags&ITEM_SELECTED;
}

bit ListWidgetItem_isCheckable(ListWidgetItem *lwi){
    return lwi->flags&ITEM_CHECKABLE;
}

bit ListWidgetItem_isChecked(ListWidgetItem *lwi){
    return lwi->flags&ITEM_CHECKED;
}

bit ListWidgetItem_isEnabled(ListWidgetItem *lwi){
    return lwi->flags&ITEM_ENABLED;
}

void ListWidgetItem_setSelectable(ListWidgetItem *lwi,unsigned char selectable){
    if(selectable){
        lwi->flags|=ITEM_SELECTABLE;
    }else{
        lwi->flags&=~ITEM_SELECTABLE;
    }
}

void ListWidgetItem_setSelected(ListWidgetItem *lwi,unsigned char selected){
    if(selected){
        lwi->flags|=ITEM_SELECTED;
    }else{
        lwi->flags&=~ITEM_SELECTED;
    }
}

void ListWidgetItem_setCheckable(ListWidgetItem *lwi,unsigned char checkable){
    if(checkable){
        lwi->flags|=ITEM_CHECKABLE;
    }else{
        lwi->flags&=~ITEM_CHECKABLE;
    }
}

void ListWidgetItem_setChecked(ListWidgetItem *lwi,unsigned char checked){
    if(checked){
        lwi->flags|=ITEM_CHECKED;
    }else{
        lwi->flags&=~ITEM_CHECKED;
    }
}

void ListWidgetItem_setEnabled(ListWidgetItem *lwi,unsigned char enabled){
    if(enabled){
        lwi->flags|=ITEM_ENABLED;
    }else{
        lwi->flags&=~ITEM_ENABLED;
    }
}

bit ListWidgetItem_select(ListWidgetItem *lwi){
    if(ListWidgetItem_isSelectable(lwi)){
        ListWidgetItem_setSelected(lwi,(unsigned char)!ListWidgetItem_isSelected(lwi));
        return ListWidgetItem_isSelected(lwi);
    }

    return 0;
}

bit ListWidgetItem_check(ListWidgetItem *lwi){
    if(ListWidgetItem_isCheckable(lwi)){
        ListWidgetItem_setChecked(lwi,(unsigned char)!ListWidgetItem_isChecked(lwi));
        return ListWidgetItem_isChecked(lwi);
    }

    return 0;
}

void ListWidgetItem_show(ListWidgetItem *lwi,unsigned char row){
    LCD_setString(row,0,"                      ");

    if(lwi!=NULL&&ListWidgetItem_isEnabled(lwi)){
        refreshText(lwi);
        LCD_setString(row,0,lwi->text);
    }
}

static void refreshText(ListWidgetItem *lwi){
    if(lwi->flags&ITEM_SELECTABLE){
        if(lwi->flags&ITEM_SELECTED){
            lwi->text[0]=(char)0x10;
        }else{
            lwi->text[0]=(char)0x20;
        }
    }else{
        if(lwi->flags&ITEM_SELECTED){
            lwi->text[0]=(char)0xf9;
        }else{
            lwi->text[0]=(char)0xfa;
        }
    }

    if(lwi->flags&ITEM_CHECKABLE){
        lwi->text[1]=(char)0x09;

        if(lwi->flags&ITEM_CHECKED){
            lwi->text[1]=(char)0xfe;
        }
    }else{
        lwi->text[1]=(char)0x20;
    }
}
