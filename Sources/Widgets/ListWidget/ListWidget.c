#include"Sources/Widgets/ListWidget/ListWidget.h"
#include"Sources/LCD12864/LCD12864.h"
#include"Sources/Pushbutton/Pushbutton.h"
#include"Sources/Universal/Universal.h"
#include"Sources/Clock/Clock.h"

#include<string.h>

enum{
    BUFFER_SIZE=25,
    POINTER_CHAR_LEFT=0x11,
    POINTER_CHAR_RIGHT=0x10
};

unsigned char listWidget_selectFromList(unsigned char code *title,unsigned char code *items[],unsigned char itemCount,void (*sigCurrentItemChanged)(unsigned char),bit saveBuffer){
    unsigned char i,selectedRow=0,selectedItem=0;
    unsigned char buffer[BUFFER_SIZE];

    if(saveBuffer){
        lcd12864_bufferStackPush();
    }
    lcd12864_clear();

    lcd12864_charSet(0,0,POINTER_CHAR_LEFT);
    lcd12864_stringSet(0,(21-strlen(title))/2,title);
    lcd12864_hLineSet(7,1);

    for(i=0;i<itemCount&&i<7;i++){
        lcd12864_stringSet(i+1,2,listWidget_strcpySpaceExtend(buffer,items[i]));
    }

    lcd12864_charSet(selectedRow+1,0,POINTER_CHAR_RIGHT);

    while(lcd12864_flush(0)){
        switch(pushbutton_waitDirectionGet()){
            case PUSHBUTTON_DIRECTION_UP:
                if(selectedItem){   // If not reaching top of the list
                    if(selectedRow){    // If not reaching top of the display
                        lcd12864_charSet(selectedRow+1,0,' ');
                        lcd12864_charSet(--selectedRow+1,0,POINTER_CHAR_RIGHT);
                    }else{  // Reached top of the display, move items down one slot
                        for(i=selectedItem-1;i<selectedItem+6&&i<itemCount;i++){
                            lcd12864_stringSet(i-selectedItem+2,2,listWidget_strcpySpaceExtend(buffer,items[i]));
                        }
                    }
                    selectedItem--;
                }else{  // Reached top of the list
                    lcd12864_charSet(selectedRow+1,0,' ');
                    if(itemCount<7){    // If have less than 7 items, no need to redraw list
                        lcd12864_charSet((selectedItem=selectedRow=itemCount-1)+1,0,POINTER_CHAR_RIGHT);
                    }else{  // Have more than 7 items, redraw list
                        lcd12864_charSet((selectedRow=6)+1,0,POINTER_CHAR_RIGHT);
                        selectedItem=itemCount-1;
                        for(i=itemCount-7;i<itemCount;i++){
                            lcd12864_stringSet(i-(itemCount-8),2,listWidget_strcpySpaceExtend(buffer,items[i]));
                        }
                    }
                }
                break;
            case PUSHBUTTON_DIRECTION_DOWN:
                if(selectedItem!=itemCount-1){  // If not reaching end of the list
                    if(selectedRow!=6){ // If not reaching end of the display
                        lcd12864_charSet(selectedRow+1,0,' ');
                        lcd12864_charSet(++selectedRow+1,0,POINTER_CHAR_RIGHT);
                    }else{  // Reached end of the display, move items up one slot
                        for(i=selectedItem-5;i<selectedItem+2&&i<itemCount;i++){
                            lcd12864_stringSet(i-selectedItem+6,2,listWidget_strcpySpaceExtend(buffer,items[i]));
                        }
                    }
                    selectedItem++;
                }else{  // Reached end of the list
                    lcd12864_charSet(selectedRow+1,0,' ');
                    lcd12864_charSet((selectedRow=0)+1,0,POINTER_CHAR_RIGHT);
                    for(i=0;i<itemCount&&i<7;i++){
                        lcd12864_stringSet(i+1,2,listWidget_strcpySpaceExtend(buffer,items[i]));
                    }
                    selectedItem=0;
                }
                break;
            case PUSHBUTTON_DIRECTION_FORWARD:  // Confirm selection
                if(saveBuffer){
                    lcd12864_bufferStackPop();
                }
                pushbutton_waitDirectionRelease();
                lcd12864_flush(0);
                return selectedItem;
            case PUSHBUTTON_DIRECTION_BACK: // Cancel selection
                if(saveBuffer){
                    lcd12864_bufferStackPop();
                }
                pushbutton_waitDirectionRelease();
                lcd12864_flush(0);
                return itemCount;
        }

        if(sigCurrentItemChanged!=NULL){
            sigCurrentItemChanged(selectedItem);
        }

        pushbutton_waitDirectionRelease();
    }

    // This part should never be reached
    if(saveBuffer){
        lcd12864_bufferStackPop();
    }
    lcd12864_flush(0);
    return itemCount;
}

unsigned char *listWidget_strcpySpaceExtend(unsigned char *destination,unsigned char *source){
    unsigned char i;
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
