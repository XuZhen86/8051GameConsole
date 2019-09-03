#include"SnakeConfig.h"
#include"SnakeDataPack.h"
#include"SnakeStatic.h"
#include<Debug.h>
#include<Delay.h>
#include<IAPFile.h>
#include<LCD.h>
#include<Pushbutton.h>
#include<Snake.h>
#include<Time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

static SnakeDataPack *dp;

void Snake(){
    dp=calloc(1,sizeof(SnakeDataPack));
    loadData();

    while(selectLevel()){
        initializeGame();
        play();
    }

    storeData();
    free(dp);
}

static bit selectLevel(){
    char buffer[8];
    unsigned char bufferLen,startGame=0;

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
            default:
                Debug(WARNING,"Unexpected direction");
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
        // Start timer for tick interval
        Time_start();

        // Assume current heading if no pressed button
        dp->pressedDirection=dp->direction;

        // Keep reading buttons within the interval
        while(Time_elapsed()<dp->refreshInterval){
            if(Pushbutton_getDirection()!=INVALID){
                dp->pressedDirection=Pushbutton_getLastDirection();
            }
            delay(LEVEL_DELAY);
        }

        // Pause game if Back and Forward button were pressed
        if(dp->pressedDirection==BACK||dp->pressedDirection==FORWARD){
            pause();
            dp->pressedDirection=dp->direction;
        }

        // Omit pressed directions that are against current heading
        if(dp->pressedDirection!=dp->direction){
            if(((dp->direction==UP||dp->direction==DOWN)&&(dp->pressedDirection==LEFT||dp->pressedDirection==RIGHT))||((dp->direction==LEFT||dp->direction==RIGHT)&&(dp->pressedDirection==UP||dp->pressedDirection==DOWN))){
                dp->direction=dp->pressedDirection;
            }
        }

        // Advance snake head based on direction
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
            default:
                Debug(WARNING,"Unexpected direction %bu",dp->direction);
        }

        // If target point is occupied, the snake dies
        if(getMap(dp->headX,dp->headY)!=0){
            break;
        }

        // Advance snake head
        dp->tick++;
        setMap(dp->headX,dp->headY,dp->tick);
        if(dp->tick>=65533){
            restartTicks();
        }

        // If head not on the food
        if(dp->headX!=dp->foodX||dp->headY!=dp->foodY){
            dp->tailVal=getMap(dp->tailX,dp->tailY)+1;
            setMap(dp->tailX,dp->tailY,0);

            // Advance tail by erasing the point with previous value
            if(getMap(dp->tailX-1,dp->tailY)==dp->tailVal){
                dp->tailX--;
            }else if(getMap(dp->tailX+1,dp->tailY)==dp->tailVal){
                dp->tailX++;
            }else if(getMap(dp->tailX,dp->tailY-1)==dp->tailVal){
                dp->tailY--;
            }else if(getMap(dp->tailX,dp->tailY+1)==dp->tailVal){
                dp->tailY++;
            }else{
                Debug(FATAL,"Cannot find tail");
                return;
            }
        }else{
            dp->length++;

            renewFood();
        }

        drawScreen();
    }

    // Refresh high score
    if(dp->highScore[dp->level]<dp->length){
        dp->highScore[dp->level]=dp->length;
    }

    // Wait for button before exit
    do{
        Pushbutton_getDirectionWait();
        Pushbutton_directionReleaseWait();
    }while(Pushbutton_getLastDirection()!=FORWARD&&Pushbutton_getLastDirection()!=BACK);
}

static void pause(){
    unsigned char i,j;

    // Blank out map
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
    unsigned char i;
    char buffer[8];

    IAPFile_open(file,"Snake.txt");
    if(IAPFile_size(file)==0){
        generateDefaultData(file);
    }

    // Read high score for 16 levels
    for(i=1;i<17;i++){
        IAPFile_readLine(file,buffer,8);
        sscanf(buffer,"%u",&(dp->highScore[i]));
    }

    // Read last selected level
    IAPFile_readLine(file,buffer,8);
    sscanf(buffer,"%bu",&(dp->level));

    IAPFile_close(file);
    IAPFile_delete(file);
}

static void storeData(){
    IAPFile *file=IAPFile_new();
    unsigned char i;
    char buffer[8];

    // Overrite the file by not seeking to the end
    IAPFile_open(file,"Snake.txt");

    // Write high score for 16 levels
    for(i=1;i<17;i++){
        sprintf(buffer,"%u\n",dp->highScore[i]);
        IAPFile_write(file,buffer,(unsigned char)strlen(buffer));
    }

    // Write last selected level
    sprintf(buffer,"%bu\n",dp->level);
    IAPFile_write(file,buffer,(unsigned char)strlen(buffer));

    IAPFile_close(file);
    IAPFile_delete(file);
}

static void generateDefaultData(IAPFile *file){
    unsigned char i;

    // Write initial high score and selected level to file
    for(i=0;i<16;i++){
        IAPFile_write(file,"0\n",(unsigned char)strlen("0\n"));
    }
    IAPFile_write(file,"8\n",(unsigned char)strlen("8\n"));

    IAPFile_seek(file,0);
}

static void initializeGame(){
    unsigned char i;

    // Clear map
    memset(dp->map,0x00,sizeof(dp->map));

    // Set boarders
    for(i=0;i<32;i++){
        dp->map[0][i]=0xffff;
        dp->map[31][i]=0xffff;
        dp->map[i][0]=0xffff;
        dp->map[i][31]=0xffff;
    }

    // Assume whole map needs redraw
    memset(dp->mapDirty,0xff,sizeof(dp->mapDirty));

    // Set snake head
    dp->headX=15;
    dp->headY=15;
    dp->tailX=15;
    dp->tailY=19;
    dp->tick=0;

    // Set snake body
    for(i=dp->tailY;i>=dp->headY;i--){
        dp->tick++;
        dp->map[dp->headX][i]=dp->tick;
    }
    dp->length=dp->tick;
    dp->direction=LEFT;

    // Init rand()
    srand((unsigned int)Time_elapsed());
    // Get new food
    renewFood();

    drawScreen();
}

// Refresh ticks on map when they reach 65533 to prevent overflow
static void restartTicks(){
    unsigned char i,j;
    // Calculate offset
    unsigned int deltaTick=dp->map[dp->tailX][dp->tailY];

    // Apply offset to points, omit boarders
    for(i=1;i<31;i++){
        for(j=1;j<31;j++){
            // 0 means it's a white space, otherwise it's occupied
            // Food XY is kept in other variables
            if(dp->map[i][j]!=0){
                dp->map[i][j]-=deltaTick;
            }
        }
    }

    // Also offset current tick
    dp->tick-=deltaTick;
}

static void renewFood(){
    // Keep generating XY if the block is occupied
    do{
        dp->foodX=(unsigned int)rand()%30+1;
        dp->foodY=(unsigned int)rand()%30+1;
    }while(dp->map[dp->foodX][dp->foodY]!=0);
}

static unsigned int setMap(unsigned char x,unsigned char y,unsigned int val){
    // Use % to prevent out-of-bound
    x%=32;
    y%=32;

    // Set dirty bit and set value
    dp->mapDirty[x][y/8]|=(1<<(y%8));
    return dp->map[x][y]=val;
}

static unsigned int getMap(unsigned char x,unsigned char y){
    // Use % to prevent out-of-bound
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
                // Draw a 2x2 point
                lightUp=(dp->map[i/2][j/2]!=0);
                LCD_setPixel(i,j,lightUp);
                LCD_setPixel(i+1,j,lightUp);
                LCD_setPixel(i,j+1,lightUp);
                LCD_setPixel(i+1,j+1,lightUp);
            }
        }
    }
    memset(dp->mapDirty,0,sizeof(dp->mapDirty));

    // Draw 2x2 food point
    LCD_setPixel(dp->foodX*2,dp->foodY*2,1);
    LCD_setPixel(dp->foodX*2+1,dp->foodY*2,1);
    LCD_setPixel(dp->foodX*2,dp->foodY*2+1,1);
    LCD_setPixel(dp->foodX*2+1,dp->foodY*2+1,1);
}

static void drawScore(){
    char buffer[8];

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
