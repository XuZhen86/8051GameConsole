#ifndef STACK_H_
#define STACK_H_

void stack_initialize(unsigned int m16Max);

unsigned int pushUI(unsigned int imm16);
unsigned char pushUC(unsigned char imm8);

unsigned int popUI();
unsigned char popUC();

unsigned int rUI(unsigned int offs16);
unsigned int rUIa(unsigned int offs16,unsigned int idx);
unsigned char rUC(unsigned int offs16);
unsigned char rUCa(unsigned int offs16,unsigned int idx);

unsigned int wUI(unsigned int offs16,unsigned int imm16);
unsigned int wUIa(unsigned int offs16,unsigned int idx,unsigned int imm16);
unsigned char wUC(unsigned int offs16,unsigned char imm8);
unsigned char wUCa(unsigned int offs16,unsigned int idx,unsigned char imm8);

void enter(unsigned int imm16);
void leave();

// void stack_dump();

#endif
