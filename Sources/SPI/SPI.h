#ifndef SPI_H_
#define SPI_H_

void spi_initialize();
void spi_setup(unsigned char clkDiv,bit cpol,bit cpha);

unsigned char spi_send(unsigned char imm8);
unsigned char *spi_sendSeq(unsigned char *src,unsigned int len);
unsigned char spi_recv();
unsigned char *spi_recvSeq(unsigned char *dst,unsigned int len);

void spi_waitFinish();

#endif
