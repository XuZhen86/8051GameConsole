#ifndef SPI_H_
#define SPI_H_

enum SPI_CONFIG{
    SPI_MSBFIRST=0,
    SPI_LSBFIRST=1,

    SPI_CLOCK_DIV4=0,
    SPI_CLOCK_DIV8=1,
    SPI_CLOCK_DIV16=2,
    SPI_CLOCK_DIV32=3,

    SPI_MODE0=0x00,
    SPI_MODE1=0x04,
    SPI_MODE2=0x08,
    SPI_MODE3=0x0c,
};

void SPI_init();

void SPI_setBitOrder(unsigned char bitOrder);
void SPI_setClockDivider(unsigned char clockDivider);
void SPI_setDataMode(unsigned char dataMode);

unsigned char SPI_transfer(unsigned char imm8);
void SPI_waitForFinish();

#endif
