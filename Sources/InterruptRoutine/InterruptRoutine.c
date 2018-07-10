#include"InterruptRoutine.h"

void interruptInitialize(){
    EA=1;
    interruptBusyLed=0;
}
