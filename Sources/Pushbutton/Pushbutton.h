#ifndef PUSHBUTTON_H_
#define PUSHBUTTON_H_

enum PUSHBUTTON_DIRECTIONS{
    PUSHBUTTON_DIRECTION_UP=1,
    PUSHBUTTON_DIRECTION_DOWN=4,
    PUSHBUTTON_DIRECTION_LEFT=5,
    PUSHBUTTON_DIRECTION_RIGHT=3,
    PUSHBUTTON_DIRECTION_BACK=0,
    PUSHBUTTON_DIRECTION_FORWARD=2,
    PUSHBUTTON_DIRECTION_INVALID=255
};

enum PUSHBUTTON_NUMBERS{
    PUSHBUTTON_NUMBER_INVALID=255
};

unsigned char pushbutton_numberGet() small;
unsigned char pushbutton_directionGet() small;

unsigned char pushbutton_waitNumberGet() small;
unsigned char pushbutton_waitDirectionGet() small;
void pushbutton_waitNumberRelease() small;
void pushbutton_waitDirectionRelease() small;

unsigned char pushbutton_lastPressedNumberGet();
unsigned char pushbutton_lastPressedDirectionGet();

// void _pushbutton_directionButtonTest();

#endif
