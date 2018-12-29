#include"SnakeConfig.h"
#include"SnakeDataPack.h"
#include"SnakeStatic.h"
#include<Debug.h>
#include<Delay.h>
#include<Far.h>
#include<IAPFile.h>
#include<LCD.h>
#include<Pushbutton.h>
#include<Snake.h>
#include<Time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

SnakeDataPack *dp;

void Snake(){
    dp=Far_calloc(1,sizeof(SnakeDataPack));
    loadData();

    while(selectLevel()){
        initializeGame();
        play();
    }

    storeData();
    Far_free(dp);
}

static bit selectLevel(){
    unsigned char buffer[8],bufferLen;
    unsigned char startGame=0;

    LCD_clear();
    LCD_setString(2,8,"Snake");
    LCD_setString(5,6,"Level:");

    while(!startGame){
        bufferLen=sprintf(buffer,"%2bu ",dp->level);

        if(dp->level==LEVEL_MAX){
            buffer[bufferLen-1]=0x19;
        }else if(dp->level==LEVEL_MIN){
            buffer[bufferLen-1]=0x18;
        }else{
            buffer[bufferLen-1]=0x12;
        }

        LCD_setString(5,12,buffer);
        LCD_flush();

        switch(Pushbutton_getDirectionWait()){
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

        Pushbutton_directionReleaseWait();
    }

    dp->refreshInterval=LEVEL_DELAY*(LEVEL_MAX-dp->level)+LEVEL_BASE;

    LCD_clear();
    return 1;
}

static void play(){
    drawScreen();

    while(1){
        Time_start();

        dp->pressedDirection=dp->direction;
        while(Time_elapsed()<dp->refreshInterval){
            if(Pushbutton_getDirection()!=INVALID){
                dp->pressedDirection=Pushbutton_getLastDirection();
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
                Debug(FATAL,"SNAKE_EXIT_CODE_UNEXPECTED_TAIL_DATA");
                Debug(FATAL,"[head=%bu,%bu food=%bu,%bu tail=%bu,%bu]",dp->headX,dp->headY,dp->foodX,dp->foodY,dp->tailX,dp->tailY);
                return;
            }
        }else{
            dp->length++;

            renewFood();
        }

        drawScreen();
    }

    if(dp->highScore[dp->level]<dp->length){
        dp->highScore[dp->level]=dp->length;
    }

    do{
        Pushbutton_getDirectionWait();
        Pushbutton_directionReleaseWait();
    }while(Pushbutton_getLastDirection()!=FORWARD&&Pushbutton_getLastDirection()!=BACK);
}

static void pause(){
    unsigned char i,j;

    for(i=2;i<62;i+=2){
        for(j=2;j<62;j+=2){
            LCD_setPixel(i,j,0);
            LCD_setPixel(i+1,j,0);
            LCD_setPixel(i,j+1,0);
            LCD_setPixel(i+1,j+1,0);
        }
    }
    LCD_setString(3,3,"Pause");
    LCD_flush();
    memset(dp->mapDirty,0xff,sizeof(dp->mapDirty));

    do{
        Pushbutton_getDirectionWait();
        Pushbutton_directionReleaseWait();
    }while(Pushbutton_getLastDirection()!=FORWARD&&Pushbutton_getLastDirection()!=BACK);

    drawScreen();
}

static void loadData(){
    IAPFile *file=IAPFile_new();
    unsigned char i,buffer[8];
    Debug(DEBUG,"loadData()");

    IAPFile_open(file,"Snake.txt");
    if(IAPFile_size(file)==0){
        generateDefaultData(file);
    }

    for(i=1;i<17;i++){
        IAPFile_readLine(file,buffer,8);
        sscanf(buffer,"%u",&(dp->highScore[i]));
    }

    IAPFile_readLine(file,buffer,8);
    sscanf(buffer,"%u",&(dp->level));

    IAPFile_close(file);
    IAPFile_delete(file);
}

static void storeData(){
    IAPFile *file=IAPFile_new();
    unsigned char i,buffer[8];
    Debug(DEBUG,"storeData()");

    IAPFile_open(file,"Snake.txt");

    for(i=1;i<17;i++){
        sprintf(buffer,"%u\n",dp->highScore[i]);
        IAPFile_write(file,buffer,strlen(buffer));
    }

    sprintf(buffer,"%u\n",dp->level);
    IAPFile_write(file,buffer,strlen(buffer));

    IAPFile_close(file);
    IAPFile_delete(file);
}

static void generateDefaultData(IAPFile *file){
    unsigned char i;
    Debug(DEBUG,"generateDefaultData()");

    for(i=0;i<16;i++){
        IAPFile_write(file,"0\n",strlen("0\n"));
    }
    IAPFile_write(file,"8\n",strlen("8\n"));

    IAPFile_seek(file,0);
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

    srand(Time_elapsed());
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
    LCD_flush();
}

static void drawMap(){
    unsigned char i,j,lightUp;

    for(i=0;i<64;i+=2){
        for(j=0;j<64;j+=2){
            if(dp->mapDirty[i/2][j/16]&(1<<(j/2%8))){
                lightUp=(dp->map[i/2][j/2]!=0);
                LCD_setPixel(i,j,lightUp);
                LCD_setPixel(i+1,j,lightUp);
                LCD_setPixel(i,j+1,lightUp);
                LCD_setPixel(i+1,j+1,lightUp);
            }
        }
    }
    memset(dp->mapDirty,0,sizeof(dp->mapDirty));

    LCD_setPixel(dp->foodX*2,dp->foodY*2,1);
    LCD_setPixel(dp->foodX*2+1,dp->foodY*2,1);
    LCD_setPixel(dp->foodX*2,dp->foodY*2+1,1);
    LCD_setPixel(dp->foodX*2+1,dp->foodY*2+1,1);
}

static void drawScore(){
    unsigned char buffer[8];

    LCD_setString(5,11,"High");
    sprintf(buffer,"%4u",dp->highScore[dp->level]);
    LCD_setString(5,17,buffer);

    LCD_setString(6,11,"Length");
    sprintf(buffer,"%4u",dp->length);
    LCD_setString(6,17,buffer);

    LCD_setString(7,11,"Ticks");
    sprintf(buffer,"%5u",dp->tick);
    LCD_setString(7,16,buffer);
}
