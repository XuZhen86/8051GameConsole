#ifndef LISTWIDGETITEM_H_
#define LISTWIDGETITEM_H_

enum LISTWIDGETITEM_FLAGS{
    ITEM_SELECTABLE=0x01,
    ITEM_SELECTED=0x02,
    ITEM_CHECKABLE=0x04,
    ITEM_CHECKED=0x08,
    ITEM_ENABLED=0x10,
};

typedef struct ListWidgetItem_{
    char *text;
    unsigned char flags;
} ListWidgetItem;

// Allocate ListWidgetItem object
ListWidgetItem *ListWidgetItem_new(const char *text,unsigned char flags);

// Deallocate ListWidgetItem object
void ListWidgetItem_delete(ListWidgetItem *lwi);

// Set text
void ListWidgetItem_setText(ListWidgetItem *lwi,const char *text);

// Set flags
void ListWidgetItem_setFlags(ListWidgetItem *lwi,unsigned char flags);

// Get text
char *ListWidgetItem_text(ListWidgetItem *lwi);

// Get flags
unsigned char ListWidgetItem_flags(ListWidgetItem *lwi);

// Check flags
bit ListWidgetItem_isSelectable(ListWidgetItem *lwi);
bit ListWidgetItem_isSelected(ListWidgetItem *lwi);
bit ListWidgetItem_isCheckable(ListWidgetItem *lwi);
bit ListWidgetItem_isChecked(ListWidgetItem *lwi);
bit ListWidgetItem_isEnabled(ListWidgetItem *lwi);

// Set flags
void ListWidgetItem_setSelectable(ListWidgetItem *lwi,unsigned char selectable);
void ListWidgetItem_setSelected(ListWidgetItem *lwi,unsigned char selected);
void ListWidgetItem_setCheckable(ListWidgetItem *lwi,unsigned char checkable);
void ListWidgetItem_setChecked(ListWidgetItem *lwi,unsigned char checked);
void ListWidgetItem_setEnabled(ListWidgetItem *lwi,unsigned char enabled);

// Toggle flags
bit ListWidgetItem_select(ListWidgetItem *lwi);
bit ListWidgetItem_check(ListWidgetItem *lwi);

// Print itself on screen
void ListWidgetItem_show(ListWidgetItem *lwi,unsigned char row);

#endif
