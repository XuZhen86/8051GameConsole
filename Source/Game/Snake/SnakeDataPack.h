#ifndef SNAKEDATAPACK_H_
#define SNAKEDATAPACK_H_

typedef struct SnakeDataPack_{
    unsigned int map[32][32];
    unsigned int highScore[16+1];
    unsigned char mapDirty[32][4];
    unsigned char foodX,foodY,headX,headY,tailX,tailY,level,direction,pressedDirection;
    unsigned int tick,length,refreshInterval,tailVal;
} SnakeDataPack;

#endif
