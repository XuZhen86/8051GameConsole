#include"Sources/Pushbutton/Pushbutton.h"
#include"Sources/LCD/LCD.h"
#include"Sources/Clock/Clock.h"
#include"Sources/Delay/Delay.h"
#include"Sources/FarMem/FarMem.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"Sources/Games/Snake/Snake.h"
#include"Sources/Games/Snake/SnakeConfig.h"

unsigned char foodX,foodY,headX,headY,tailX,tailY,level,direction,pressedDirection;
unsigned int tick,length,lengthHigh,refreshInterval,tailVal;

struct SnakeData{
    unsigned int map[32][32];
} *snakeDataP;

void snake(){
    snakeDataP=farMalloc(sizeof(struct SnakeData));

    snake_levelSelect();
    snake_gamePlay(snakeDataP->map);

    pushbutton_waitDirectionGet();
    pushbutton_waitDirectionRelease();

    farFree(snakeDataP);
}

void snake_levelSelect(){
    unsigned char buffer[8],bufferLen;
    bit startGame=0;

    lcd_clear();
    lcd_stringSet(2,8,"Snake");
    lcd_stringSet(5,6,"Level:");

    // snake_mapInitialize(map);
    level=8;

    while(!startGame){
        refreshInterval=LEVEL_DELAY*(LEVEL_MAX-level)+LEVEL_BASE;

        bufferLen=sprintf(buffer,"%2bu ",level);
        printf("[bufferLen=%bu]\n",bufferLen);

        if(level==LEVEL_MAX){
            buffer[bufferLen-1]=0x19;
        }else if(level==LEVEL_MIN){
            buffer[bufferLen-1]=0x18;
        }else{
            buffer[bufferLen-1]=0x12;
        }

        lcd_stringSet(5,12,buffer);
        lcd_flush();

        switch(pushbutton_waitDirectionGet()){
            case UP:
                if(LEVEL_MAX<(++level)){
                    level=LEVEL_MAX;
                }
                break;
            case DOWN:
                if((--level)<LEVEL_MIN){
                    level=LEVEL_MIN;
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

    lengthHigh=15;

    srand(clock_get());
    // snake_foodRenew();

    lcd_clear();
    // snake_screenRenew();
}

void snake_gamePlay(unsigned int map[32][32]){
    // unsigned int far map[32][32];
    unsigned char mapDirty[32][4];
    unsigned long int tickTock;

    lcd_stringSet(5,11,"High");
    lcd_stringSet(6,11,"Length");
    lcd_stringSet(7,11,"Ticks");

    snake_mapInitialize(map);
    memset(mapDirty,0xff,128);
    snake_foodRenew(map);
    snake_screenRenew(map,mapDirty);

    while(1){
        clock_timerStart(refreshInterval);

        pressedDirection=direction;
        while(!clock_timerIsTimeUp()){
            if(pushbutton_directionGet()!=INVALID){
                pressedDirection=pushbutton_lastPressedDirectionGet();
            }
            delay(LEVEL_DELAY);
        }

        if(pressedDirection==BACK||pressedDirection==FORWARD){
            snake_gamePause();
            pressedDirection=direction;
        }

        tickTock=clock_get();

        if(pressedDirection!=(direction)){
            if(((direction==UP||direction==DOWN)&&(pressedDirection==LEFT||pressedDirection==RIGHT))||((direction==LEFT||direction==RIGHT)&&(pressedDirection==UP||pressedDirection==DOWN))){
                direction=pressedDirection;
            }
        }

        switch(direction){
            case UP:
                headX--;
                break;
            case DOWN:
                headX++;
                break;
            case LEFT:
                headY--;
                break;
            case RIGHT:
                headY++;
                break;
        }

        if(map[headX][headY]!=0){
            break;
        }

        map[headX][headY]=++tick;
        if(tick==65534){
            snake_tickRestart(map);
        }

        if(headX!=foodX||headY!=foodY){
            tailVal=map[tailX][tailY]+1;
            map[tailX][tailY]=0;

            if(map[tailX-1][tailY]==tailVal){
                tailX--;
            }else if(map[tailX+1][tailY]==tailVal){
                tailX++;
            }else if(map[tailX][tailY-1]==tailVal){
                tailY--;
            }else if(map[tailX][tailY+1]==tailVal){
                tailY++;
            }else{
                puts("SNAKE_EXIT_CODE_UNEXPECTED_TAIL_DATA");
                printf("[head=%bu,%bu food=%bu,%bu tail=%bu,%bu]\n",headX,headY,foodX,foodY,tailX,tailY);
                return;
            }
        }else{
            length++;
            if(length>lengthHigh){
                lengthHigh=length;
            }
            snake_foodRenew(map);
        }

        snake_screenRenew(map,mapDirty);
    }

    do{
        pushbutton_waitDirectionGet();
        pushbutton_waitDirectionRelease();
    }while(pushbutton_lastPressedDirectionGet()!=FORWARD&&pushbutton_lastPressedDirectionGet()!=BACK);
}

void snake_gamePause(){
    unsigned char i,j;

    // lcd_bufferStackPush();
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

    // lcd_bufferStackPop();
    lcd_flush();
}

void snake_mapInitialize(unsigned int map[32][32]){
    unsigned char i,j;

    for(i=1;i<31;i++){
        for(j=1;j<31;j++){
            map[i][j]=0;
        }
    }

    for(i=0;i<32;i++){
        map[0][i]=0xffff;
        map[31][i]=0xffff;
        map[i][0]=0xffff;
        map[1][31]=0xffff;
    }

    headX=15;
    headY=15;
    tailX=15;
    tailY=19;
    tick=0;
    for(i=tailY;i>=headY;i--){
        map[headX][i]=++tick;
    }
    length=tick;

    direction=LEFT;
}

// unsigned int snake_mapGet(unsigned char x,unsigned char y){
//     return map[x%32][y%32];
// }

// unsigned int snake_mapSet(unsigned char x,unsigned char y,unsigned int val){
//     x%=32;
//     y%=32;
//     mapDirty[x][y/8]|=(1<<(y%8));
//     return map[x][y]=val;
// }

void snake_tickRestart(unsigned int map[32][32]){
    unsigned int dTick=map[tailX][tailY];
    unsigned char i,j;

    for(i=1;i<31;i++){
        for(j=1;j<31;j++){
            if(map[i][j]!=0){
                map[i][j]-=dTick;
            }
        }
    }

    tick-=dTick;
}

void snake_foodRenew(unsigned int map[32][32]){
    do{
        foodX=(unsigned int)rand()%30+1;
        foodY=(unsigned int)rand()%30+1;
    }while(map[foodX][foodY]!=0);
}

void snake_screenRenew(unsigned int map[32][32],unsigned char mapDirty[32][4]){
    unsigned char i,j,buffer[16];
    bit lightUp;
    unsigned long int tickTock=clock_get();

    sprintf(buffer,"%4u",lengthHigh);
    lcd_stringSet(5,17,buffer);
    sprintf(buffer,"%4u",length);
    lcd_stringSet(6,17,buffer);
    sprintf(buffer,"%5u",tick);
    lcd_stringSet(7,16,buffer);

    // printf("[time=%u]",(unsigned int)(clock_get()-tickTock));tickTock=clock_get();

    for(i=0;i<64;i+=2){
        for(j=0;j<64;j+=2){
            // if(mapDirty[i/2][j/16]&(1<<(j/2%8))){
                lightUp=(map[i/2][j/2]!=0);
                lcd_pixelSet(i,j,lightUp);
                lcd_pixelSet(i+1,j,lightUp);
                lcd_pixelSet(i,j+1,lightUp);
                lcd_pixelSet(i+1,j+1,lightUp);
            // }
        }
    }
    // printf("[time=%u]",(unsigned int)(clock_get()-tickTock));tickTock=clock_get();
    memset(mapDirty,0,128);
    // printf("[time=%u]",(unsigned int)(clock_get()-tickTock));tickTock=clock_get();

    lcd_pixelSet(foodX*2,foodY*2,1);
    lcd_pixelSet(foodX*2+1,foodY*2,1);
    lcd_pixelSet(foodX*2,foodY*2+1,1);
    lcd_pixelSet(foodX*2+1,foodY*2+1,1);
    // printf("[time=%u]",(unsigned int)(clock_get()-tickTock));tickTock=clock_get();

    lcd_flush();
    printf("[snake_screenRenew time=%u]\n",(unsigned int)(clock_get()-tickTock));
}
