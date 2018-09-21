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

char code directionXYDelta[6][2]={
    {0,0},{0,1},{-1,0},{0,-1},{1,0},{0,0}
};

void main(){
    unsigned char buffer[16];
    unsigned char snakeErrorCode;

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
    puts(COMPILE_DATE);
    puts(COMPILE_TIME);
    puts(uitoa(C51_VERSION,buffer));

    delay(0,0,0);
    interrupt_initialize();
    spi_initialize(0,0,0);
    pwn_initialize(0,0);
    adc_initialize(0);

    i23lc512_initialize();
    lcd12864_spi_initialize();

    timer_4_initialize(1,1,0x93,0xd3);
    timer_4_start();

    lcd12864_stringSet(0,0,COMPILE_DATE);
    lcd12864_stringSet(1,0,COMPILE_TIME);
    lcd12864_stringSet(2,0,uitoa(C51_VERSION,buffer));

    lcd12864_flush(1);

    // while(lcd12864_flush(0)&&delay(5,9,179)){
    //     buffer[0]=pushbutton_numberGet()+'0';
    //     buffer[1]=0;
    //     // buffer[2]=pushbutton_lastPressedGet()+'0';
    //     // buffer[3]=0;

    //     lcd12864_stringSet(3,0,buffer);
    //     lcd12864_stringSet(4,0,uitoa(systemClock_mSecGet(),buffer));
    //     lcd12864_stringSet(5,0,uctoa(systemClock_secGet(),buffer));
    //     lcd12864_stringSet(6,0,uctoa(systemClock_minGet(),buffer));
    //     lcd12864_stringSet(7,0,uctoa(systemClock_hurGet(),buffer));

    //     lcd12864_charSet(7,8,'A');

    //     puts(buffer);
    // }

    // _pushbutton_directionButtonTest();

    // puts("snake_initialize");
    // puts(ultoa(systemClock_get(),buffer));
    // snake_initialize();

    // puts("snake_renewDisplay");
    // puts(ultoa(systemClock_get(),buffer));
    // snake_renewDisplay();

    // puts("lcd12864_flush");
    // puts(ultoa(systemClock_get(),buffer));
    // lcd12864_flush(0);

    snake_splashScreen();

    snakeErrorCode=snake();
    printf("snakeErrorCode=%u\n",(unsigned int)snakeErrorCode);

    while(delay(0,0,0));
}

