#include"Sources/Games/Snake/Snake.h"

enum SNAKE_DIRECTION{
    UP=PUSHBUTTON_DIRECTION_UP,
    DOWN=PUSHBUTTON_DIRECTION_DOWN,
    LEFT=PUSHBUTTON_DIRECTION_LEFT,
    RIGHT=PUSHBUTTON_DIRECTION_RIGHT,
    INVALID=PUSHBUTTON_DIRECTION_INVALID
};

enum{
    MAP_ADDR=0x0000
};

enum SNAKE_MAP{
    MAP_X=32,
    MAP_Y=32,
    PIXEL_SIZE=2
};

static unsigned int
    snakeLength=0,
    snakeTick=0;

static unsigned char
    snakeHeadX=15,
    snakeHeadY=15,
    snakeTailX=15,
    snakeTailY=19,
    foodX,
    foodY,
    direction=LEFT;

unsigned char snake(){
    unsigned int refreshInterval=200,tailVal;
    unsigned char pressedDirection;
    char i,j;
    bit foundTail=0;

    puts("snake()");

    lcd12864_stringSet(3,0,"Waiting");lcd12864_flush(0);
    while(pushbutton_directionGet()==INVALID);
    lcd12864_stringSet(3,0,"Pressed");lcd12864_flush(0);

    snake_initialize();

    snake_renewDisplay(1);

    while(1){
        systemClock_timerStart(refreshInterval);

        pressedDirection=direction;
        while(!systemClock_timerIsTimeUp()){
            if(pushbutton_directionGet()!=INVALID){
                pressedDirection=pushbutton_directionGet();
            }
        }

        if(pressedDirection!=direction){
            if(((direction==UP||direction==DOWN)&&(pressedDirection==LEFT||pressedDirection==RIGHT))||((direction==LEFT||direction==RIGHT)&&(pressedDirection==UP||pressedDirection==DOWN))){
                direction=pressedDirection;
            }
        }

        if(direction==UP){
            snakeHeadX--;
        }else if(direction==DOWN){
            snakeHeadX++;
        }else if(direction==LEFT){
            snakeHeadY--;
        }else{
            snakeHeadY++;
        }

        if(snake_isNextStepLethal()){
            return SNAKE_ERROR_NORMAL;
        }

        snake_mapSet(snakeHeadX,snakeHeadY,++snakeTick);

        foundTail=0;
        if(snakeHeadX!=foodX||snakeHeadY!=foodY){
            tailVal=snake_mapGet(snakeTailX,snakeTailY);
            for(i=snakeTailX-1;i<=snakeTailX+1&&!foundTail;i++){
                for(j=snakeTailY-1;j<=snakeTailY+1&&!foundTail;j++){
                    if(0<=i&&i<MAP_X&&0<=j&&j<MAP_Y&&snake_mapGet(i,j)==tailVal+1){
                        foundTail=1;
                        printf("foundTail[%d,%d]\n",(int)i,(int)j);
                        snake_mapSet(snakeTailX,snakeTailY,0);
                        snakeTailX=i;
                        snakeTailY=j;
                    }
                }
            }

            if(!foundTail){
                return SNAKE_ERROR_UNEXPECTED_TAIL_DATA;
            }
        }else{
            snakeLength++;
            snake_newFood();
        }

        snake_renewDisplay(1);
    }

    return SNAKE_ERROR_NORMAL;
}

void snake_initialize(){
    unsigned char data i;

    puts("snake_initialize()");

    // Clear map
    i23lc512_memset(MAP_ADDR,0,2048);
    // Setup walls
    for(i=0;i<32;i++){
        snake_mapSet(0,i,0xffff);
        snake_mapSet(31,i,0xffff);
        snake_mapSet(i,0,0xffff);
        snake_mapSet(i,31,0xffff);
    }

    // Put baby snake in
    for(i=snakeTailY;i>=snakeHeadY;i--){
        snake_mapSet(snakeHeadX,i,++snakeTick);
    }
    snakeLength=snakeTick;

    srand(systemClock_get());
    snake_newFood();
}

unsigned int snake_mapGet(unsigned char x,unsigned char y){
    return i23lc512_uIntRead(MAP_ADDR+x%32*64+y%32*2);
}

unsigned int snake_mapSet(unsigned char x,unsigned char y,unsigned int val){
    return i23lc512_uIntWrite(MAP_ADDR+x%32*64+y%32*2,val);
}

void snake_renewDisplay(bit flush){
    unsigned char data i,j;
    unsigned char buffer[8];

    // draw map
    for(i=0;i<32;i++){
        for(j=0;j<32;j++){
            if(snake_mapGet(i,j)==0){
                lcd12864_pixelSet(i*2,j*2,0);
                lcd12864_pixelSet(i*2+1,j*2,0);
                lcd12864_pixelSet(i*2,j*2+1,0);
                lcd12864_pixelSet(i*2+1,j*2+1,0);
            }else{
                lcd12864_pixelSet(i*2,j*2,1);
                lcd12864_pixelSet(i*2+1,j*2,1);
                lcd12864_pixelSet(i*2,j*2+1,1);
                lcd12864_pixelSet(i*2+1,j*2+1,1);
            }
        }
    }

    // draw food
    lcd12864_pixelSet(foodX*2,foodY*2,1);
    lcd12864_pixelSet(foodX*2+1,foodY*2,1);
    lcd12864_pixelSet(foodX*2,foodY*2+1,1);
    lcd12864_pixelSet(foodX*2+1,foodY*2+1,1);

    // display score
    lcd12864_stringSet(7,13,"Length=");
    lcd12864_stringSet(7,20,uitoa(snakeLength,buffer));

    if(flush){
        lcd12864_flush(0);
    }
}

void snake_newFood(){
    unsigned char data randX,randY;

    do{
        randX=rand()%32;
        randY=rand()%32;
    }while(snake_mapGet(randX,randY)!=0);

    foodX=randX;
    foodY=randY;
}

bit snake_isNextStepLethal(){
    return snake_mapGet(snakeHeadX,snakeHeadY)!=0;
}

void _snake_printMap(){
    unsigned char i,j;
    for(i=0;i<MAP_X;i++){
        for(j=0;j<MAP_Y;j++){
            printf("%04x ",snake_mapGet(i,j));
        }
        printf("\n");
    }
    printf("\n");
}
