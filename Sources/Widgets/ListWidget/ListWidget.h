#ifndef LISTWIDGET_H_
#define LISTWIDGET_H_

typedef struct ListWidget_{
    char *title;
    char *items[32];
    char count;
    void (*sigCurrentItemChanged)(unsigned char);
} ListWidget;

ListWidget *ListWidget_new();
void ListWidget_delete(ListWidget *lw);
unsigned char ListWidget_getSelection(ListWidget *lw);

void ListWidget_setTitle(ListWidget *lw,char *title);
void ListWidget_addItem(ListWidget *lw,char *item);
void ListWidget_setSigCurrentItemChanged(ListWidget *lw,void (*sigCurrentItemChanged)(unsigned char));

void ListWidget_print(ListWidget *lw);
static void strcpySpaceExtend(unsigned char *dst,unsigned char *src);

#endif
