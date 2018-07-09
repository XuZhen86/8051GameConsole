#include"Timer.h"

// void timer3Initialize(bit x12Mode,unsigned char t3h,unsigned char t3l){
//     if(x12Mode){
//         T4T3M&=0xFD;
//     }
//     T3L=t3h;
//     T3H=t3l;
// }

// void timer3Start(){
//     IE2|=0x20;
//     T4T3M|=0x08;
// }

// void timer3Stop(){
//     T4T3M&=~0x08;
// }

void timer4Initialize(bit x12Mode,unsigned char t4h,unsigned char t4l){
    if(x12Mode){
        T4T3M&=0xDF;
    }else{
        T4T3M|=0x20;
    }
    T4L=t4h;
    T4H=t4l;
}

void timer4Start(){
    IE2|=0x40;
    T4T3M|=0x80;
}

// void timer4Stop(){
//     T4T3M&=~0x80;
// }
