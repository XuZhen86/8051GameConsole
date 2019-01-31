#include"InputDialogConfig.h"
#include"InputDialogStatic.h"
#include<Debug.h>
#include<Far.h>
#include<InputDialog.h>
#include<LCD.h>
#include<Pushbutton.h>
#include<stdio.h>
#include<string.h>

InputDialog *InputDialog_new(){
    InputDialog *id=fcalloc(1,sizeof(InputDialog));
    return id;
}

void InputDialog_delete(InputDialog *id){
    ffree(id->title);
    ffree(id);
}

unsigned int InputDialog_getSelection(InputDialog *id){
    unsigned char i;

    LCD_setString(2,2,"                 ");
    LCD_setString(3,2,"                 ");
    LCD_setString(4,2,"                 ");
    LCD_setString(2,(21-strlen(id->title))/2,id->title);

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

    print(id);

    while(1){
        LCD_flush();

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

        if(id->sigValueChanged!=NULL){
            id->sigValueChanged(VectorInt_get(id->values,id->selected));
        }
        if(id->sigSelectionChanged!=NULL){
            id->sigSelectionChanged(id->selected);
        }
    }
}

void InputDialog_setTitle(InputDialog *id,const char *title){
    ffree(id->title);
    id->title=fmalloc(strlen(title)+1);
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

    bufferLen=sprintf(buffer," [ %d ]  ",VectorInt_get(id->values,id->selected));

    if(id->selected==0){
        buffer[bufferLen-2]=POINTER_CHAR_UP;
    }else if(id->selected==VectorInt_size(id->values)-1){
        buffer[bufferLen-2]=POINTER_CHAR_DOWN;
    }else{
        buffer[bufferLen-2]=POINTER_CHAR_UP_DOWN;
    }

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
