#include"Sources/XRAM/XRAM.h"
#include<stdio.h>
#include"Sources/Delay/Delay.h"

#include"Sources/Stack/Stack.h"

// static unsigned int bpMax;
static unsigned int data bp,sp;

void stack_initialize(unsigned int m16Max){
    // bpMax=bp=sp=m16Max;
    bp=sp=m16Max;
}

unsigned int pushUI(unsigned int imm16){
    return xRam_uIntWrite(sp-=2,imm16);
}

unsigned char pushUC(unsigned char imm8){
    return xRam_uCharWrite(sp-=1,imm8);
}

unsigned int popUI(){
    sp+=2;
    return xRam_uIntRead(sp-2);
}

unsigned char popUC(){
    sp+=1;
    return xRam_uIntRead(sp-1);
}

unsigned int rUI(unsigned int offs16){
    return xRam_uIntRead(bp-offs16);
}

unsigned int rUIa(unsigned int offs16,unsigned int idx){
    return xRam_uIntRead(bp-offs16+2*idx);
}

unsigned char rUC(unsigned int offs16){
    return xRam_uCharRead(bp-offs16);
}

unsigned char rUCa(unsigned int offs16,unsigned int idx){
    return xRam_uCharRead(bp-offs16+idx);
}

unsigned int wUI(unsigned int offs16,unsigned int imm16){
    // printf("[wUI %u %u]",bp-offs16,imm16);
    return xRam_uIntWrite(bp-offs16,imm16);
}

unsigned int wUIa(unsigned int offs16,unsigned int idx,unsigned int imm16){
    return xRam_uIntWrite(bp-offs16+2*idx,imm16);
}

unsigned char wUC(unsigned int offs16,unsigned char imm8){
    return xRam_uCharWrite(bp-offs16,imm8);
}

unsigned char wUCa(unsigned int offs16,unsigned int idx,unsigned char imm8){
    return xRam_uCharWrite(bp-offs16+idx,imm8);
}

void enter(unsigned int imm16){
    pushUI(bp);
    bp=sp;
    sp-=imm16;
    // printf("[enter %u sp=%u bp=%u]\n",imm16,sp,bp);
}

void leave(){
    sp=bp;
    bp=popUI();
    // printf("[leave sp=%u bp=%u]\n",sp,bp);
}

// void stack_dump(){
//     unsigned int i;
//     for(i=bpMax-1;i>=sp-4;i--){
//         printf("[stack[0x%04x]=%bu]\n",i,xRam_uCharRead(i));
//         delay(20);
//     }
// }
