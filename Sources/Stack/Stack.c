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

unsigned int stack_spGet(){
    return sp;
}

unsigned int stack_bpGet(){
    return bp;
}

unsigned int push16(unsigned int imm16){
    return xRam_uIntWrite(sp-=2,imm16);
}

unsigned char push8(unsigned char imm8){
    return xRam_uCharWrite(sp-=1,imm8);
}

unsigned char *pushSeq(unsigned char *src,unsigned int len){
    sp-=len;
    xRam_uCharWriteSeq(src,sp,len);
    return src;
}

unsigned int pushN(unsigned int len){
    sp-=len;
    return sp;
}

unsigned int pop16(){
    sp+=2;
    return xRam_uIntRead(sp-2);
}

unsigned char pop8(){
    sp+=1;
    return xRam_uIntRead(sp-1);
}

unsigned char *popSeq(unsigned char *dst,unsigned int len){
    xRam_uCharReadSeq(dst,sp,len);
    sp+=len;
    return dst;
}

unsigned int popN(unsigned int len){
    sp+=len;
    return sp;
}

unsigned int r16(unsigned int offs16){
    return xRam_uIntRead(bp-offs16);
}

unsigned int r16a(unsigned int offs16,unsigned int idx){
    return xRam_uIntRead(bp-offs16+2*idx);
}

unsigned char r8(unsigned int offs16){
    return xRam_uCharRead(bp-offs16);
}

unsigned char r8a(unsigned int offs16,unsigned int idx){
    return xRam_uCharRead(bp-offs16+idx);
}

unsigned int w16(unsigned int offs16,unsigned int imm16){
    // printf("[w16 %u %u]",bp-offs16,imm16);
    return xRam_uIntWrite(bp-offs16,imm16);
}

unsigned int w16a(unsigned int offs16,unsigned int idx,unsigned int imm16){
    return xRam_uIntWrite(bp-offs16+2*idx,imm16);
}

unsigned char w8(unsigned int offs16,unsigned char imm8){
    return xRam_uCharWrite(bp-offs16,imm8);
}

unsigned char w8a(unsigned int offs16,unsigned int idx,unsigned char imm8){
    return xRam_uCharWrite(bp-offs16+idx,imm8);
}

unsigned int inc16(unsigned int offs16){
    return xRam_uIntWrite(bp-offs16,xRam_uIntRead(bp-offs16)+1);
}

unsigned int dec16(unsigned int offs16){
    return xRam_uIntWrite(bp-offs16,xRam_uIntRead(bp-offs16)-1);
}

unsigned char inc8(unsigned int offs16){
    return xRam_uCharWrite(bp-offs16,xRam_uCharRead(bp-offs16)+1);
}

unsigned char dec8(unsigned int offs16){
    return xRam_uCharWrite(bp-offs16,xRam_uCharRead(bp-offs16)-1);
}

unsigned int stack_memset(unsigned int offs16,unsigned char imm8,unsigned int len){
    xRam_memset(bp-offs16,imm8,len);
    return offs16;
}

void enter(unsigned int imm16){
    push16(bp);
    bp=sp;
    sp-=imm16;
}

void leave(){
    sp=bp;
    bp=pop16();
    // printf("[leave sp=%u bp=%u]\n",sp,bp);
}

// void stack_dump(){
//     unsigned int i;
//     for(i=bpMax-1;i>=sp-4;i--){
//         printf("[stack[0x%04x]=%bu]\n",i,xRam_uCharRead(i));
//         delay(20);
//     }
// }
