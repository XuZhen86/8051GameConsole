#include<Sources/Main/STC15W4K48S4.h>
#include<Sources/Version/Version.h>
#include<Sources/Universal/Universal.h>
#include<Sources/Universal/SystemClock.h>
#include<Sources/PWM/PWM.h>
#include<Sources/SPI/SPI.h>
#include<Sources/LCD12864/LCD12864.h>
#include<Sources/ADC/ADC.h>
#include<Sources/InterruptRoutine/InterruptRoutine.h>
#include<Sources/I23LC512/I23LC512.h>
#include<Sources/Timer/Timer4.h>
#include<Sources/Pushbutton/Pushbutton.h>
#include<Sources/Games/Snake/Snake.h>
#include<Sources/Widgets/ListWidget/ListWidget.h>

#include<stdio.h>
#include<math.h>

void uartInit(void){
    SCON = 0x50;        //8位数据,可变波特率
    AUXR |= 0x01;       //串口1选择定时器2为波特率发生器
    AUXR &= 0xFB;       //定时器2时钟为Fosc/12,即12T
    T2L = 0xFA;         //设定定时初值
    T2H = 0xFF;         //设定定时初值
    AUXR |= 0x10;       //启动定时器2
    TI=1;
}

void initialize(){
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
    // puts(COMPILE_DATE);
    // puts(COMPILE_TIME);
    // puts(uitoa(C51_VERSION,buffer));

    delay(0,0,0);
    interrupt_initialize();
    spi_initialize(0,0,0);
    pwn_initialize(0,0);
    adc_initialize(0);

    i23lc512_initialize();
    lcd12864_spi_initialize();

    timer_4_initialize(1,1,0x93,0xd3);
    timer_4_start();
}

unsigned char code *MAIN_TITLE="Main";
unsigned char code MAIN_ITEM_COUNT=2;
unsigned char code *MAIN_ITEMS[]={"Snake","Version"};

void main(){
    initialize();

    while(delay(0,0,0)){
        switch(listWidget_selectFromList(MAIN_TITLE,MAIN_ITEMS,MAIN_ITEM_COUNT)){
            case 0:
                printf("[Main %bu]",0);
                snake_splashScreen();
                snake();
                break;
            case 1:
                printf("[Main %bu]",1);
                version_showVersion();
                break;

            case 2:
                break;
        }
    }
}

