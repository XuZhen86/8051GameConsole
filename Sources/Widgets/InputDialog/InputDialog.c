#include"Sources/Widgets/InputDialog/InputDialog.h"
#include"Sources/LCD/LCD.h"
#include"Sources/Pushbutton/Pushbutton.h"

#include<stdio.h>
#include<string.h>

enum INPUT_DIALOG{
    BOARDER_SIZE=6,
    BUFFER_SIZE=20,
    POINTER_CHAR_UP_DOWN=0x12,
    POINTER_CHAR_UP=0x18,
    POINTER_CHAR_DOWN=0x19
};

unsigned char inputDialog_getUChar(unsigned char *title,unsigned char value,unsigned char min,unsigned char max,unsigned char step,void (*sigUCharValueChanged)(unsigned char),bit saveBuffer){
    unsigned char i;
    bit answerDetermined=0;

    if(saveBuffer){
        lcd_bufferStackPush();
    }

    lcd_stringSet(2,2,"                 ");
    lcd_stringSet(3,2,"                 ");
    lcd_stringSet(4,2,"                 ");
    lcd_stringSet(2,(21-strlen(title))/2,title);

    for(i=0;i<BOARDER_SIZE;i++){
        lcd_pixelSet(15,15+i,1);
        lcd_pixelSet(15+i,15,1);
        lcd_pixelSet(15,113-i,1);
        lcd_pixelSet(15+i,113,1);
        lcd_pixelSet(40,15+i,1);
        lcd_pixelSet(40-i,15,1);
        lcd_pixelSet(40,113-i,1);
        lcd_pixelSet(40-i,113,1);
    }

    inputDialog_getUChar_printValue(value,min,max);
    while(lcd_flush(0)&&!answerDetermined){
        switch(pushbutton_waitDirectionGet()){
            case PUSHBUTTON_DIRECTION_UP:
                value+=step;
                if(value>=max||value<min){
                    value=max-1;
                }
                break;
            case PUSHBUTTON_DIRECTION_DOWN:
                value-=step;
                if(value<min||value>=max){
                    value=min;
                }
                break;
            case PUSHBUTTON_DIRECTION_FORWARD:
                answerDetermined=1;
                break;
            case PUSHBUTTON_DIRECTION_BACK:
                value=max;
                answerDetermined=1;
                break;
        }
        inputDialog_getUChar_printValue(value,min,max);

        pushbutton_waitDirectionRelease();
        if(sigUCharValueChanged!=NULL){
            sigUCharValueChanged(value);
        }
    }

    if(saveBuffer){
        lcd_bufferStackPop();
    }
    return value;
}

void inputDialog_getUChar_printValue(unsigned char value,unsigned char min,unsigned char max){
    unsigned char bufferLen;
    unsigned char buffer[BUFFER_SIZE];

    bufferLen=sprintf(buffer," [ %bu ]  ",value);

    if(min==value){
        buffer[bufferLen-2]=POINTER_CHAR_UP;
    }else if(value==max-1){
        buffer[bufferLen-2]=POINTER_CHAR_DOWN;
    }else{
        buffer[bufferLen-2]=POINTER_CHAR_UP_DOWN;
    }

    lcd_stringSet(4,(22-bufferLen)/2,buffer);
}
