#ifndef PUSHBUTTON_H_
#define PUSHBUTTON_H_

enum PUSHBUTTON_DIRECTIONS{
    PUSHBUTTON_DIRECTION_UP=1,
    PUSHBUTTON_DIRECTION_DOWN=4,
    PUSHBUTTON_DIRECTION_LEFT=5,
    PUSHBUTTON_DIRECTION_RIGHT=3,
    PUSHBUTTON_DIRECTION_BACK=0,
    PUSHBUTTON_DIRECTION_FORWARD=2,
    PUSHBUTTON_DIRECTION_INVALID=255,
};

enum PUSHBUTTON_NUMBERS{
    PUSHBUTTON_NUMBER_INVALID=255,
};

// Read button even no button is pressed
unsigned char Pushbutton_getNumber();
unsigned char Pushbutton_getDirection();

// Block program unitl a button is pressed
unsigned char Pushbutton_getNumberWait();
unsigned char Pushbutton_getDirectionWait();

// Block program until a button is released
void Pushbutton_numberReleaseWait();
void Pushbutton_directionReleaseWait();

// Get last pushed button
unsigned char Pushbutton_getLastNumber();
unsigned char Pushbutton_getLastDirection();

#endif
