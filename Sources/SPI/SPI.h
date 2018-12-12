#ifndef SPI_H_
#define SPI_H_

void spi_initialize();
void spi_setup(unsigned char clkDiv,unsigned char cpol,unsigned char cpha) small;

unsigned char spi_send(unsigned char imm8) small;
unsigned char *spi_sendSeq(unsigned char *src,unsigned int len) small;
unsigned char spi_sendN(unsigned char imm8,unsigned int count) small;

unsigned char spi_recv() small;
unsigned char *spi_recvSeq(unsigned char *dst,unsigned int len) small;

void spi_waitFinish() small;

#endif
