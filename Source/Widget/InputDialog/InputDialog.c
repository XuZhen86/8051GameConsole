#include"InputDialogConfig.h"
#include"InputDialogStatic.h"
#include<Debug.h>
#include<InputDialog.h>
#include<LCD.h>
#include<Pushbutton.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

InputDialog *InputDialog_new(){
    InputDialog *id=calloc(1,sizeof(InputDialog));
    return id;
}

void InputDialog_delete(InputDialog *id){
    free(id->title);
    free(id);
}

unsigned int InputDialog_getSelection(InputDialog *id){
    unsigned char i;

    // Clear out space
    LCD_setString(2,2,"                 ");
    LCD_setString(3,2,"                 ");
    LCD_setString(4,2,"                 ");

    // Print title
    LCD_setString(2,(21-strlen(id->title))/2,id->title);

    // Draw boarder at angles
    for(i=0;i<BOARDER_SIZE;i++){
        LCD_setPixel(15,15+i,1);
        LCD_setPixel(15+i,15,1);
        LCD_setPixel(15,113-i,1);
        LCD_setPixel(15+i,113,1);
        LCD_setPixel(40,15+i,1);
        LCD_setPixel(40-i,15,1);
        LCD_setPixel(40,113-i,1);
        LCD_setPixel(40-i,113,1);
    }

    // Print current selected
    print(id);

    while(1){
        LCD_flush();

        // Get button and change selection
        switch(Pushbutton_getDirectionWait()){
            case PUSHBUTTON_DIRECTION_UP:
                id->selected++;
                if(id->selected==VectorInt_size(id->values)){
                    id->selected=VectorInt_size(id->values)-1;
                }
                break;
            case PUSHBUTTON_DIRECTION_DOWN:
                if(id->selected){
                    id->selected--;
                }
                break;
            case PUSHBUTTON_DIRECTION_FORWARD:
                Pushbutton_directionReleaseWait();
                return id->selected;
            case PUSHBUTTON_DIRECTION_BACK:
                Pushbutton_directionReleaseWait();
                return VectorInt_size(id->values);
            default:
                Debug(WARNING,"Unexpected button direction");
        }

        print(id);
        Pushbutton_directionReleaseWait();

        // Execute signal functions if specified
        // Example: brightness changes with value even before user confirms selection
        if(id->sigValueChanged!=NULL){
            id->sigValueChanged(VectorInt_get(id->values,id->selected));
        }
        if(id->sigSelectionChanged!=NULL){
            id->sigSelectionChanged(id->selected);
        }
    }
}

void InputDialog_setTitle(InputDialog *id,const char *title){
    free(id->title);
    id->title=malloc(strlen(title)+1);
    strcpy(id->title,title);
}

void InputDialog_setValues(InputDialog *id,VectorInt *values){
    id->values=values;
}

void InputDialog_setSelected(InputDialog *id,unsigned int selected){
    id->selected=selected;
}

void InputDialog_setSigValueChanged(InputDialog *id,void (*sigValueChanged)(int)){
    id->sigValueChanged=sigValueChanged;
}

void InputDialog_setSigSelectionChanged(InputDialog *id,void (*sigSelectionChanged)(unsigned int)){
    id->sigSelectionChanged=sigSelectionChanged;
}

static void print(InputDialog *id){
    unsigned char bufferLen;
    char buffer[BUFFER_SIZE];

    // Print value
    bufferLen=sprintf(buffer," [ %d ]  ",VectorInt_get(id->values,id->selected));

    // Print arrow after value
    if(id->selected==0){
        buffer[bufferLen-2]=POINTER_CHAR_UP;
    }else if(id->selected==VectorInt_size(id->values)-1){
        buffer[bufferLen-2]=POINTER_CHAR_DOWN;
    }else{
        buffer[bufferLen-2]=POINTER_CHAR_UP_DOWN;
    }

    // Show value
    LCD_setString(4,(22-bufferLen)/2,buffer);
}

void InputDialog_test(){
    VectorInt *vi=VectorInt_new();
    InputDialog *id=InputDialog_new();
    int i;

    for(i=0;i<10;i++){
        VectorInt_add(vi,i);
    }

    InputDialog_setTitle(id,"InputDialog_test");
    InputDialog_setValues(id,vi);
    InputDialog_setSelected(id,0);
    InputDialog_getSelection(id);
}
