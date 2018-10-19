#include<Sources/Main/STC15W4K48S4.h>
#include<Sources/Version/Version.h>
#include<Sources/Universal/Universal.h>
#include<Sources/Clock/Clock.h>
#include<Sources/PWM/PWM.h>
#include<Sources/SPI/SPI.h>
#include<Sources/LCD12864/LCD12864.h>
#include<Sources/ADC/ADC.h>
#include<Sources/XRAM/XRAM.h>
#include<Sources/Pushbutton/Pushbutton.h>
#include<Sources/Games/Snake/Snake.h>
#include<Sources/Widgets/ListWidget/ListWidget.h>
#include<Sources/IAP/IAP.h>
#include<Sources/Widgets/InputDialog/InputDialog.h>
#include<Sources/Serial/Serial.h>

#include<stdio.h>
#include<math.h>

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

    // delay(0,0,0);
    serial1_initialize(0xff,0xb8,1);
    spi_initialize();
    pwm_initialize(0);
    adc_initialize(0);
    iap_initialize();
    clock_initialize();

    xRam_initialize();
    lcd12864_spi_initialize();

    // while(1)
    // puts("Hello World!");
}

unsigned char code *MAIN_TITLE="Main";
unsigned char code MAIN_ITEM_COUNT=16;
unsigned char code *MAIN_ITEMS[]={
    "Snake",
    "Brightness",
    "Version",
    "Input Dialog",
    "Item 4",
    "Item 5",
    "Item 6",
    "Item 7",
    "Item 8",
    "Item 9",
    "Item 10",
    "Item 11",
    "Item 12",
    "Item 13",
    "Item 14",
    "Item 15"
};

void slotCurrentItemChanged(unsigned char item){
    printf("[main slotCurrentItemChanged %bu]\n",item);
}

void slotUCharValueChanged(unsigned char uChar){
    printf("[main slotUCharValueChanged %bu]",uChar);
}

void main(){
    initialize();

    while(delay(0,0,0)){
        switch(listWidget_selectFromList(MAIN_TITLE,MAIN_ITEMS,MAIN_ITEM_COUNT,slotCurrentItemChanged,0)){
            case 0:
                snake();
                break;
            case 1:
                lcd12864_brightnessAdjust();
                break;
            case 2:
                version_showVersion();
                break;
            case 3:
                inputDialog_getUChar("Input Dialog",8,0,128,20,slotUCharValueChanged,1);
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:
                break;
            case 10:
                break;
            case 11:
                break;
            case 12:
                break;
            case 13:
                break;
            case 14:
                break;
            case 15:
                break;
            case 16:
                break;
        }
    }
}
