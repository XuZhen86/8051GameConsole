#include<stdio.h>
#include<string.h>
#include"../Main/STC15W4K48S4.h"
#include"../PWM/PWM.h"
#include"../Universal/Universal.h"
#include"../SPI/SPI.h"
#include"../Widgets/InputDialog/InputDialog.h"
#include"../IAPFile/IAPFile.h"
#include"../Delay/Delay.h"
#include"./LCD.h"
#include"./LCDConfig.h"
#include"./LCD_ASCII6x8.h"

sbit chipSelect=P2^7;
sbit resetSignal=P2^0;

static unsigned char gdram[64][32],brightness;

void lcd_spi_send(unsigned char c,unsigned char b) small{
    spi_setup(SPI_CLKDIV,SPI_CPOL,SPI_CPHA);
    chipSelect=1;

    spi_send(b?0xfa:0xf8);
    spi_send(c&0xf0);
    spi_send(c<<4);

    spi_waitFinish();
    chipSelect=0;
    delayLoop(0,0,50);
    spi_setup(0,0,0);
}

void lcd_spi_send2Bytes(unsigned char c1,unsigned char c2,unsigned char b) small{
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
    spi_setup(0,0,0);
}

void lcd_hwReset(){
    resetSignal=0;
    delay(20);
    resetSignal=1;
    delay(20);
}

void lcd_pwm_initialize(){
    IapFile *file=IapFile_new();
    unsigned char buffer[8];

    IapFile_open(file,"LCD.txt");
    if(IapFile_readLine(file,buffer,8)){
        sscanf(buffer,"%bu",&brightness);
        // printf("buffer=[%s] brightness=%bu\n",buffer,brightness);
    }else{
        IapFile_write(file,"8\n",strlen("8\n"));
        brightness=8;
    }
    IapFile_close(file);
    IapFile_delete(file);

    pwm3_initialize(0,1,1,1);
    lcd_setBrightness(brightness);
}

void lcd_setBrightness(unsigned char b){
    brightness=b;
    if(b==0){
        pwm3_timerValueSet(0,1);
    }else{
        pwm3_timerValueSet(0,b*0x800);
    }
}

void lcd_adjustBrightness(){
    IapFile *file=IapFile_new();
    unsigned char bOld=brightness,bNew,buffer[8];

    bNew=inputDialog_getUChar("Brightness",brightness,0,16,1,lcd_setBrightness,0);
    if(bNew==16){
        lcd_setBrightness(bOld);
    }else{
        sprintf(buffer,"%bu\n",bNew);
        IapFile_open(file,"LCD.txt");
        IapFile_write(file,buffer,strlen(buffer));
    }

    IapFile_close(file);
    IapFile_delete(file);
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

    memset(gdram,BUFFER_INIT_VALUE,sizeof gdram);
    lcd_forceFlush();

    lcd_pwm_initialize();
}

void lcd_flush() small{
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

void lcd_forceFlush() small{
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

void lcd_charSet(unsigned char row,unsigned char col,unsigned char c) small{
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

void lcd_stringSet(unsigned char row,unsigned char col,unsigned char *str) small{
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

void lcd_pixelSet(unsigned char row,unsigned char col,unsigned char lightUp) small{
    if(lightUp){
        gdram[row%64][col%128/8]|=(0x80>>col%8);
    }else{
        gdram[row%64][col%128/8]&=~(0x80>>col%8);
    }
}

void lcd_clear() small{
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

    for(i=0;i<64;i++){
        if(lightUp){
            gdram[i][col%128/8]|=(0x80>>(col%8));
        }else{
            gdram[i][col%128/8]&=~(0x80>>(col%8));
        }
    }
}
