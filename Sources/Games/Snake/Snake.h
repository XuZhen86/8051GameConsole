#ifndef SNAKE_H_
#define SNAKE_H_

#include"Sources/I23LC512/I23LC512.h"
#include"Sources/LCD12864/LCD12864.h"
#include"Sources/Universal/SystemClock.h"
#include"Sources/Universal/Universal.h"
#include"Sources/Pushbutton/Pushbutton.h"

#include<stdlib.h>

enum SNAKE_EXIT_CODE{
    SNAKE_EXIT_CODE_NORMAL,
    SNAKE_EXIT_CODE_UNEXPECTED_TAIL_DATA
};

unsigned char snake();
void snake_initialize();
unsigned int snake_mapGet(unsigned char x,unsigned char y);
unsigned int snake_mapSet(unsigned char x,unsigned char y,unsigned int val);
void snake_renewDisplay(bit flush);
void snake_newFood();
void snake_restartTick();
void snake_splashScreen();

void _snake_printMap();

#endif
