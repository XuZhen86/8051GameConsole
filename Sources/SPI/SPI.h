#ifndef SPI_H_
#define SPI_H_

void spi_initialize();
void spi_setup(unsigned char clkDiv,bit cpol,bit cpha);

unsigned char spi_send(unsigned char c);
unsigned char spi_recv();
unsigned char *spi_recvSeq(unsigned char *destination,unsigned int length);

void spi_waitFinish();

#endif
