#include"Sources/Games/Snake/Snake.h"

enum SNAKE_DIRECTION{
    UP=PUSHBUTTON_DIRECTION_UP,
    DOWN=PUSHBUTTON_DIRECTION_DOWN,
    LEFT=PUSHBUTTON_DIRECTION_LEFT,
    RIGHT=PUSHBUTTON_DIRECTION_RIGHT,
    INVALID=PUSHBUTTON_DIRECTION_INVALID
};

enum SNAKE_RAM_CONFIG{
    MAP_ADDR=0x0000,
    MAP_SIZE=2048,
    MAP_DIRTY_ADDR=MAP_ADDR+MAP_SIZE,
    MAP_DIRTY_SIZE=128
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
    unsigned int data refreshInterval=100,tailVal;
    unsigned char data pressedDirection;
    char data i,j;
    bit foundTail=0;

    unsigned long int lastClock=systemClock_get();

    // printf("[%u, %lu]\n",(unsigned int)1,systemClock_get()-lastClock);lastClock=systemClock_get();

    lcd12864_stringSet(3,0,"Waiting");lcd12864_flush(0);
    while(pushbutton_directionGet()==INVALID);
    lcd12864_stringSet(3,0,"Pressed");lcd12864_flush(0);

    // printf("[%u, %lu]\n",(unsigned int)2,systemClock_get()-lastClock);lastClock=systemClock_get();
    snake_initialize();
    // printf("[%u, %lu]\n",(unsigned int)3,systemClock_get()-lastClock);lastClock=systemClock_get();
    snake_renewDisplay(1);
    // printf("[%u, %lu]\n",(unsigned int)4,systemClock_get()-lastClock);lastClock=systemClock_get();

    while(1){
        // printf("[%u, %lu]\n",(unsigned int)5,systemClock_get()-lastClock);lastClock=systemClock_get();
        systemClock_timerStart(refreshInterval);
        // printf("[%u, %lu]\n",(unsigned int)6,systemClock_get()-lastClock);lastClock=systemClock_get();

        pressedDirection=direction;
        while(!systemClock_timerIsTimeUp()){
            // printf("[%u, %lu]\n",(unsigned int)7,systemClock_get()-lastClock);lastClock=systemClock_get();
            if(pushbutton_directionGet()!=INVALID){
                pressedDirection=pushbutton_directionGet();
            }
            // printf("[%u, %lu]\n",(unsigned int)8,systemClock_get()-lastClock);lastClock=systemClock_get();
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
            break;
        }

        snake_mapSet(snakeHeadX,snakeHeadY,++snakeTick);
        // printf("[%u]",snakeTick);

        // printf("[%u, %lu]\n",(unsigned int)9,systemClock_get()-lastClock);lastClock=systemClock_get();
        foundTail=0;
        if(snakeHeadX!=foodX||snakeHeadY!=foodY){
            tailVal=snake_mapGet(snakeTailX,snakeTailY);
            for(i=snakeTailX-1;i<=snakeTailX+1&&!foundTail;i++){
                for(j=snakeTailY-1;j<=snakeTailY+1&&!foundTail;j++){
                    if(0<=i&&i<MAP_X&&0<=j&&j<MAP_Y&&snake_mapGet(i,j)==tailVal+1){
                        foundTail=1;
                        snake_mapSet(snakeTailX,snakeTailY,0);
                        snakeTailX=i;
                        snakeTailY=j;
                    }
                }
            }

            if(!foundTail){
                return SNAKE_EXIT_CODE_UNEXPECTED_TAIL_DATA;
            }
        }else{
            snakeLength++;
            snake_newFood();
        }
        // printf("[%u, %lu]\n",(unsigned int)10,systemClock_get()-lastClock);lastClock=systemClock_get();

        snake_renewDisplay(1);
        // printf("[%u, %lu]\n",(unsigned int)11,systemClock_get()-lastClock);lastClock=systemClock_get();
    }

    return SNAKE_EXIT_CODE_NORMAL;
}

void snake_initialize(){
    unsigned char data i;

    // Clear map
    i23lc512_memset(MAP_ADDR,0,MAP_SIZE);
    // Clear map delta
    i23lc512_memset(MAP_DIRTY_ADDR,0xff,MAP_DIRTY_SIZE);

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
    x%=32;
    y%=32;
    return i23lc512_uIntRead(MAP_ADDR+x*64+y*2);
}

unsigned int snake_mapSet(unsigned char x,unsigned char y,unsigned int val){
    x%=32;
    y%=32;
    i23lc512_uCharWrite(MAP_DIRTY_ADDR+x*4+y/8,i23lc512_uCharRead(MAP_DIRTY_ADDR+x*4+y/8)|(1<<(y%8)));
    return i23lc512_uIntWrite(MAP_ADDR+x*64+y*2,val);
}

void snake_renewDisplay(bit flush){
    unsigned char data i,j;
    unsigned char buffer[8];
    bit lightUp;

    // draw map
    for(i=0;i<64;i+=2){
        i23lc512_uCharSeqRead(buffer,MAP_DIRTY_ADDR+i*2,4);
        for(j=0;j<64;j+=2){
            if(buffer[j/16]&(1<<(j/2%8))){
                lightUp=(snake_mapGet(i/2,j/2)!=0);
                lcd12864_pixelSet(i,j,lightUp);
                lcd12864_pixelSet(i+1,j,lightUp);
                lcd12864_pixelSet(i,j+1,lightUp);
                lcd12864_pixelSet(i+1,j+1,lightUp);
            }
        }
    }
    i23lc512_memset(MAP_DIRTY_ADDR,0,MAP_DIRTY_SIZE);

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
    do{
        foodX=rand()%32;
        foodY=rand()%32;
    }while(snake_mapGet(foodX,foodY)!=0);
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
