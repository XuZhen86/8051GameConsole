#include<Sources/Main/STC15W4K48S4.h>
#include<Sources/Version/Version.h>
#include<Sources/Universal/Universal.h>
#include<Sources/Clock/Clock.h>
#include<Sources/PWM/PWM.h>
#include<Sources/SPI/SPI.h>
#include<Sources/LCD/LCD.h>
#include<Sources/ADC/ADC.h>
#include<Sources/XRAM/XRAM.h>
#include<Sources/Pushbutton/Pushbutton.h>
#include<Sources/Games/Snake/Snake.h>
#include<Sources/Widgets/ListWidget/ListWidget.h>
#include<Sources/IAP/IAP.h>
#include<Sources/Widgets/InputDialog/InputDialog.h>
#include<Sources/Serial/Serial.h>
#include<Sources/Stack/Stack.h>
#include<Sources/FarMem/FarMem.h>
#include<Sources/IAPFile/IAPFile.h>

#include<stdio.h>
#include<string.h>

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
    EA=1;

    serial1_initialize(0xff,0xb8,1);
    spi_initialize();
    xRam_initialize();
    farMem_Initialize();
    pwm_initialize(0);
    adc_initialize(0);
    Iap_initialize();
    clock_initialize();

    stack_initialize(0x0000);
    lcd_spi_initialize();
}

void slotCurrentItemChanged(unsigned char item){
    printf("[main slotCurrentItemChanged %bu]\n",item);
}

// void slotUCharValueChanged(unsigned char uChar){
//     printf("[main slotUCharValueChanged %bu]",uChar);
// }

ListWidget *mainListWidget;

void main(){
    initialize();

    mainListWidget=ListWidget_new();
    ListWidget_setTitle(mainListWidget,"Main");
    ListWidget_addItem(mainListWidget,"Snake");
    ListWidget_addItem(mainListWidget,"Brightness");
    ListWidget_addItem(mainListWidget,"Version");
    ListWidget_addItem(mainListWidget,"Input Dialog");
    ListWidget_addItem(mainListWidget,"bead94c4-5f1f-4757-b488-b99ad8528078");
    ListWidget_addItem(mainListWidget,"7c7811e5-89bb-4f44-8e74-cd1358cbad3c");
    ListWidget_addItem(mainListWidget,"4522629a-0ebb-4153-9045-5d55f23aac85");
    ListWidget_addItem(mainListWidget,"1e85499b-9108-44c8-8f71-00d073c65783");
    ListWidget_addItem(mainListWidget,"fd0cf450-c49d-43df-8cb9-2cff7c9192e6");
    ListWidget_addItem(mainListWidget,"be5cb133-3f62-411d-9c08-11948589c17b");
    ListWidget_addItem(mainListWidget,"53f85aa2-5dea-4045-988d-0229b5f3ca47");
    ListWidget_setSigCurrentItemChanged(mainListWidget,slotCurrentItemChanged);

    while(1){
        switch(ListWidget_getSelection(mainListWidget)){
            case 0:
                snake();
                break;
            case 1:
                lcd_adjustBrightness();
                break;
            case 2:
                version_showVersion();
                break;
            case 3:
                // inputDialog_getUChar("Input Dialog",8,0,128,20,slotUCharValueChanged,1);
                break;
        }
    }
}
