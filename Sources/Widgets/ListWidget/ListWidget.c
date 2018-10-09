#include"Sources/Widgets/ListWidget/ListWidget.h"
#include"Sources/LCD12864/LCD12864.h"
#include"Sources/Pushbutton/Pushbutton.h"
#include"Sources/Universal/Universal.h"
#include"Sources/Universal/SystemClock.h"

#include<string.h>

enum{
    BUFFER_SIZE=25
};

unsigned char listWidget_selectFromList(unsigned char code *title,unsigned char code *items[],unsigned char itemCount,bit saveBuffer){
    unsigned char data i,selectedRow=0,selectedItem=0;
    unsigned char buffer[BUFFER_SIZE];

    if(saveBuffer){
        lcd12864_bufferStackPush();
    }
    lcd12864_clear();

    lcd12864_charSet(0,0,'<');
    lcd12864_stringSet(0,(21-strlen(title))/2,title);
    lcd12864_hLineSet(7,1);

    for(i=0;i<itemCount&&i<7;i++){
        lcd12864_stringSet(i+1,2,listWidget_strcpySpaceExtend(buffer,items[i]));
    }

    lcd12864_charSet(selectedRow+1,0,'>');

    while(lcd12864_flush(0)){
        switch(pushbutton_waitDirectionGet()){
            case PUSHBUTTON_DIRECTION_UP:
                if(selectedItem){
                    if(selectedRow){
                        lcd12864_charSet(selectedRow+1,0,' ');
                        lcd12864_charSet(--selectedRow+1,0,'>');
                    }else{
                        for(i=selectedItem-1;i<selectedItem+6&&i<itemCount;i++){
                            lcd12864_stringSet(i-selectedItem+2,2,listWidget_strcpySpaceExtend(buffer,items[i]));
                        }
                    }
                    selectedItem--;
                }else{
                    lcd12864_charSet(selectedRow+1,0,' ');
                    if(itemCount<7){
                        lcd12864_charSet((selectedItem=selectedRow=itemCount-1)+1,0,'>');
                    }else{
                        lcd12864_charSet((selectedRow=6)+1,0,'>');
                        selectedItem=itemCount-1;
                        for(i=itemCount-7;i<itemCount;i++){
                            lcd12864_stringSet(i-(itemCount-8),2,listWidget_strcpySpaceExtend(buffer,items[i]));
                        }
                    }
                }
                break;
            case PUSHBUTTON_DIRECTION_DOWN:
                if(selectedItem!=itemCount-1){
                    if(selectedRow!=6){
                        lcd12864_charSet(selectedRow+1,0,' ');
                        lcd12864_charSet(++selectedRow+1,0,'>');
                    }else{
                        for(i=selectedItem-5;i<selectedItem+2&&i<itemCount;i++){
                            lcd12864_stringSet(i-selectedItem+6,2,listWidget_strcpySpaceExtend(buffer,items[i]));
                        }
                    }
                    selectedItem++;
                }else{
                    lcd12864_charSet(selectedRow+1,0,' ');
                    lcd12864_charSet((selectedRow=0)+1,0,'>');
                    for(i=0;i<itemCount&&i<7;i++){
                        lcd12864_stringSet(i+1,2,listWidget_strcpySpaceExtend(buffer,items[i]));
                    }
                    selectedItem=0;
                }
                break;
            case PUSHBUTTON_DIRECTION_FORWARD:
                if(saveBuffer){
                    lcd12864_bufferStackPop();
                }
                pushbutton_waitDirectionRelease();
                lcd12864_flush(0);
                return selectedItem;
            case PUSHBUTTON_DIRECTION_BACK:
                if(saveBuffer){
                    lcd12864_bufferStackPop();
                }
                pushbutton_waitDirectionRelease();
                lcd12864_flush(0);
                return itemCount;
        }

        pushbutton_waitDirectionRelease();
    }

    if(saveBuffer){
        lcd12864_bufferStackPop();
    }
    lcd12864_flush(0);
    return itemCount;
}

unsigned char *listWidget_strcpySpaceExtend(unsigned char *destination,unsigned char *source){
    unsigned char data i;
    bit spaceExt=0;

    for(i=0;i<BUFFER_SIZE-1;i++){
        if(spaceExt){
            destination[i]=' ';
        }else{
            if(source[i]!=0){
                destination[i]=source[i];
            }else{
                spaceExt=1;
                destination[i]=' ';
            }
        }
    }
    destination[BUFFER_SIZE-1]=0;

    return destination;
}