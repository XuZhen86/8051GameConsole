#include"Sources/Main/STC15W4K48S4.h"
#include"Sources/PWM/PWM.h"
#include"Sources/Universal/Universal.h"
#include"Sources/SPI/SPI.h"
#include"Sources/Widgets/InputDialog/InputDialog.h"
#include"Sources/IAP/IAP.h"
#include"Sources/Delay/Delay.h"

#include"Sources/LCD/LCD.h"
#include"Sources/LCD/LCDConfig.h"
#include"Sources/LCD/LCD_ASCII6x8.h"

#include<stdio.h>
#include<string.h>

static unsigned char gdram[64][32];

void lcd_spi_send(unsigned char c,unsigned char b){
    spi_setup(SPI_CLKDIV,SPI_CPOL,SPI_CPHA);
    chipSelect=1;

    spi_send(b?0xfa:0xf8);
    spi_send(c&0xf0);
    spi_send(c<<4);

    spi_waitFinish();
    chipSelect=0;
    delayLoop(0,0,50);
}

void lcd_spi_send2Bytes(unsigned char c1,unsigned char c2,unsigned char b){
    spi_setup(SPI_CLKDIV,SPI_CPOL,SPI_CPHA);
    chipSelect=1;

    spi_send(b?0xfa:0xf8);
    spi_send(c1&0xf0);
    spi_send(c1<<4);
    spi_send(b?0xfa:0xf8);
    spi_send(c2&0xf0);
    spi_send(c2<<4);

    spi_waitFinish();
    chipSelect=0;
    delayLoop(0,0,50);
}

void lcd_hwReset(){
    resetSignal=0;
    delay(20);
    resetSignal=1;
    delay(20);
}

void lcd_pwm_initialize(){
    pwm3_initialize(0,lcd_iap_brightnessRawGet(),1,1);
}

void lcd_brightnessAdjust(){
    unsigned char levelNew,levelOld=lcd_brightnessLevelGet();

    levelNew=inputDialog_getUChar("Brightness",levelOld,0,16,1,lcd_brightnessLevelSet,1);
    if(levelNew==16){
        lcd_brightnessLevelSet(levelOld);
    }else if(levelNew!=levelOld){
        lcd_iap_write();
    }
}

unsigned char lcd_brightnessLevelGet(){
    return lcd_iap_brightnessRawGet()/0x800;
}

void lcd_brightnessLevelSet(unsigned char level){
    lcd_iap_brightnessRawSet(level%16*0x800);
}

void lcd_iap_read(){
    iap_sectorRead(IAP_SECTOR);
}

void lcd_iap_write(){
    iap_sectorWrite(IAP_SECTOR);
}

unsigned int lcd_iap_brightnessRawGet(){
    unsigned int brightnessRaw=iap_uIntGet(IAP_BRIGHTNESS_OFFSET);
    if(brightnessRaw==0xffff){
        iap_uIntSet(IAP_BRIGHTNESS_OFFSET,0x4000);
        return 0x4000;
    }
    return brightnessRaw;
}

unsigned int lcd_iap_brightnessRawSet(unsigned int brightnessRaw){
    brightnessRaw%=0x8000;
    if(brightnessRaw==0){
        brightnessRaw=1;
    }

    iap_uIntSet(IAP_BRIGHTNESS_OFFSET,brightnessRaw);
    pwm3_timerValueSet(0,brightnessRaw);
    return brightnessRaw;
}

void lcd_spi_initialize(){
    unsigned char i,j;

    chipSelect=0;
    P2M0=CHIP_SELECT_P2M0;
    P2M1=CHIP_SELECT_P2M1;

    lcd_hwReset();
    lcd_spi_send(FUNCTION_SET|0x04,0);
    for(i=0;i<32;i++){
        lcd_spi_send2Bytes(SET_GDRAM_ADDR|i,SET_GDRAM_ADDR,0);
        for(j=0;j<32;j+=2){
            lcd_spi_send2Bytes(BUFFER_INIT_VALUE,BUFFER_INIT_VALUE,1);
        }
    }
    lcd_spi_send(FUNCTION_SET|0x04|0x02,0);

    lcd_iap_read();
    lcd_pwm_initialize();
    memset(gdram,BUFFER_INIT_VALUE,sizeof gdram);
    lcd_forceFlush();
}

void lcd_flush(){
    unsigned char i,j,displayDisabled=0;

    for(i=0;i<64;i++){
        for(j=0;j<16;j+=2){
            if(gdram[i][j+0]!=gdram[i][j+16]||gdram[i][j+1]!=gdram[i][j+17]){
                if(!displayDisabled){
                    lcd_spi_send(FUNCTION_SET|0x04,0);
                    displayDisabled=1;
                }
                lcd_spi_send2Bytes(SET_GDRAM_ADDR|(i%32),SET_GDRAM_ADDR|((j/2)+8*(i>31)),0);
                lcd_spi_send2Bytes(gdram[i][j+0],gdram[i][j+1],1);
                gdram[i][j+16]=gdram[i][j+0];
                gdram[i][j+17]=gdram[i][j+1];
            }
        }
    }

    if(displayDisabled){
        lcd_spi_send(FUNCTION_SET|0x04|0x02,0);
    }
}

void lcd_forceFlush(){
    unsigned char i,j;

    lcd_spi_send(FUNCTION_SET|0x04,0);
    for(i=0;i<64;i++){
        lcd_spi_send2Bytes(SET_GDRAM_ADDR|(i%32),SET_GDRAM_ADDR|(8*(i>31)),0);
        for(j=0;j<16;j+=2){
            lcd_spi_send2Bytes(gdram[i][j+0],gdram[i][j+1],1);
        }
        memcpy(gdram[i]+16,gdram[i],16);
    }
    lcd_spi_send(FUNCTION_SET|0x04|0x02,0);
}

void lcd_charSet(unsigned char row,unsigned char col,unsigned char c){
    unsigned char i,mask,fill;
    col=col%21*6;   // col%=128;
    row=row%8*8;    // row%=64;
    mask=(0xff>>(col%8));

    for(i=row;i<row+8;i++){
        fill=LCD_ASCII6x8[c][i%8];
        gdram[i][col/8+0]=gdram[i][col/8+0]&(~mask)|(fill>>(col%8));
        gdram[i][col/8+1]=gdram[i][col/8+1]&(mask)|(fill<<(8-col%8));
    }
}

void lcd_stringSet(unsigned char row,unsigned char col,unsigned char *str){
    unsigned char i,j,k,mask,fill;
    col=col%21*6;
    row=row%8*8;

    for(i=row;i<row+8;i++){
        for(j=0,k=col;str[j]&&k<128;j++){
            mask=(0xff>>(k%8));
            fill=LCD_ASCII6x8[str[j]][i%8];

            if(k/8<16){
                gdram[i][k/8+0]=gdram[i][k/8+0]&(~mask)|(fill>>(k%8));
            }
            if(k/8+1<16){
                gdram[i][k/8+1]=gdram[i][k/8+1]&(mask)|(fill<<(8-k%8));
            }

            k+=6;
        }
    }
}

void lcd_pixelSet(unsigned char row,unsigned char col,unsigned char lightUp){
    row%=64;
    col%=128;
    lightUp%=2;

    if(lightUp){
        gdram[row][col/8]|=(0x80>>col%8);
    }else{
        gdram[row][col/8]&=~(0x80>>col%8);
    }
}

void lcd_clear(){
    unsigned char i;
    for(i=0;i<64;i++){
        memset(gdram[i],BUFFER_INIT_VALUE,16);
    }
}

void lcd_hLineSet(unsigned char row,unsigned char lightUp){
    memset(gdram[row%64],0xff*(lightUp%2),16);
}

void lcd_vLineSet(unsigned char col,unsigned char lightUp){
    unsigned char i;
    col%=128;

    for(i=0;i<64;i++){
        if(lightUp){
            gdram[i][col/8]|=(0x80>>(col%8));
        }else{
            gdram[i][col/8]&=~(0x80>>(col%8));
        }
    }
}
