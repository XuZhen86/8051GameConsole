#include"Sources/Main/STC15W4K48S4.h"
#include"Sources/PWM/PWM.h"
#include"Sources/Universal/Universal.h"
#include"Sources/SPI/SPI.h"
#include"Sources/XRAM/XRAM.h"
#include"Sources/Widgets/InputDialog/InputDialog.h"
#include"Sources/IAP/IAP.h"

#include"Sources/LCD12864/LCD12864.h"
#include"Sources/LCD12864/LCD12864_ASCII6x8.h"

#include<stdio.h>

enum LCD12864_COMMAND{
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

enum LCD12864_SPI_CONFIG{
    SPI_CPOL=1,
    SPI_CPHA=1,
    SPI_CLKDIV=3
};

enum LCD12864_OTHER_CONFIG{
    BUFFER_INIT_VALUE=0x00,
    CHIP_SELECT_P2M0=0xc0,
    CHIP_SELECT_P2M1=0x00
};

enum LCD12864_RAM_CONFIG{
    GDRAM_ADDR=0xf800,
    GDRAM_STACK_ADDR=0xe000,
    GDRAM_STACK_MAX=6
};

enum LCD12864_IAP_CONFIG{
    IAP_SECTOR=0,
    IAP_BRIGHTNESS_OFFSET=0,
    IAP_BRIGHTNESS_SIZE=2
};

static unsigned char idata gdramRowDirty[4]={0,0,0,0};
static unsigned char bufferStack=0;

sbit chipSelect=P2^7;
sbit resetSignal=P2^0;

void lcd12864_spi_send(bit b,unsigned char c){
    spi_setup(SPI_CLKDIV,SPI_CPOL,SPI_CPHA);
    chipSelect=1;

    spi_send(b?0xfa:0xf8);
    spi_send(c&0xf0);
    spi_send(c<<4);

    spi_waitFinish();
    chipSelect=0;
    delayBusy(0,0,50);
}

void lcd12864_spi_send2Bytes(bit b,unsigned char c1,unsigned char c2){
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
    delayBusy(0,0,50);
}

void lcd12864_hwReset(){
    resetSignal=0;
    delayBusy(1,146,229);
    resetSignal=1;
    delayBusy(1,146,229);
}

void lcd12864_pwm_initialize(){
    pwm3_initialize(0,lcd12864_iap_brightnessRawGet(),1,1);
}

void lcd12864_brightnessAdjust(){
    unsigned char levelNew,levelOld=lcd12864_brightnessLevelGet();

    levelNew=inputDialog_getUChar("Brightness",levelOld,0,16,1,lcd12864_brightnessLevelSet,1);
    if(levelNew==16){
        lcd12864_brightnessLevelSet(levelOld);
    }else if(levelNew!=levelOld){
        lcd12864_iap_write();
    }
}

unsigned char lcd12864_brightnessLevelGet(){
    return lcd12864_iap_brightnessRawGet()/0x800;
}

void lcd12864_brightnessLevelSet(unsigned char level){
    lcd12864_iap_brightnessRawSet(level%16*0x800);
}

void lcd12864_iap_read(){
    iap_sectorRead(IAP_SECTOR);
}

void lcd12864_iap_write(){
    iap_sectorWrite(IAP_SECTOR);
}

unsigned int lcd12864_iap_brightnessRawGet(){
    unsigned int brightnessRaw=iap_uIntGet(IAP_BRIGHTNESS_OFFSET);
    if(brightnessRaw==0xffff){
        iap_uIntSet(IAP_BRIGHTNESS_OFFSET,0x4000);
        return 0x4000;
    }
    return brightnessRaw;
}

unsigned int lcd12864_iap_brightnessRawSet(unsigned int brightnessRaw){
    brightnessRaw%=0x8000;
    if(brightnessRaw==0){
        brightnessRaw=1;
    }

    iap_uIntSet(IAP_BRIGHTNESS_OFFSET,brightnessRaw);
    pwm3_timerValueSet(0,brightnessRaw);
    return brightnessRaw;
}

void lcd12864_spi_initialize(){
    unsigned char data i,j;

    chipSelect=0;
    P2M0=CHIP_SELECT_P2M0;
    P2M1=CHIP_SELECT_P2M1;

    lcd12864_hwReset();
    lcd12864_spi_send(0,FUNCTION_SET|0x04);
    for(i=0;i<32;i++){
        lcd12864_spi_send2Bytes(0,SET_GDRAM_ADDR|i,SET_GDRAM_ADDR);
        for(j=0;j<32;j+=2){
            lcd12864_spi_send2Bytes(1,BUFFER_INIT_VALUE,BUFFER_INIT_VALUE);
        }
    }
    lcd12864_spi_send(0,FUNCTION_SET|0x04|0x02);

    lcd12864_iap_read();
    lcd12864_pwm_initialize();
    // xRam_memset(GDRAM_ADDR,BUFFER_INIT_VALUE,32*32*2);
    xRam_memset(GDRAM_STACK_ADDR,BUFFER_INIT_VALUE,0x2000);
}

/**
 * The commented area seals a legendary bug.
 * Sealed by Xu Zhen @ 2018/09/25 8:34PM EDT
 */
bit lcd12864_flush(bit forceFlush){
    unsigned char buffer[64];
    unsigned char data i,j;
    bit graphicDisplayDisabled=0/*,addressJustBeenSent=0*/;

    for(i=0;i<32;i++){
        if(forceFlush||gdramRowDirty[i/8]&(1<<(i%8))){
            xRam_uCharSeqRead(buffer,GDRAM_ADDR+64*i,64);

            for(j=0;j<32;j+=2){
                if(forceFlush||buffer[j+0]!=buffer[j+32]||buffer[j+1]!=buffer[j+33]){
                    if(!graphicDisplayDisabled){
                        graphicDisplayDisabled=1;
                        lcd12864_spi_send(0,FUNCTION_SET|0x04);
                    }

                    /*if(forceFlush||!addressJustBeenSent){
                        lcd12864_spi_send2Bytes(0,SET_GDRAM_ADDR|i,SET_GDRAM_ADDR|j/2);
                        addressJustBeenSent=1;
                    }*/

                    lcd12864_spi_send2Bytes(0,SET_GDRAM_ADDR|i,SET_GDRAM_ADDR|(j/2));
                    lcd12864_spi_send2Bytes(1,buffer[j+32],buffer[j+33]);
                }/*else{
                    addressJustBeenSent=0;
                }*/
            }
            xRam_uCharSeqWrite(buffer+32,GDRAM_ADDR+64*i,32);
        }
    }

    if(graphicDisplayDisabled){
        lcd12864_spi_send(0,FUNCTION_SET|0x04|0x02);
    }

    for(i=0;i<4;i++){
        gdramRowDirty[i]=0;
    }

    return 1;
}

void lcd12864_charSet(unsigned char row,unsigned char col,unsigned char c){
    unsigned char buffer[4];
    unsigned char data i,tempChar;

    col=col%21*6;
    row=row%8*8;
    if(row>31){col+=128;}
    row%=32;

    for(i=row;i<row+8;i++){
        xRam_uCharSeqRead(buffer,GDRAM_ADDR+64*i+32+col/8,2);

        tempChar=buffer[0];
        tempChar&=(0xff<<(8-col%8));
        tempChar|=(LCD12864_ASCII6x8[c][i%8]>>(col%8));
        buffer[2]=tempChar;

        tempChar=buffer[1];
        tempChar&=(0xff>>(col%8));
        tempChar|=(LCD12864_ASCII6x8[c][i%8]<<(8-col%8));
        buffer[3]=tempChar;

        if(buffer[0]!=buffer[2]||buffer[1]!=buffer[3]){
            xRam_uCharSeqWrite(buffer+2,GDRAM_ADDR+64*i+32+col/8,2);
            gdramRowDirty[i/8]|=(1<<(i%8));
        }
    }
}

void lcd12864_stringSet(unsigned char row,unsigned char col,unsigned char *str){
    unsigned char buffer[16];
    unsigned char data i,j,k,tempChar[2];
    bit rowDirty,rowGe32=0; // row greater or equal 32

    col=col%21*6;
    row=row%8*8;
    if(row>31){col+=128;rowGe32=1;}
    row%=32;

    for(i=row;i<row+8;i++){
        xRam_uCharSeqRead(buffer,GDRAM_ADDR+64*i+32+16*rowGe32,16);
        rowDirty=0;
        k=col;

        for(j=0;str[j]&&col<=k;j++){
            tempChar[0]=tempChar[1]=buffer[k/8-16*rowGe32];
            tempChar[0]&=(0xff<<(8-k%8));
            if(str[j]!=' '){
                tempChar[0]|=(LCD12864_ASCII6x8[str[j]][i%8]>>(k%8));
            }
            if(tempChar[0]!=tempChar[1]){
                buffer[k/8-16*rowGe32]=tempChar[0];
                rowDirty=1;
            }

            tempChar[0]=tempChar[1]=buffer[k/8+1-16*rowGe32];
            tempChar[0]&=(0xff>>(k%8));
            if(str[j]!=' '){
                tempChar[0]|=(LCD12864_ASCII6x8[str[j]][i%8]<<(8-k%8));
            }
            if(tempChar[0]!=tempChar[1]){
                buffer[k/8+1-16*rowGe32]=tempChar[0];
                rowDirty=1;
            }

            k+=6;
        }

        if(rowDirty){
            xRam_uCharSeqWrite(buffer,GDRAM_ADDR+64*i+32+16*rowGe32,16);
            gdramRowDirty[i/8]|=(1<<(i%8));
        }
    }
}

void lcd12864_pixelSet(unsigned char row,unsigned char col,bit lightUp){
    unsigned char data buffer[2];

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

void lcd12864_clear(){
    unsigned char idata i;
    for(i=0;i<32;i++){
        xRam_memset(GDRAM_ADDR+64*i+32,BUFFER_INIT_VALUE,32);
    }

    for(i=0;i<4;i++){
        gdramRowDirty[i]=0xff;
    }
}

bit lcd12864_bufferStackPush(){
    unsigned char i;

    if(bufferStack==GDRAM_STACK_MAX){
        return 0;
    }else{
        for(i=0;i<32;i++){
            xRam_memcpy(GDRAM_STACK_ADDR+1024*bufferStack+64*i,GDRAM_ADDR+64*i,32);
        }
        bufferStack++;
        return 1;
    }
}

bit lcd12864_bufferStackPop(){
    unsigned char i;

    if(!bufferStack){
        return 0;
    }else{
        bufferStack--;
        for(i=0;i<32;i++){
            xRam_memcpy(GDRAM_ADDR+64*i+32,GDRAM_STACK_ADDR+1024*bufferStack+64*i,32);
        }
        for(i=0;i<4;i++){
            gdramRowDirty[i]=0xff;
        }
        return 1;
    }
}

void lcd12864_bufferStackClear(){
    bufferStack=0;
}

// void lcd12864_rowReverse(unsigned char startRow,unsigned char endRow){
//     unsigned char buffer[16];
//     unsigned char data i,j;

//     startRow%=64;
//     endRow%=64;

//     for(i=startRow;i<endRow;i++){
//         if(i<32){
//             xRam_uCharSeqRead(buffer,GDRAM_ADDR+64*i+32,16);
//             for(j=0;j<16;j++){
//                 buffer[i]=~buffer[i];
//             }
//             xRam_uCharSeqWrite(buffer,GDRAM_ADDR+64*i+32,16);
//         }else{
//             xRam_uCharSeqRead(buffer,GDRAM_ADDR+64*i+48,16);
//             for(j=0;j<16;j++){
//                 buffer[i]=~buffer[i];
//             }
//             xRam_uCharSeqWrite(buffer,GDRAM_ADDR+64*i+48,16);
//         }
//     }
// }

void lcd12864_hLineSet(unsigned char row,bit lightUp){
    row%=64;

    if(row<32){
        xRam_memset(GDRAM_ADDR+64*row+32,0xff*lightUp,16);
    }else{
        row%=32;
        xRam_memset(GDRAM_ADDR+64*row+48,0xff*lightUp,16);
    }
    gdramRowDirty[row/8]|=(1<<(row%8));
}
