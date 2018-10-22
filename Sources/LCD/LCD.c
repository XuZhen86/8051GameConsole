#include"Sources/Main/STC15W4K48S4.h"
#include"Sources/PWM/PWM.h"
#include"Sources/Universal/Universal.h"
#include"Sources/SPI/SPI.h"
#include"Sources/XRAM/XRAM.h"
#include"Sources/Widgets/InputDialog/InputDialog.h"
#include"Sources/IAP/IAP.h"
#include"Sources/Delay/Delay.h"
#include"Sources/Stack/Stack.h"

#include"Sources/LCD/LCD.h"
#include"Sources/LCD/LCD_ASCII6x8.h"

#include<stdio.h>

enum LCD_COMMAND{
    CLEAR=0x01,
    STANDBY=0x01,
    HOME=0x02,
    RAM_ADDR_SELECT=0x02,
    ENTRY_MODE=0X04,
    DISPLAY_ON_OFF=0x08,
    CURSOR_DISPLAY_CONTROL=0x10,
    FUNCTION_SET=0x30,
    SET_CGRAM_ADDR=0x40,
    SET_DDRAM_ADDR=0x80,
    SET_GDRAM_ADDR=0x80
};

enum LCD_SPI_CONFIG{
    SPI_CPOL=1,
    SPI_CPHA=1,
    SPI_CLKDIV=3
};

enum LCD_OTHER_CONFIG{
    BUFFER_INIT_VALUE=0x00,
    CHIP_SELECT_P2M0=0xc0,
    CHIP_SELECT_P2M1=0x00
};

enum LCD_RAM_CONFIG{
    GDRAM_ADDR=0x0000,
};

enum LCD_IAP_CONFIG{
    IAP_SECTOR=0,
    IAP_BRIGHTNESS_OFFSET=0,
    IAP_BRIGHTNESS_SIZE=2
};

static unsigned char gdramRowDirty[4]={0,0,0,0};

sbit chipSelect=P2^7;
sbit resetSignal=P2^0;

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
    xRam_memset(GDRAM_ADDR,BUFFER_INIT_VALUE,32*32*2);
    lcd_forceFlush();
}

/**
 * The commented area seals a legendary bug.
 * Sealed by Xu Zhen @ 2018/09/25 8:34PM EDT
 */
void lcd_flush(){
    unsigned char buffer[64];
    unsigned char i,j;
    bit graphicDisplayDisabled=0/*,addressJustBeenSent=0*/;

    for(i=0;i<32;i++){
        if(gdramRowDirty[i/8]&(1<<(i%8))){
            xRam_uCharReadSeq(buffer,GDRAM_ADDR+64*i,64);

            for(j=0;j<32;j+=2){
                if(buffer[j+0]!=buffer[j+32]||buffer[j+1]!=buffer[j+33]){
                    if(!graphicDisplayDisabled){
                        graphicDisplayDisabled=1;
                        lcd_spi_send(FUNCTION_SET|0x04,0);
                    }

                    /*if(forceFlush||!addressJustBeenSent){
                        lcd_spi_send2Bytes(0,SET_GDRAM_ADDR|i,SET_GDRAM_ADDR|j/2);
                        addressJustBeenSent=1;
                    }*/

                    lcd_spi_send2Bytes(SET_GDRAM_ADDR|i,SET_GDRAM_ADDR|(j/2),0);
                    lcd_spi_send2Bytes(buffer[j+32],buffer[j+33],1);
                }/*else{
                    addressJustBeenSent=0;
                }*/
            }
            xRam_uCharWriteSeq(buffer+32,GDRAM_ADDR+64*i,32);
        }
    }

    if(graphicDisplayDisabled){
        lcd_spi_send(FUNCTION_SET|0x04|0x02,0);
    }

    lcd_clearAllRowDirtyFlags();
}

void lcd_forceFlush(){
    unsigned char buffer[32];
    unsigned char i,j;

    lcd_spi_send(FUNCTION_SET|0x04,0);
    for(i=0;i<32;i++){
        xRam_uCharReadSeq(buffer,GDRAM_ADDR+64*i+32,32);
        for(j=0;j<32;j+=2){
            lcd_spi_send2Bytes(SET_GDRAM_ADDR|i,SET_GDRAM_ADDR|(j/2),0);
            lcd_spi_send2Bytes(buffer[j],buffer[j+1],1);
        }
        xRam_uCharWriteSeq(buffer,GDRAM_ADDR+64*i,32);
    }
    lcd_spi_send(FUNCTION_SET|0x04|0x02,0);

    lcd_clearAllRowDirtyFlags();
}

void lcd_clearAllRowDirtyFlags(){
    gdramRowDirty[0]=0;
    gdramRowDirty[1]=0;
    gdramRowDirty[2]=0;
    gdramRowDirty[3]=0;
}

void lcd_setAllRowDirtyFlags(){
    gdramRowDirty[0]=0xff;
    gdramRowDirty[1]=0xff;
    gdramRowDirty[2]=0xff;
    gdramRowDirty[3]=0xff;
}

void lcd_charSet(unsigned char row,unsigned char col,unsigned char c){
    unsigned char buffer[4];
    unsigned char i,tempChar;

    col=col%21*6;
    row=row%8*8;
    if(row>31){col+=128;}
    row%=32;

    for(i=row;i<row+8;i++){
        xRam_uCharReadSeq(buffer,GDRAM_ADDR+64*i+32+col/8,2);

        tempChar=buffer[0];
        tempChar&=(0xff<<(8-col%8));
        tempChar|=(LCD_ASCII6x8[c][i%8]>>(col%8));
        buffer[2]=tempChar;

        tempChar=buffer[1];
        tempChar&=(0xff>>(col%8));
        tempChar|=(LCD_ASCII6x8[c][i%8]<<(8-col%8));
        buffer[3]=tempChar;

        if(buffer[0]!=buffer[2]||buffer[1]!=buffer[3]){
            xRam_uCharWriteSeq(buffer+2,GDRAM_ADDR+64*i+32+col/8,2);
            gdramRowDirty[i/8]|=(1<<(i%8));
        }
    }
}

void lcd_stringSet(unsigned char row,unsigned char col,unsigned char *str){
    unsigned char buffer[32];
    unsigned char data tempChar,mask,fill,i,j,k;
    unsigned char attr=0x00;    // bit 1: rowDirty, bit 0: row>=32

    col=col%21*6;
    row=row%8*8;
    if(row>31){
        col+=128;
        attr|=0x01;
        row-=32;
    }

    for(i=0;i<8;i++){
        xRam_uCharReadSeq(buffer+col/8,GDRAM_ADDR+64*(i+row)+32+col/8,32-col/8);
        attr&=~0x02;
        k=col;

        for(j=0;str[j]&&col<=k;j++){
            mask=0xff>>(k%8);
            fill=LCD_ASCII6x8[str[j]][i];

            if(k/8<32){
                tempChar=buffer[k/8]&(~mask)|(fill>>(k%8));
                if(tempChar!=buffer[k/8]){
                    buffer[k/8]=tempChar;
                    attr|=0x02;
                }
            }

            if(k/8+1<32){
                tempChar=buffer[k/8+1]&(mask)|(fill<<(8-k%8));
                if(tempChar!=buffer[k/8+1]){
                    buffer[k/8+1]=tempChar;
                    attr|=0x02;
                }
            }

            k+=6;
        }

        if(attr&0x02){
            xRam_uCharWriteSeq(buffer+col/8,GDRAM_ADDR+64*(i+row)+32+col/8,32-col/8);
            gdramRowDirty[row/8]|=(1<<i);
        }
    }
}

void lcd_pixelSet(unsigned char row,unsigned char col,unsigned char lightUp){
    unsigned char buffer[2];

    row%=64;
    col%=128;
    if(row>31){col+=128;}
    row%=32;

    buffer[0]=xRam_uCharRead(GDRAM_ADDR+64*row+32+col/8);

    if(lightUp){
        buffer[1]=buffer[0]|(0x80>>col%8);
    }else{
        buffer[1]=buffer[0]&(~(0x80>>col%8));
    }

    if(buffer[0]!=buffer[1]){
        xRam_uCharWrite(GDRAM_ADDR+64*row+32+col/8,buffer[1]);
        gdramRowDirty[row/8]|=(1<<(row%8));
    }
}

void lcd_clear(){
    unsigned char i;
    for(i=0;i<32;i++){
        xRam_memset(GDRAM_ADDR+64*i+32,BUFFER_INIT_VALUE,32);
    }
    lcd_setAllRowDirtyFlags();
}

void lcd_bufferStackPush(){
    unsigned char i;
    pushN(1024);
    for(i=0;i<32;i++){
        xRam_memcpy(stack_spGet()+32*i,GDRAM_ADDR+64*i,32);
    }
}

void lcd_bufferStackPop(){
    unsigned char i;
    for(i=0;i<32;i++){
        xRam_memcpy(GDRAM_ADDR+64*i+32,stack_spGet()+32*i,32);
    }
    lcd_setAllRowDirtyFlags();
    popN(1024);
}

// void lcd_rowReverse(unsigned char startRow,unsigned char endRow){
//     unsigned char buffer[16];
//     unsigned char data i,j;

//     startRow%=64;
//     endRow%=64;

//     for(i=startRow;i<endRow;i++){
//         if(i<32){
//             xRam_uCharReadSeq(buffer,GDRAM_ADDR+64*i+32,16);
//             for(j=0;j<16;j++){
//                 buffer[i]=~buffer[i];
//             }
//             xRam_uCharWriteSeq(buffer,GDRAM_ADDR+64*i+32,16);
//         }else{
//             xRam_uCharReadSeq(buffer,GDRAM_ADDR+64*i+48,16);
//             for(j=0;j<16;j++){
//                 buffer[i]=~buffer[i];
//             }
//             xRam_uCharWriteSeq(buffer,GDRAM_ADDR+64*i+48,16);
//         }
//     }
// }

void lcd_hLineSet(unsigned char row,unsigned char lightUp){
    row%=64;

    if(row<32){
        xRam_memset(GDRAM_ADDR+64*row+32,0xff*lightUp,16);
    }else{
        // row%=32;
        xRam_memset(GDRAM_ADDR+64*(row%32)+48,0xff*lightUp,16);
    }
    gdramRowDirty[row/8]|=(1<<(row%8));
}
