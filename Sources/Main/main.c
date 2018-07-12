#include<Sources/Main/STC15W4K48S4.h>
#include<Sources/Universal/Universal.h>
#include<Sources/PWM/PWM.h>
#include<Sources/SPI/SPI.h>
#include<Sources/LCD12864/LCD12864.h>
#include<Sources/ADC/ADC.h>
#include<Sources/InterruptRoutine/InterruptRoutine.h>
#include<Sources/Joystick/Joystick.h>
#include<Sources/I23LC512/I23LC512.h>
#include<Sources/Timer/Timer4.h>
#include<Sources/Pushbutton/Pushbutton.h>

#include<stdio.h>
#include<math.h>

void uartInit(void){
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR &= 0xFB;		//定时器2时钟为Fosc/12,即12T
	T2L = 0xFA;		//设定定时初值
	T2H = 0xFF;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
    TI=1;
}

char code directionXYDelta[6][2]={
    {0,0},{0,1},{-1,0},{0,-1},{1,0},{0,0}
};

void main(){
    unsigned char i=31,j=63,dir;
    int a,b;
    unsigned char buffer[16];

    P0M0=0x00;
    P0M1=0x00;
    P1M0=0x00;
    P1M1=0x00;
    P2M0=0x00;
    P2M1=0x00;
    P3M0=0x00;
    P3M1=0x00;
    P4M0=0x00;
    P4M1=0x00;

    uartInit();
    printf("Hello World!\n");

    delay(0,0,0);
    interruptInitialize();
    spiInitialize(0,0,0);
    pwnInitialize(0,0);
    adcInitialize(0);

    i23lc512Initialize();
    lcd12864SpiInitialize();
    joystickInitialize();

    timer4Initialize(0,1,0x7e,0x66);  //1毫秒@33.1776MHz
    timer4Start();

    lcd12864CharSet(0,0,'A',1);
    lcd12864StringSet(7,0,"Hello World",1);

    while(delay(4,9,179)){
        // dir=joystickGetDirection();
        // a=abs(joystickGetX());
        // b=abs(joystickGetY());

        // if(dir){
        //     lcd12864PixelSet(i,j,0,0);
        //     i+=directionXYDelta[dir][0]*(a/128);
        //     j+=directionXYDelta[dir][1]*(b/128);
        //     lcd12864PixelSet(i,j,1,1);
        // }

        // sprintf(buffer,"%c %c",pushbuttonGet()+'0',pushbuttonLastPressedGet()+'0');

        buffer[0]=pushbuttonGet()+'0';
        buffer[1]=' ';
        buffer[2]=pushbuttonLastPressedGet()+'0';
        buffer[3]=0;
        lcd12864StringSet(1,0,buffer,1);
    }

    while(delay(0,0,0));
}

