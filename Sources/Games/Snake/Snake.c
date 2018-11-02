#include"Sources/Stack/Stack.h"
#include"Sources/Pushbutton/Pushbutton.h"
#include"Sources/LCD/LCD.h"
#include"Sources/Clock/Clock.h"
#include"Sources/Delay/Delay.h"

#include<stdio.h>
#include<stdlib.h>

#include"Sources/Games/Snake/Snake.h"
#include"Sources/Games/Snake/SnakeConfig.h"

enum SNAKE_STACK{
    map=0+2048,
    mapDirty=map+128,

    foodX,foodY,headX,headY,tailX,tailY,

    tick=tailY+2,
    level,
    direction,
    pressedDirection,
    length=pressedDirection+2,
    lengthHigh=length+2,
    refreshInterval=lengthHigh+2,
    tailVal=refreshInterval+2,

    STACK_SIZE
};

void snake(){
    enter(STACK_SIZE);
    snake_levelSelect();
    snake_gamePlay();

    pushbutton_waitDirectionGet();pushbutton_waitDirectionRelease();

    leave();
}

void snake_levelSelect(){
    unsigned char buffer[8],bufferLen;
    bit startGame=0;

    lcd_clear();
    lcd_stringSet(2,8,"Snake");
    lcd_stringSet(5,6,"Level:");

    snake_mapInitialize();
    w8(level,8);

    while(!startGame){
        w16(refreshInterval,LEVEL_DELAY*(LEVEL_MAX-r8(level))+LEVEL_BASE);

        bufferLen=sprintf(buffer,"%2bu ",r8(level));
        printf("[bufferLen=%bu]\n",bufferLen);
        if(r8(level)==LEVEL_MAX){
            buffer[bufferLen-1]=0x19;
        }else if(r8(level)==LEVEL_MIN){
            buffer[bufferLen-1]=0x18;
        }else{
            buffer[bufferLen-1]=0x12;
        }

        lcd_stringSet(5,12,buffer);
        lcd_flush();

        switch(pushbutton_waitDirectionGet()){
            case UP:
                if(LEVEL_MAX<inc8(level)){
                    w8(level,LEVEL_MAX);
                }
                break;
            case DOWN:
                if(dec8(level)<LEVEL_MIN){
                    w8(level,LEVEL_MIN);
                }
                break;
            case FORWARD:
                startGame=1;
                break;
            case BACK:
                return;
        }

        pushbutton_waitDirectionRelease();
    }

    w16(lengthHigh,15);

    srand(clock_get());
    snake_foodRenew();

    lcd_clear();
    snake_screenRenew();
}

void snake_gamePlay(){
    unsigned long int tickTock;

    lcd_stringSet(5,11,"High");
    lcd_stringSet(6,11,"Length");
    lcd_stringSet(7,11,"Ticks");

    while(1){
        clock_timerStart(r16(refreshInterval));

        w8(pressedDirection,r8(direction));
        while(!clock_timerIsTimeUp()){
            if(pushbutton_directionGet()!=INVALID){
                w8(pressedDirection,pushbutton_lastPressedDirectionGet());
            }
            delay(LEVEL_DELAY);
        }

        if(r8(pressedDirection)==BACK||r8(pressedDirection)==FORWARD){
            snake_gamePause();
            w8(pressedDirection,r8(direction));
        }

        tickTock=clock_get();

        if(r8(pressedDirection)!=r8(direction)){
            if(((r8(direction)==UP||r8(direction)==DOWN)&&(r8(pressedDirection)==LEFT||r8(pressedDirection)==RIGHT))||((r8(direction)==LEFT||r8(direction)==RIGHT)&&(r8(pressedDirection)==UP||r8(pressedDirection)==DOWN))){
                w8(direction,r8(pressedDirection));
            }
        }

        switch(r8(direction)){
            case UP:
                dec8(headX);
                break;
            case DOWN:
                inc8(headX);
                break;
            case LEFT:
                dec8(headY);
                break;
            case RIGHT:
                inc8(headY);
                break;
        }

        if(snake_mapGet(r8(headX),r8(headY))!=0){
            break;
        }

        snake_mapSet(r8(headX),r8(headY),inc16(tick));
        if(r16(tick)==65534){
            snake_tickRestart();
        }

        if(r8(headX)!=r8(foodX)||r8(headY)!=r8(foodY)){
            w16(tailVal,snake_mapGet(r8(tailX),r8(tailY))+1);

            if(snake_mapGet(r8(tailX)-1,r8(tailY))==r16(tailVal)){
                snake_mapSet(r8(tailX),r8(tailY),0);
                dec8(tailX);
            }else if(snake_mapGet(r8(tailX)+1,r8(tailY))==r16(tailVal)){
                snake_mapSet(r8(tailX),r8(tailY),0);
                inc8(tailX);
            }else if(snake_mapGet(r8(tailX),r8(tailY)-1)==r16(tailVal)){
                snake_mapSet(r8(tailX),r8(tailY),0);
                dec8(tailY);
            }else if(snake_mapGet(r8(tailX),r8(tailY)+1)==r16(tailVal)){
                snake_mapSet(r8(tailX),r8(tailY),0);
                inc8(tailY);
            }else{
                puts("SNAKE_EXIT_CODE_UNEXPECTED_TAIL_DATA");
                return;
            }
        }else{
            inc16(length);
            if(r16(length)>r16(lengthHigh)){
                w16(lengthHigh,r16(length));
            }
            snake_foodRenew();
        }

        snake_screenRenew();
    }

    do{
        pushbutton_waitDirectionGet();
        pushbutton_waitDirectionRelease();
    }while(pushbutton_lastPressedDirectionGet()!=FORWARD&&pushbutton_lastPressedDirectionGet()!=BACK);
}

void snake_gamePause(){
    unsigned char i,j;

    lcd_bufferStackPush();
    for(i=2;i<62;i+=2){
        for(j=2;j<62;j+=2){
            lcd_pixelSet(i,j,0);
            lcd_pixelSet(i+1,j,0);
            lcd_pixelSet(i,j+1,0);
            lcd_pixelSet(i+1,j+1,0);
        }
    }
    lcd_stringSet(3,2,"Paused");
    lcd_flush();

    do{
        pushbutton_waitDirectionGet();
        pushbutton_waitDirectionRelease();
    }while(pushbutton_lastPressedDirectionGet()!=FORWARD&&pushbutton_lastPressedDirectionGet()!=BACK);

    lcd_bufferStackPop();
    lcd_flush();
}

void snake_mapInitialize(){
    unsigned char i,j;

    for(i=1;i<31;i++){
        for(j=1;j<31;j++){
            snake_mapSet(i,j,0);
        }
    }

    for(i=0;i<32;i++){
        snake_mapSet(0,i,0xffff);
        snake_mapSet(31,i,0xffff);
        snake_mapSet(i,0,0xffff);
        snake_mapSet(i,31,0xffff);
    }

    w8(headX,15);
    w8(headY,15);
    w8(tailX,15);
    w8(tailY,19);
    w16(tick,0);
    for(i=r8(tailY);i>=r8(headY);i--){
        snake_mapSet(r8(headX),i,inc16(tick));
    }
    w16(length,r16(tick));

    w8(direction,LEFT);
}

unsigned int snake_mapGet(unsigned char x,unsigned char y){
    return r16a(map,x%32*32+y%32);
}

unsigned int snake_mapSet(unsigned char x,unsigned char y,unsigned int val){
    x%=32;
    y%=32;
    w8a(mapDirty,x*4+y/8,r8a(mapDirty,x*4+y/8)|(1<<(y%8)));
    return w16a(map,x*32+y,val);
}

void snake_tickRestart(){
    unsigned int dTick=snake_mapGet(r8(tailX),r8(tailY));
    unsigned char i,j;

    for(i=1;i<31;i++){
        for(j=1;j<31;j++){
            if(snake_mapGet(i,j)!=0){
                snake_mapSet(i,j,snake_mapGet(i,j)-dTick);
            }
        }
    }

    w16(tick,r16(tick)-dTick);
}

void snake_foodRenew(){
    while(snake_mapGet(w8(foodX,(unsigned int)rand()%30+1),w8(foodY,(unsigned int)rand()%30+1))!=0);
}

void snake_screenRenew(){
    unsigned char i,j,buffer[8];
    bit lightUp;
    unsigned long int tickTock=clock_get();

    sprintf(buffer,"%4u",r16(lengthHigh));
    lcd_stringSet(5,17,buffer);
    sprintf(buffer,"%4u",r16(length));
    lcd_stringSet(6,17,buffer);
    sprintf(buffer,"%5u",r16(tick));
    lcd_stringSet(7,16,buffer);

    // printf("[time=%u]",(unsigned int)(clock_get()-tickTock));tickTock=clock_get();

    for(i=0;i<64;i+=2){
        for(j=0;j<4;j++){
            buffer[j]=r8a(mapDirty,i*2+j);
        }
        for(j=0;j<64;j+=2){
            if(buffer[j/16]&(1<<(j/2%8))){
                lightUp=(snake_mapGet(i/2,j/2)!=0);
                lcd_pixelSet(i,j,lightUp);
                lcd_pixelSet(i+1,j,lightUp);
                lcd_pixelSet(i,j+1,lightUp);
                lcd_pixelSet(i+1,j+1,lightUp);
            }
        }
    }
    // printf("[time=%u]",(unsigned int)(clock_get()-tickTock));tickTock=clock_get();
    stack_memset(mapDirty,0,128);
    // printf("[time=%u]",(unsigned int)(clock_get()-tickTock));tickTock=clock_get();

    lcd_pixelSet(r8(foodX)*2,r8(foodY)*2,1);
    lcd_pixelSet(r8(foodX)*2+1,r8(foodY)*2,1);
    lcd_pixelSet(r8(foodX)*2,r8(foodY)*2+1,1);
    lcd_pixelSet(r8(foodX)*2+1,r8(foodY)*2+1,1);
    // printf("[time=%u]",(unsigned int)(clock_get()-tickTock));tickTock=clock_get();

    lcd_flush();
    printf("[snake_screenRenew time=%u]\n",(unsigned int)(clock_get()-tickTock));
}
