#ifndef PWM_XSFR_H_
#define PWM_XSFR_H_

//和PWM有关的2字节寄存器必须先写入高8位，再写入低8位。
//如果直接进行+=等运算，编译器会先计算低8位再计算高8位，造成输出LED闪烁。
//在这里给每个2字节寄存器分配一个缓冲变量。
//用户先用缓冲变量做运算，再一次性写入寄存器。
//这样保证写入高8位，再写入低8位。
static volatile unsigned int  xdata PWMCxsfr   _at_ 0xfff0;
static volatile unsigned char xdata PWMCKS     _at_ 0xfff2;
static volatile unsigned int  xdata PWMC;

static volatile unsigned int  xdata PWM2T1xsfr _at_ 0xff00;
static volatile unsigned int  xdata PWM2T2xsfr _at_ 0xff02;
static volatile unsigned char xdata PWM2CR     _at_ 0xff04;
static volatile unsigned int  xdata PWM2T1;
static volatile unsigned int  xdata PWM2T2;

static volatile unsigned int  xdata PWM3T1xsfr _at_ 0xff10;
static volatile unsigned int  xdata PWM3T2xsfr _at_ 0xff12;
static volatile unsigned char xdata PWM3CR     _at_ 0xff14;
static volatile unsigned int  xdata PWM3T1;
static volatile unsigned int  xdata PWM3T2;

static volatile unsigned int  xdata PWM4T1xsfr _at_ 0xff20;
static volatile unsigned int  xdata PWM4T2xsfr _at_ 0xff22;
static volatile unsigned char xdata PWM4CR     _at_ 0xff24;
static volatile unsigned int  xdata PWM4T1;
static volatile unsigned int  xdata PWM4T2;

static volatile unsigned int  xdata PWM5T1xsfr _at_ 0xff30;
static volatile unsigned int  xdata PWM5T2xsfr _at_ 0xff32;
static volatile unsigned char xdata PWM5CR     _at_ 0xff34;
static volatile unsigned int  xdata PWM5T1;
static volatile unsigned int  xdata PWM5T2;

static volatile unsigned int  xdata PWM6T1xsfr _at_ 0xff40;
static volatile unsigned int  xdata PWM6T2xsfr _at_ 0xff42;
static volatile unsigned char xdata PWM6CR     _at_ 0xff44;
static volatile unsigned int  xdata PWM6T1;
static volatile unsigned int  xdata PWM6T2;

static volatile unsigned int  xdata PWM7T1xsfr _at_ 0xff50;
static volatile unsigned int  xdata PWM7T2xsfr _at_ 0xff52;
static volatile unsigned char xdata PWM7CR     _at_ 0xff54;
static volatile unsigned int  xdata PWM7T1;
static volatile unsigned int  xdata PWM7T2;

#endif
