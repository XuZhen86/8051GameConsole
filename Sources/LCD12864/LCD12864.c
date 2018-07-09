#include"LCD12864.h"
#include"LCD12864_ASCII5x8.h"

static unsigned char code
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
    SET_GDRAM_ADDR=0x80;

static unsigned char code
    SPI_CPOL=1,
    SPI_CPHA=1,
    SPI_CLKDIV=3;

static unsigned char code
    BUFFER_INIT_VALUE=0x00,
    CHIP_SELECT_P2M0=0x40,
    CHIP_SELECT_P2M1=0x00;

static unsigned char xdata gdramBuffer[32][32][2];
static bit gdramNeedsFlush,gdramAccessLock=1;
static unsigned int brightness=0x0fff;

sbit chipSelect=P2^6;
sbit resetSignal=P2^7;

void lcd12864SpiSend(bit b,unsigned char c){
    spiSetup(SPI_CPOL,SPI_CPHA,SPI_CLKDIV);
    spiSetIsOccupied(1);

    chipSelect=1;

    spiSend(b?0xfa:0xf8);
    spiSend(c&0xf0);
    spiSend(c<<4);

    while(!spiTransmissionComplete());
    chipSelect=0;
    delayBusy(0,0,50);

    spiSetIsOccupied(0);
}

void lcd12864SpiSend2Bytes(bit b,unsigned char c1,unsigned char c2){
    spiSetup(SPI_CPOL,SPI_CPHA,SPI_CLKDIV);
    spiSetIsOccupied(1);

    chipSelect=1;

    spiSend(b?0xfa:0xf8);
    spiSend(c1&0xf0);
    spiSend(c1<<4);
    spiSend(b?0xfa:0xf8);
    spiSend(c2&0xf0);
    spiSend(c2<<4);

    while(!spiTransmissionComplete());
    chipSelect=0;
    delayBusy(0,0,50);

    spiSetIsOccupied(0);
}

void lcd12864HwReset(){
    gdramAccessLock=1;

    resetSignal=0;
    delayBusy(1,146,229);

    resetSignal=1;
    delayBusy(1,146,229);

    gdramAccessLock=0;
}

void lcd12864SpiInitialize(){
    unsigned char i,j;

    chipSelect=0;
    P2M0=CHIP_SELECT_P2M0;
    P2M1=CHIP_SELECT_P2M1;

    lcd12864HwReset();
    lcd12864SpiSend(0,FUNCTION_SET|0x04);
    for(i=0;i<32;i++){
        lcd12864SpiSend2Bytes(0,SET_GDRAM_ADDR|i,SET_GDRAM_ADDR);
        for(j=0;j<32;j+=2){
            gdramBuffer[i][j][0]=BUFFER_INIT_VALUE;
            gdramBuffer[i][j][1]=BUFFER_INIT_VALUE;
            gdramBuffer[i][j+1][0]=BUFFER_INIT_VALUE;
            gdramBuffer[i][j+1][1]=BUFFER_INIT_VALUE;
            lcd12864SpiSend2Bytes(1,gdramBuffer[i][j][0],gdramBuffer[i][j+1][0]);
        }
    }
    lcd12864SpiSend(0,FUNCTION_SET|0x04|0x02);

    lcd12864PwmInitialize();
    // lcd12864TimerInitialize();

    gdramAccessLock=0;
}

void lcd12864PwmInitialize(){
    pwm3Initialize(1,1,0,0,0,brightness);    // brightness
    pwmStart();
}

// void lcd12864TimerInitialize(){
//     timer4Initialize(1,0x3c,0xb0);  // 20 ms @ 30MHz
//     timer4Start();
// }

void lcd12864GdramFlush(bit forceFlush){
    unsigned char i,j;
    bit graphicDisplayDisabled=0,addressJustBeenSent=0;

    if(gdramNeedsFlush&&!gdramAccessLock){
        for(i=0;i<32;i++){
            for(j=0;j<32;j+=2){
                if(forceFlush||gdramBuffer[i][j][0]!=gdramBuffer[i][j][1]||gdramBuffer[i][j+1][0]!=gdramBuffer[i][j+1][1]){
                    if(!graphicDisplayDisabled){
                        graphicDisplayDisabled=1;
                        lcd12864SpiSend(0,FUNCTION_SET|0x04);
                    }

                    gdramBuffer[i][j][0]=gdramBuffer[i][j][1];
                    gdramBuffer[i][j+1][0]=gdramBuffer[i][j+1][1];

                    if(!addressJustBeenSent){
                        lcd12864SpiSend2Bytes(0,SET_GDRAM_ADDR|i,SET_GDRAM_ADDR|j/2);
                        addressJustBeenSent=1;
                    }

                    lcd12864SpiSend2Bytes(1,gdramBuffer[i][j][0],gdramBuffer[i][j+1][0]);
                }else{
                    addressJustBeenSent=0;
                }
            }
        }

        if(graphicDisplayDisabled){
            lcd12864SpiSend(0,FUNCTION_SET|0x04|0x02);
        }
        gdramNeedsFlush=0;
    }
}

void brightnessSet(unsigned int brightness){
    if(brightness==0){
        brightness=1;
    }
    brightness=brightness;
    pwm3TimerValueSet(0,brightness);
}

unsigned int brightnessGet(){
    return brightness;
}

void lcd12864CharSet(unsigned char row,unsigned char col,unsigned char c,bit flush){
    unsigned char i;
    gdramAccessLock=1;
    gdramNeedsFlush|=flush;

    col=col%25*5;
    row=row%8*8;
    col+=(row>31)*128;
    row%=32;

    for(i=row;i<row+8;i++){
        gdramBuffer[i][col/8][1]&=(0xff<<(8-col%8));
        gdramBuffer[i][col/8][1]|=(ASCII5x8[c][i%8]>>(col%8));

        gdramBuffer[i][col/8+1][1]&=(0xff>>(col%8));
        gdramBuffer[i][col/8+1][1]|=(ASCII5x8[c][i%8]<<(8-col%8));
    }

    gdramAccessLock=0;
}

void lcd12864StringSet(unsigned char row,unsigned char col,unsigned char *str,bit flush){
    unsigned int i;
    for(i=0;str[i];i++){
        lcd12864CharSet(row,col+i,str[i],flush);
    }
}

void lcd12864PixelSet(unsigned char row,unsigned char col,bit lightUp,bit flush){
    gdramAccessLock=1;
    gdramNeedsFlush|=flush;

    row%=64;
    col%=128;
    col+=(row>31)*128;
    row%=32;

    if(lightUp){
        gdramBuffer[row][col/8][1]|=(0x80>>col%8);
    }else{
        gdramBuffer[row][col/8][1]&=~(0x80>>col%8);
    }

    gdramAccessLock=0;
}

bit lcd12864PixelGet(unsigned char row,unsigned char col,bit frame){
    row%=64;
    col%=128;
    col+=(row>31)*128;
    row%=32;

    return gdramBuffer[row][col/8][frame]&(0x80>>col%8)!=0;
}

void lcd12864FlushAtNextUpdate(){
    gdramNeedsFlush=1;
}
