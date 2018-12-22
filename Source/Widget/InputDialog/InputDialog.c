#include"InputDialog.h"
#include"InputDialogConfig.h"
#include"InputDialogStatic.h"
#include<LCD.h>
#include<Pushbutton.h>
#include<stdio.h>
#include<string.h>

unsigned char InputDialog_getUChar(unsigned char *title,unsigned char value,unsigned char min,unsigned char max,unsigned char step,void (*sigUCharValueChanged)(unsigned char)){
    unsigned char i;
    unsigned char answerDetermined=0;

    LCD_setString(2,2,"                 ");
    LCD_setString(3,2,"                 ");
    LCD_setString(4,2,"                 ");
    LCD_setString(2,(21-strlen(title))/2,title);

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

    printValue(value,min,max);
    while(!answerDetermined){
        LCD_flush();
        switch(Pushbutton_getDirectionWait()){
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
        printValue(value,min,max);

        Pushbutton_directionReleaseWait();
        if(sigUCharValueChanged!=NULL){
            sigUCharValueChanged(value);
        }
    }

    return value;
}

static void printValue(unsigned char value,unsigned char min,unsigned char max){
    unsigned char bufferLen;
    unsigned char xdata buffer[BUFFER_SIZE];

    bufferLen=sprintf(buffer," [ %bu ]  ",value);

    if(min==value){
        buffer[bufferLen-2]=POINTER_CHAR_UP;
    }else if(value==max-1){
        buffer[bufferLen-2]=POINTER_CHAR_DOWN;
    }else{
        buffer[bufferLen-2]=POINTER_CHAR_UP_DOWN;
    }

    LCD_setString(4,(22-bufferLen)/2,buffer);
}
