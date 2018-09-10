#include"Sources/InterruptRoutine/InterruptRoutine.h"

void interrupt_initialize(){
    EA=1;
    interrupt_busyLed=0;
}
