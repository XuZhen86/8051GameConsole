#ifndef LISTWIDGET_H_
#define LISTWIDGET_H_

#include<ListWidgetItem.h>
#include<Vector.h>

typedef struct ListWidget_{
    char *title;
    Vector *items;
    unsigned int selected,itemShowStart;
} ListWidget;

ListWidget *ListWidget_new();
void ListWidget_delete(ListWidget *lw);

void ListWidget_setTitle(ListWidget *lw,char *title);
void ListWidget_addItem(ListWidget *lw,ListWidgetItem *item);
void ListWidget_insertItem(ListWidget *lw,unsigned int row,ListWidgetItem *item);
void ListWidget_enableAll(ListWidget *lw);

ListWidgetItem *ListWidget_item(ListWidget *lw,unsigned int row);
ListWidgetItem *ListWidget_currrentItem(ListWidget *lw);
unsigned int ListWidget_count(ListWidget *lw);

unsigned int ListWidget_getSelection(ListWidget *lw);

#endif
