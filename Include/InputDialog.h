#ifndef INPUTDIALOG_H_
#define INPUTDIALOG_H_

#include<VectorInt.h>

typedef struct InputDialog_{
    char *title;
    VectorInt *values;
    unsigned int selected;
    void (*sigValueChanged)(int);
    void (*sigSelectionChanged)(unsigned int);
} InputDialog;

InputDialog *InputDialog_new();
void InputDialog_delete(InputDialog *id);
unsigned int InputDialog_getSelection(InputDialog *id);

void InputDialog_setTitle(InputDialog *id,const char *title);
void InputDialog_setValues(InputDialog *id,VectorInt *values);
void InputDialog_setSelected(InputDialog *id,unsigned int selected);
void InputDialog_setSigValueChanged(InputDialog *id,void (*sigValueChanged)(int));
void InputDialog_setSigSelectionChanged(InputDialog *id,void (*sigSelectionChanged)(unsigned int));

void InputDialog_test();

#endif
