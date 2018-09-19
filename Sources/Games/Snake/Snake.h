#ifndef SNAKE_H_
#define SNAKE_H_

#include"Sources/I23LC512/I23LC512.h"
#include"Sources/LCD12864/LCD12864.h"
#include"Sources/Universal/SystemClock.h"
#include"Sources/Universal/Universal.h"
#include"Sources/Pushbutton/Pushbutton.h"

#include<stdlib.h>

enum SNAKE_EXIT_CODE{
    SNAKE_ERROR_NORMAL,
    SNAKE_ERROR_UNEXPECTED_DIRECTION,
    SNAKE_ERROR_UNEXPECTED_TAIL_DATA
};

unsigned char snake();
void snake_initialize();
unsigned int snake_mapGet(unsigned char x,unsigned char y);
unsigned int snake_mapSet(unsigned char x,unsigned char y,unsigned int val);
void snake_renewDisplay(bit flush);
void snake_newFood();
bit snake_isNextStepLethal();

void _snake_printMap();

#endif
