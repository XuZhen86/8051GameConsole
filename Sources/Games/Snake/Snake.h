#ifndef SNAKE_H_
#define SNAKE_H_

void snake();

void snake_levelSelect();
void snake_gamePlay();
void snake_gamePause();

void snake_mapInitialize();
unsigned int snake_mapGet(unsigned char x,unsigned char y);
unsigned int snake_mapSet(unsigned char x,unsigned char y,unsigned int val);
void snake_tickRestart();
void snake_foodRenew();

void snake_screenRenew();

#endif
