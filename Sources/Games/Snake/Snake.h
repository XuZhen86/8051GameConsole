#ifndef SNAKE_H_
#define SNAKE_H_

enum SNAKE_EXIT_CODE{
    SNAKE_EXIT_CODE_NORMAL,
    SNAKE_EXIT_CODE_CANCELED,
    SNAKE_EXIT_CODE_UNEXPECTED_TAIL_DATA
};

unsigned char snake();
unsigned char snake_gamePlay();
void snake_mapInitialize();
unsigned int snake_mapGet(unsigned char x,unsigned char y);
unsigned int snake_mapSet(unsigned char x,unsigned char y,unsigned int val);
void snake_renewDisplay(bit forceFlush);
void snake_newFood();
void snake_restartTick();
unsigned char snake_splashScreen();

void snake_iap_read();
void snake_iap_write();
void snake_iap_highScoreSet(unsigned char level,unsigned int score);
unsigned int snake_iap_highScoreGet(unsigned char level);
void snake_iap_levelSet(unsigned char level);
unsigned char snake_iap_levelGet();

// void _snake_printMap();

#endif
