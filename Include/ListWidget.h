#ifndef LISTWIDGET_H_
#define LISTWIDGET_H_

#include<ListWidgetItem.h>
#include<Vector.h>

typedef struct ListWidget_{
    char *title;
    Vector *items;
    unsigned int selected,itemShowStart;
} ListWidget;

// Allocate ListWidget object
ListWidget *ListWidget_new();

// Deallocate ListWidget object
void ListWidget_delete(ListWidget *lw);

// Set title
void ListWidget_setTitle(ListWidget *lw,const char *title);

// Add item
void ListWidget_addItem(ListWidget *lw,ListWidgetItem *item);

// Insert item
void ListWidget_insertItem(ListWidget *lw,unsigned int row,ListWidgetItem *item);

// enable all items
void ListWidget_enableAll(ListWidget *lw);

// Get item
ListWidgetItem *ListWidget_item(ListWidget *lw,unsigned int row);

// Get current item
ListWidgetItem *ListWidget_currrentItem(ListWidget *lw);

// Get number of items
unsigned int ListWidget_count(ListWidget *lw);

// Prompt user on GUI to make a selction
unsigned int ListWidget_getSelection(ListWidget *lw);

#endif
