#ifndef SNAKE_H_
#define SNAKE_H_

void snake();

void snake_levelSelect();
void snake_gamePlay(unsigned int map[32][32]);
void snake_gamePause();

void snake_mapInitialize(unsigned int map[32][32]);
// unsigned int snake_mapGet(unsigned char x,unsigned char y);
// unsigned int snake_mapSet(unsigned char x,unsigned char y,unsigned int val);
void snake_tickRestart(unsigned int map[32][32]);
void snake_foodRenew(unsigned int map[32][32]);

void snake_screenRenew(unsigned int map[32][32],unsigned char mapDirty[32][4]);

#endif
