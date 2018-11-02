#include"Sources/Widgets/ListWidget/ListWidget.h"
#include"Sources/LCD/LCD.h"
#include"Sources/Pushbutton/Pushbutton.h"
#include"Sources/Universal/Universal.h"
#include"Sources/Clock/Clock.h"

#include<string.h>

enum{
    BUFFER_SIZE=20,
    POINTER_CHAR_LEFT=0x11,
    POINTER_CHAR_RIGHT=0x10
};

unsigned char listWidget_selectFromList(unsigned char code *title,unsigned char code *items[],unsigned char itemCount,void (*sigCurrentItemChanged)(unsigned char),bit saveBuffer){
    unsigned char i,selectedRow=0,selectedItem=0;
    unsigned char buffer[BUFFER_SIZE];

    if(saveBuffer){
        lcd_bufferStackPush();
    }
    lcd_clear();

    lcd_charSet(0,0,POINTER_CHAR_LEFT);
    lcd_stringSet(0,(21-strlen(title))/2,title);
    lcd_hLineSet(7,1);

    for(i=0;i<itemCount&&i<7;i++){
        lcd_stringSet(i+1,2,listWidget_strcpySpaceExtend(buffer,items[i]));
    }

    lcd_charSet(selectedRow+1,0,POINTER_CHAR_RIGHT);

    while(1){
        lcd_flush();

        switch(pushbutton_waitDirectionGet()){
            case PUSHBUTTON_DIRECTION_UP:
                if(selectedItem){   // If not reaching top of the list
                    if(selectedRow){    // If not reaching top of the display
                        lcd_charSet(selectedRow+1,0,' ');
                        lcd_charSet(--selectedRow+1,0,POINTER_CHAR_RIGHT);
                    }else{  // Reached top of the display, move items down one slot
                        for(i=selectedItem-1;i<selectedItem+6&&i<itemCount;i++){
                            lcd_stringSet(i-selectedItem+2,2,listWidget_strcpySpaceExtend(buffer,items[i]));
                        }
                    }
                    selectedItem--;
                }else{  // Reached top of the list
                    lcd_charSet(selectedRow+1,0,' ');
                    if(itemCount<7){    // If have less than 7 items, no need to redraw list
                        lcd_charSet((selectedItem=selectedRow=itemCount-1)+1,0,POINTER_CHAR_RIGHT);
                    }else{  // Have more than 7 items, redraw list
                        lcd_charSet((selectedRow=6)+1,0,POINTER_CHAR_RIGHT);
                        selectedItem=itemCount-1;
                        for(i=itemCount-7;i<itemCount;i++){
                            lcd_stringSet(i-(itemCount-8),2,listWidget_strcpySpaceExtend(buffer,items[i]));
                        }
                    }
                }
                break;
            case PUSHBUTTON_DIRECTION_DOWN:
                if(selectedItem!=itemCount-1){  // If not reaching end of the list
                    if(selectedRow!=6){ // If not reaching end of the display
                        lcd_charSet(selectedRow+1,0,' ');
                        lcd_charSet(++selectedRow+1,0,POINTER_CHAR_RIGHT);
                    }else{  // Reached end of the display, move items up one slot
                        for(i=selectedItem-5;i<selectedItem+2&&i<itemCount;i++){
                            lcd_stringSet(i-selectedItem+6,2,listWidget_strcpySpaceExtend(buffer,items[i]));
                        }
                    }
                    selectedItem++;
                }else{  // Reached end of the list
                    lcd_charSet(selectedRow+1,0,' ');
                    lcd_charSet((selectedRow=0)+1,0,POINTER_CHAR_RIGHT);
                    for(i=0;i<itemCount&&i<7;i++){
                        lcd_stringSet(i+1,2,listWidget_strcpySpaceExtend(buffer,items[i]));
                    }
                    selectedItem=0;
                }
                break;
            case PUSHBUTTON_DIRECTION_FORWARD:  // Confirm selection
                if(saveBuffer){
                    lcd_bufferStackPop();
                }
                pushbutton_waitDirectionRelease();
                lcd_flush();
                return selectedItem;
            case PUSHBUTTON_DIRECTION_BACK: // Cancel selection
                if(saveBuffer){
                    lcd_bufferStackPop();
                }
                pushbutton_waitDirectionRelease();
                lcd_flush();
                return itemCount;
        }

        if(sigCurrentItemChanged!=NULL){
            sigCurrentItemChanged(selectedItem);
        }

        pushbutton_waitDirectionRelease();
    }
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
