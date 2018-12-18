#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"../../Pushbutton/Pushbutton.h"
#include"../../LCD/LCD.h"
#include"../../Clock/Clock.h"
#include"../../Delay/Delay.h"
#include"../../FarMem/FarMem.h"
#include"../../IAPFile/IAPFile.h"
#include"../../XRAM/XRAM.h"

#include"./Snake.h"
#include"./SnakeDataPack.h"
#include"./SnakeStatic.h"
#include"./SnakeConfig.h"

SnakeDataPack *dp;

void Snake(){
    dp=farCalloc(1,sizeof(SnakeDataPack));
    loadData();

    while(selectLevel()){
        initializeGame();
        play();
    }

    storeData();
    farFree(dp);
}

static bit selectLevel(){
    unsigned char buffer[8],bufferLen;
    unsigned char startGame=0;

    lcd_clear();
    lcd_stringSet(2,8,"Snake");
    lcd_stringSet(5,6,"Level:");

    while(!startGame){
        bufferLen=sprintf(buffer,"%2bu ",dp->level);

        if(dp->level==LEVEL_MAX){
            buffer[bufferLen-1]=0x19;
        }else if(dp->level==LEVEL_MIN){
            buffer[bufferLen-1]=0x18;
        }else{
            buffer[bufferLen-1]=0x12;
        }

        lcd_stringSet(5,12,buffer);
        lcd_flush();

        switch(pushbutton_waitDirectionGet()){
            case UP:
                if(LEVEL_MAX<(++(dp->level))){
                    dp->level=LEVEL_MAX;
                }
                break;
            case DOWN:
                if((--(dp->level))<LEVEL_MIN){
                    dp->level=LEVEL_MIN;
                }
                break;
            case FORWARD:
                startGame=1;
                break;
            case BACK:
                return 0;
        }

        pushbutton_waitDirectionRelease();
    }

    dp->refreshInterval=LEVEL_DELAY*(LEVEL_MAX-dp->level)+LEVEL_BASE;

    lcd_clear();
    return 1;
}

static void play(){
    drawScreen();

    while(1){
        clock_timerStart(dp->refreshInterval);

        dp->pressedDirection=dp->direction;
        while(!clock_timerIsTimeUp()){
            if(pushbutton_directionGet()!=INVALID){
                dp->pressedDirection=pushbutton_lastPressedDirectionGet();
            }
            delay(LEVEL_DELAY);
        }

        if(dp->pressedDirection==BACK||dp->pressedDirection==FORWARD){
            pause();
            dp->pressedDirection=dp->direction;
        }

        if(dp->pressedDirection!=dp->direction){
            if(((dp->direction==UP||dp->direction==DOWN)&&(dp->pressedDirection==LEFT||dp->pressedDirection==RIGHT))||((dp->direction==LEFT||dp->direction==RIGHT)&&(dp->pressedDirection==UP||dp->pressedDirection==DOWN))){
                dp->direction=dp->pressedDirection;
            }
        }

        switch(dp->direction){
            case UP:
                dp->headX--;
                break;
            case DOWN:
                dp->headX++;
                break;
            case LEFT:
                dp->headY--;
                break;
            case RIGHT:
                dp->headY++;
                break;
        }

        if(getMap(dp->headX,dp->headY)!=0){
            break;
        }

        dp->tick++;
        setMap(dp->headX,dp->headY,dp->tick);
        if(dp->tick>=65533){
            restartTicks();
        }

        if(dp->headX!=dp->foodX||dp->headY!=dp->foodY){
            dp->tailVal=getMap(dp->tailX,dp->tailY)+1;
            setMap(dp->tailX,dp->tailY,0);

            if(getMap(dp->tailX-1,dp->tailY)==dp->tailVal){
                dp->tailX--;
            }else if(getMap(dp->tailX+1,dp->tailY)==dp->tailVal){
                dp->tailX++;
            }else if(getMap(dp->tailX,dp->tailY-1)==dp->tailVal){
                dp->tailY--;
            }else if(getMap(dp->tailX,dp->tailY+1)==dp->tailVal){
                dp->tailY++;
            }else{
                puts("SNAKE_EXIT_CODE_UNEXPECTED_TAIL_DATA");
                printf("[head=%bu,%bu food=%bu,%bu tail=%bu,%bu]\n",dp->headX,dp->headY,dp->foodX,dp->foodY,dp->tailX,dp->tailY);
                return;
            }
        }else{
            dp->length++;
            if(dp->highScore[dp->level]<dp->length){
                dp->highScore[dp->level]=dp->length;
            }
            renewFood();
        }

        drawScreen();
    }

    do{
        pushbutton_waitDirectionGet();
        pushbutton_waitDirectionRelease();
    }while(pushbutton_lastPressedDirectionGet()!=FORWARD&&pushbutton_lastPressedDirectionGet()!=BACK);
}

static void pause(){
    unsigned char i,j;

    for(i=2;i<62;i+=2){
        for(j=2;j<62;j+=2){
            lcd_pixelSet(i,j,0);
            lcd_pixelSet(i+1,j,0);
            lcd_pixelSet(i,j+1,0);
            lcd_pixelSet(i+1,j+1,0);
        }
    }
    lcd_stringSet(3,3,"Pause");
    lcd_flush();
    memset(dp->mapDirty,0xff,sizeof(dp->mapDirty));

    do{
        pushbutton_waitDirectionGet();
        pushbutton_waitDirectionRelease();
    }while(pushbutton_lastPressedDirectionGet()!=FORWARD&&pushbutton_lastPressedDirectionGet()!=BACK);

    drawScreen();
}

static void loadData(){
    IapFile *file=IapFile_new();
    unsigned char i,buffer[8];
    // printf("loadData()\n");

    IapFile_open(file,"Snake.txt");
    if(IapFile_size(file)==0){
        generateDefaultData(file);
    }

    for(i=0;i<16;i++){
        IapFile_readLine(file,buffer,8);
        sscanf(buffer,"%u",&(dp->highScore[i]));
    }

    IapFile_readLine(file,buffer,8);
    sscanf(buffer,"%u",&(dp->level));

    IapFile_close(file);
    IapFile_delete(file);
}

static void storeData(){
    IapFile *file=IapFile_new();
    unsigned char i,buffer[8];
    // printf("storeData()\n");

    IapFile_open(file,"Snake.txt");

    for(i=0;i<16;i++){
        sprintf(buffer,"%u\n",dp->highScore[i]);
        IapFile_write(file,buffer,strlen(buffer));
    }

    sprintf(buffer,"%u\n",dp->level);
    IapFile_write(file,buffer,strlen(buffer));

    IapFile_close(file);
    IapFile_delete(file);
}

static void generateDefaultData(IapFile *file){
    unsigned char i;
    // printf("generateDefaultData()\n");

    for(i=0;i<16;i++){
        IapFile_write(file,"0\n",strlen("0\n"));
    }
    IapFile_write(file,"8\n",strlen("8\n"));

    IapFile_seek(file,0);
}

static void initializeGame(){
    unsigned char i;

    memset(dp->map,0x00,sizeof(dp->map));
    for(i=0;i<32;i++){
        dp->map[0][i]=0xffff;
        dp->map[31][i]=0xffff;
        dp->map[i][0]=0xffff;
        dp->map[i][31]=0xffff;
    }
    memset(dp->mapDirty,0xff,sizeof(dp->mapDirty));

    dp->headX=15;
    dp->headY=15;
    dp->tailX=15;
    dp->tailY=19;
    dp->tick=0;

    for(i=dp->tailY;i>=dp->headY;i--){
        dp->tick++;
        dp->map[dp->headX][i]=dp->tick;
    }
    dp->length=dp->tick;
    dp->direction=LEFT;

    srand(clock_get());
    renewFood();

    drawScreen();
}

static void restartTicks(){
    unsigned char i,j;
    unsigned int deltaTick=dp->map[dp->tailX][dp->tailY];

    for(i=1;i<31;i++){
        for(j=1;j<31;j++){
            if(dp->map[i][j]!=0){
                dp->map[i][j]-=deltaTick;
            }
        }
    }

    dp->tick-=deltaTick;
}

static void renewFood(){
    do{
        dp->foodX=(unsigned int)rand()%30+1;
        dp->foodY=(unsigned int)rand()%30+1;
    }while(dp->map[dp->foodX][dp->foodY]!=0);
}

static unsigned int setMap(unsigned char x,unsigned char y,unsigned int val){
    x%=32;
    y%=32;
    dp->mapDirty[x][y/8]|=(1<<(y%8));
    return dp->map[x][y]=val;
}

static unsigned int getMap(unsigned char x,unsigned char y){
    return dp->map[x%32][y%32];
}

static void drawScreen(){
    drawMap();
    drawScore();
    lcd_flush();
}

static void drawMap(){
    unsigned char i,j,lightUp;

    for(i=0;i<64;i+=2){
        for(j=0;j<64;j+=2){
            if(dp->mapDirty[i/2][j/16]&(1<<(j/2%8))){
                lightUp=(dp->map[i/2][j/2]!=0);
                lcd_pixelSet(i,j,lightUp);
                lcd_pixelSet(i+1,j,lightUp);
                lcd_pixelSet(i,j+1,lightUp);
                lcd_pixelSet(i+1,j+1,lightUp);
            }
        }
    }
    memset(dp->mapDirty,0,sizeof(dp->mapDirty));

    lcd_pixelSet(dp->foodX*2,dp->foodY*2,1);
    lcd_pixelSet(dp->foodX*2+1,dp->foodY*2,1);
    lcd_pixelSet(dp->foodX*2,dp->foodY*2+1,1);
    lcd_pixelSet(dp->foodX*2+1,dp->foodY*2+1,1);
}

static void drawScore(){
    unsigned char buffer[8];

    lcd_stringSet(5,11,"High");
    sprintf(buffer,"%4u",dp->highScore[dp->level]);
    lcd_stringSet(5,17,buffer);

    lcd_stringSet(6,11,"Length");
    sprintf(buffer,"%4u",dp->length);
    lcd_stringSet(6,17,buffer);

    lcd_stringSet(7,11,"Ticks");
    sprintf(buffer,"%5u",dp->tick);
    lcd_stringSet(7,16,buffer);
}

static void dumpMap(){
    unsigned char i,j;
    for(i=0;i<32;i++){
        for(j=0;j<32;j++){
            printf("0x%04x ",xRam_uIntRead((unsigned int)(dp->map[i]+j)));
        }
        printf("\n");
    }
}
