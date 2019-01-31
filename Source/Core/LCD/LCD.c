#include"Font/LCDFont.h"
#include"LCDConfig.h"
#include"LCDStatic.h"
#include<Analog.h>
#include<Delay.h>
#include<IAPFile.h>
#include<InputDialog.h>
#include<LCD.h>
#include<SPI.h>
#include<STC15W4K48S4.h>
#include<stdio.h>
#include<string.h>

sbit chipSelect=P2^7;
sbit resetSignal=P2^0;

static unsigned char xdata gdram[64][32],brightness;

static void send(unsigned char c,unsigned char b){
    SPI_setClockDivider(SPI_CLOCK_DIV32);
    SPI_setDataMode(SPI_MODE3);
    chipSelect=1;

    SPI_transfer(b?0xfa:0xf8);
    SPI_transfer(c&0xf0);
    SPI_transfer(c<<4);

    chipSelect=0;
    SPI_setClockDivider(SPI_CLOCK_DIV4);
    SPI_setDataMode(SPI_MODE0);

    delayLoop(0,0,50);
}

static void send2(unsigned char c1,unsigned char c2,unsigned char b){
    SPI_setClockDivider(SPI_CLOCK_DIV32);
    SPI_setDataMode(SPI_MODE3);
    chipSelect=1;

    SPI_transfer(b?0xfa:0xf8);
    SPI_transfer(c1&0xf0);
    SPI_transfer(c1<<4);
    SPI_transfer(b?0xfa:0xf8);
    SPI_transfer(c2&0xf0);
    SPI_transfer(c2<<4);

    chipSelect=0;
    SPI_setClockDivider(SPI_CLOCK_DIV4);
    SPI_setDataMode(SPI_MODE0);

    delayLoop(0,0,50);
}

void LCD_hwReset(){
    resetSignal=0;
    delay(20);
    resetSignal=1;
    delay(20);
}

static void initAnalog(){
    IAPFile *file=IAPFile_new();
    char buffer[8];

    IAPFile_open(file,"LCD.txt");
    if(IAPFile_readLine(file,buffer,8)){
        sscanf(buffer,"%bu",&brightness);
        // Debug(DEBUG,"buffer=[%s] brightness=%bu\n",buffer,brightness);
    }else{
        IAPFile_write(file,"8\n",(unsigned char)strlen("8\n"));
        brightness=8;
    }
    IAPFile_close(file);
    IAPFile_delete(file);

    LCD_setBrightness(brightness);
}

void LCD_setBrightness(int b){
    brightness=(unsigned char)b;
    Analog_write(3,(unsigned char)b*0x800,1);
}

void LCD_adjustBrightness(){
    IAPFile *file=IAPFile_new();
    VectorInt *v=VectorInt_new();
    InputDialog *id=InputDialog_new();
    unsigned char bOld=brightness,bNew;
    char buffer[8];
    unsigned char i;


    for(i=0;i<16;i++){
        VectorInt_add(v,(int)i);
    }
    InputDialog_setValues(id,v);
    InputDialog_setTitle(id,"Brightness");
    InputDialog_setSigValueChanged(id,LCD_setBrightness);
    InputDialog_setSelected(id,(unsigned int)brightness);

    bNew=(unsigned char)InputDialog_getSelection(id);
    if(bNew==16){
        LCD_setBrightness(bOld);
    }else{
        sprintf(buffer,"%bu\n",bNew);
        IAPFile_open(file,"LCD.txt");
        IAPFile_write(file,buffer,(unsigned char)strlen(buffer));
        IAPFile_close(file);
    }

    InputDialog_delete(id);
    VectorInt_delete(v);
    IAPFile_delete(file);
}

void LCD_init(){
    unsigned char i,j;

    chipSelect=0;
    P2M0=CHIP_SELECT_P2M0;
    P2M1=CHIP_SELECT_P2M1;

    LCD_hwReset();
    send(FUNCTION_SET|0x04,0);
    for(i=0;i<32;i++){
        send2(SET_GDRAM_ADDR|i,SET_GDRAM_ADDR,0);
        for(j=0;j<32;j+=2){
            send2(BUFFER_INIT_VALUE,BUFFER_INIT_VALUE,1);
        }
    }
    send(FUNCTION_SET|0x04|0x02,0);

    memset(gdram,BUFFER_INIT_VALUE,sizeof gdram);
    LCD_forceFlush();

    initAnalog();
}

void LCD_flush(){ //lint -e661
    unsigned char i,j,displayDisabled=0;

    for(i=0;i<64;i++){
        for(j=0;j<16;j+=2){
            if(gdram[i][j+0]!=gdram[i][j+16]||gdram[i][j+1]!=gdram[i][j+17]){
                if(!displayDisabled){
                    send(FUNCTION_SET|0x04,0);
                    displayDisabled=1;
                }
                send2(SET_GDRAM_ADDR|(i%32),SET_GDRAM_ADDR|((j/2)+8*(i>31)),0);
                send2(gdram[i][j+0],gdram[i][j+1],1);
                gdram[i][j+16]=gdram[i][j+0];
                gdram[i][j+17]=gdram[i][j+1];
            }
        }
    }

    if(displayDisabled){
        send(FUNCTION_SET|0x04|0x02,0);
    }
}

void LCD_forceFlush(){
    unsigned char i,j;

    send(FUNCTION_SET|0x04,0);
    for(i=0;i<64;i++){
        send2(SET_GDRAM_ADDR|(i%32),SET_GDRAM_ADDR|(8*(i>31)),0);
        for(j=0;j<16;j+=2){
            send2(gdram[i][j+0],gdram[i][j+1],1);
        }
        memcpy(gdram[i]+16,gdram[i],16);
    }
    send(FUNCTION_SET|0x04|0x02,0);
}

void LCD_setChar(unsigned char row,unsigned char col,char c){
    unsigned char i,mask,fill;
    col=col%21*6;   // col%=128;
    row=row%8*8;    // row%=64;
    mask=(0xff>>(col%8));

    for(i=row;i<row+8;i++){
        fill=ASCII6x8[(unsigned char)c][i%8];
        gdram[i][col/8+0]=gdram[i][col/8+0]&(~mask)|(fill>>(col%8));
        gdram[i][col/8+1]=gdram[i][col/8+1]&(mask)|(fill<<(8-col%8));
    }
}

void LCD_setString(unsigned char row,unsigned char col,const char *str){
    unsigned char i,j,k,mask,fill;
    col=col%21*6;
    row=row%8*8;

    for(i=row;i<row+8;i++){
        for(j=0,k=col;str[j]&&k<128;j++){
            mask=(0xff>>(k%8));
            fill=ASCII6x8[(unsigned char)str[j]][i%8];

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

void LCD_setPixel(unsigned char row,unsigned char col,unsigned char lightUp){
    if(lightUp){
        gdram[row%64][col%128/8]|=(0x80>>(col%8));
    }else{
        gdram[row%64][col%128/8]&=~(0x80>>(col%8));
    }
}

void LCD_clear(){
    unsigned char i;
    for(i=0;i<64;i++){
        memset(gdram[i],BUFFER_INIT_VALUE,16);
    }
}

void LCD_setHLine(unsigned char row,unsigned char lightUp){
    memset(gdram[row%64],0xff*(lightUp%2),16);
}

void LCD_setVLine(unsigned char col,unsigned char lightUp){
    unsigned char i;

    for(i=0;i<64;i++){
        if(lightUp){
            gdram[i][col%128/8]|=(0x80>>(col%8));
        }else{
            gdram[i][col%128/8]&=~(0x80>>(col%8));
        }
    }
}

void LCD_invertHLine(unsigned char row){
    unsigned char i;
    row%=64;

    for(i=0;i<16;i++){
        gdram[row][i]=~gdram[row][i];
    }
}
