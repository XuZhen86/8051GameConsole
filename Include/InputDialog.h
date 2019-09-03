#ifndef INPUTDIALOG_H_
#define INPUTDIALOG_H_

#include<VectorInt.h>

typedef struct InputDialog_{
    char *title;
    VectorInt *values;
    unsigned int selected;

    // Optional signal functions to call when things changes
    void (*sigValueChanged)(int);
    void (*sigSelectionChanged)(unsigned int);
} InputDialog;

// Allocate InputDialog object
InputDialog *InputDialog_new();

// Deallocate InputDialog object
void InputDialog_delete(InputDialog *id);

// Prompt user on GUI to make a selection
unsigned int InputDialog_getSelection(InputDialog *id);

// Set title
void InputDialog_setTitle(InputDialog *id,const char *title);

// Set candidate values
void InputDialog_setValues(InputDialog *id,VectorInt *values);

// Set currently selected candidate
void InputDialog_setSelected(InputDialog *id,unsigned int selected);

// Set signal functions
void InputDialog_setSigValueChanged(InputDialog *id,void (*sigValueChanged)(int));
void InputDialog_setSigSelectionChanged(InputDialog *id,void (*sigSelectionChanged)(unsigned int));

// Show a default InputDialog for testing
void InputDialog_test();

#endif
