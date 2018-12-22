#ifndef LCDCONFIG_H_
#define LCDCONFIG_H_

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

enum LCD_IAP_CONFIG{
    IAP_SECTOR=0,
    IAP_BRIGHTNESS_OFFSET=0,
    IAP_BRIGHTNESS_SIZE=2
};

#endif