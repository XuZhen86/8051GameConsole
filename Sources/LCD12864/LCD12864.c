#include"Sources/LCD12864/LCD12864.h"
#include"Sources/LCD12864/LCD12864_ASCII5x8.h"

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
    GDRAM_ADDR=0xf800
};

static unsigned int brightness=0x3fff;
static unsigned char gdramRowDirty[4]={0,0,0,0};

sbit chipSelect=P2^7;
sbit resetSignal=P2^0;

void lcd12864_spi_send(bit b,unsigned char c){
    spi_setup(SPI_CPOL,SPI_CPHA,SPI_CLKDIV);
    spi_isOccupiedSet(1);

    chipSelect=1;

    spi_send(b?0xfa:0xf8);
    spi_send(c&0xf0);
    spi_send(c<<4);

    while(!spi_transmissionCompleteGet());
    chipSelect=0;
    delayBusy(0,0,50);

    spi_isOccupiedSet(0);
}

void lcd12864_spi_send2Bytes(bit b,unsigned char c1,unsigned char c2){
    spi_setup(SPI_CPOL,SPI_CPHA,SPI_CLKDIV);
    spi_isOccupiedSet(1);

    chipSelect=1;

    spi_send(b?0xfa:0xf8);
    spi_send(c1&0xf0);
    spi_send(c1<<4);
    spi_send(b?0xfa:0xf8);
    spi_send(c2&0xf0);
    spi_send(c2<<4);

    while(!spi_transmissionCompleteGet());
    chipSelect=0;
    delayBusy(0,0,50);

    spi_isOccupiedSet(0);
}

void lcd12864_hwReset(){
    resetSignal=0;
    delayBusy(1,146,229);
    resetSignal=1;
    delayBusy(1,146,229);
}

void lcd12864_pwmInitialize(){
    pwm_3_initialize(1,1,0,0,0,brightness);
    pwm_start();
}

void lcd12864_brightnessSet(unsigned int b){
    if(b==0){
        b=1;
    }
    brightness=b;
    pwm_3_timerValueSet(0,b);
}

unsigned int lcd12864_brightnessGet(){
    return brightness;
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

    lcd12864_pwmInitialize();
    i23lc512_memset(GDRAM_ADDR,BUFFER_INIT_VALUE,32*32*2);
}

bit lcd12864_flush(bit forceFlush){
    unsigned char buffer[64];
    unsigned char data i,j;
    bit graphicDisplayDisabled=0,addressJustBeenSent=0;

    for(i=0;i<32;i++){
        if(forceFlush||gdramRowDirty[i/8]&(1<<(i%8))){
            i23lc512_uCharSeqRead(buffer,GDRAM_ADDR+64*i,64);

            for(j=0;j<32;j+=2){
                if(forceFlush||buffer[j+0]!=buffer[j+32]||buffer[j+1]!=buffer[j+33]){
                    if(!graphicDisplayDisabled){
                        graphicDisplayDisabled=1;
                        lcd12864_spi_send(0,FUNCTION_SET|0x04);
                    }

                    if(!addressJustBeenSent||forceFlush){
                        lcd12864_spi_send2Bytes(0,SET_GDRAM_ADDR|i,SET_GDRAM_ADDR|j/2);
                        addressJustBeenSent=1;
                    }

                    lcd12864_spi_send2Bytes(1,buffer[j+32],buffer[j+33]);
                }else{
                    addressJustBeenSent=0;
                }
            }
            i23lc512_uCharSeqWrite(buffer+32,GDRAM_ADDR+64*i,32);
        }
    }

    if(graphicDisplayDisabled){
        lcd12864_spi_send(0,FUNCTION_SET|0x04|0x02);
    }

    memset(gdramRowDirty,0,sizeof(gdramRowDirty));

    return 1;
}

void lcd12864_charSet(unsigned char row,unsigned char col,unsigned char c){
    unsigned char buffer[4];
    unsigned char data i,tempChar;

    col=col%25*5;
    row=row%8*8;
    if(row>31){col+=128;}
    row%=32;

    for(i=row;i<row+8;i++){
        i23lc512_uCharSeqRead(buffer,GDRAM_ADDR+64*i+32+col/8,2);

        tempChar=buffer[0];
        tempChar&=(0xff<<(8-col%8));
        tempChar|=(LCD12864_ASCII5x8[c][i%8]>>(col%8));
        buffer[2]=tempChar;

        tempChar=buffer[1];
        tempChar&=(0xff>>(col%8));
        tempChar|=(LCD12864_ASCII5x8[c][i%8]<<(8-col%8));
        buffer[3]=tempChar;

        if(buffer[0]!=buffer[2]||buffer[1]!=buffer[3]){
            i23lc512_uCharSeqWrite(buffer+2,GDRAM_ADDR+64*i+32+col/8,2);
            gdramRowDirty[i/8]|=(1<<(i%8));
        }
    }
}

void lcd12864_stringSet(unsigned char row,unsigned char col,unsigned char *str){
    unsigned char buffer[32];
    unsigned char data i,j,k,tempChar;

    col=col%25*5;
    row=row%8*8;
    if(row>31){col+=128;}
    row%=32;

    for(i=row;i<row+8;i++){
        i23lc512_uCharSeqRead(buffer,GDRAM_ADDR+64*i+32,32);
        k=col;
        for(j=0;str[j];j++){
            tempChar=buffer[k/8];
            tempChar&=(0xff<<(8-k%8));
            tempChar|=(LCD12864_ASCII5x8[str[j]][i%8]>>(k%8));
            buffer[k/8]=tempChar;

            tempChar=buffer[k/8+1];
            tempChar&=(0xff>>(k%8));
            tempChar|=(LCD12864_ASCII5x8[str[j]][i%8]<<(8-k%8));
            buffer[k/8+1]=tempChar;

            k+=5;
        }
        i23lc512_uCharSeqWrite(buffer,GDRAM_ADDR+64*i+32,32);
        gdramRowDirty[i/8]|=(1<<(i%8));
    }
}

void lcd12864_pixelSet(unsigned char row,unsigned char col,bit lightUp){
    unsigned char data buffer[2];

    row%=64;
    col%=128;
    if(row>31){col+=128;}
    row%=32;

    buffer[0]=i23lc512_uCharRead(GDRAM_ADDR+64*row+32+col/8);

    if(lightUp){
        buffer[1]=buffer[0]|(0x80>>col%8);
    }else{
        buffer[1]=buffer[0]&(~(0x80>>col%8));
    }

    if(buffer[0]!=buffer[1]){
        i23lc512_uCharWrite(GDRAM_ADDR+64*row+32+col/8,buffer[1]);
        gdramRowDirty[row/8]|=(1<<(row%8));
    }
}

void lcd12864_clear(){
    unsigned char i;
    for(i=0;i<32;i++){
        i23lc512_memset(GDRAM_ADDR+64*i+32,BUFFER_INIT_VALUE,32);
    }
    memset(gdramRowDirty,0xff,sizeof(gdramRowDirty));
}
