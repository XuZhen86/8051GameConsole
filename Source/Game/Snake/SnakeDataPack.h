#ifndef SNAKEDATAPACK_H_
#define SNAKEDATAPACK_H_

typedef struct SnakeDataPack_{
    // 30*30 map with 1 boarder on each side
    unsigned int map[32][32];

    // 1-16 levels with [0] not used
    unsigned int highScore[16+1];

    // The bits indicate if a point on map needs redraw
    unsigned char mapDirty[32][4];

    // Other vars
    unsigned char foodX,foodY,headX,headY,tailX,tailY,level,direction,pressedDirection;
    unsigned int tick,length,refreshInterval,tailVal;
} SnakeDataPack;

#endif
