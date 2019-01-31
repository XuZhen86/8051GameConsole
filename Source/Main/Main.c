#include<_ALL.h>

static void init(){
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

    Serial1_init();
    SPI_init();
    Far_init();
    Analog_init();
    IAP_init();
    Time_init();
    LCD_init();

    Debug(INFO,"Init complete");
}

static ListWidget *lw;

void main(){
    init();

    Debug(INFO,"Start main()");

    lw=ListWidget_new();
    ListWidget_setTitle(lw,"Main");
    ListWidget_addItem(lw,ListWidgetItem_new("Snake",ITEM_SELECTABLE));
    ListWidget_addItem(lw,ListWidgetItem_new("Brightness",ITEM_SELECTABLE));
    ListWidget_addItem(lw,ListWidgetItem_new("Version",ITEM_SELECTABLE));
    ListWidget_addItem(lw,ListWidgetItem_new("Input Dialog",ITEM_SELECTABLE));
    ListWidget_addItem(lw,ListWidgetItem_new("Debug",ITEM_SELECTABLE));
    ListWidget_addItem(lw,ListWidgetItem_new("Far Speed Test",ITEM_SELECTABLE));

    ListWidget_addItem(lw,ListWidgetItem_new("En",0));
    ListWidget_addItem(lw,ListWidgetItem_new("En|Se",ITEM_SELECTABLE));
    ListWidget_addItem(lw,ListWidgetItem_new("En|Ck",ITEM_CHECKABLE));
    ListWidget_addItem(lw,ListWidgetItem_new("En|Ck|Ckd",ITEM_CHECKABLE|ITEM_CHECKED));
    ListWidget_addItem(lw,ListWidgetItem_new("En|Se|Ck",ITEM_SELECTABLE|ITEM_CHECKABLE));
    ListWidget_addItem(lw,ListWidgetItem_new("En|Se|Ck|Ckd",ITEM_SELECTABLE|ITEM_CHECKABLE|ITEM_CHECKED));
    ListWidget_enableAll(lw);

    while(1){
        switch(ListWidget_getSelection(lw)){
            case 0:
                Snake();
                break;
            case 1:
                LCD_adjustBrightness();
                break;
            case 2:
                Version_show();
                break;
            case 3:
                InputDialog_test();
                break;
            case 4:
                Debug_show();
                break;
            case 5:
                Far_speedTest();
                break;
            default:
                ;
        }
    }
}
