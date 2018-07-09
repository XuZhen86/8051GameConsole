#include<Sources/Main/STC15W4K48S4.h>

char putchar(const char c){
    while(!TI);
    SBUF=c;
    TI=0;
    return c;
}
