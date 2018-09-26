#ifndef SPI_H_
#define SPI_H_

bit spi_transmissionCompleteGet();
void spi_isOccupiedSet(bit occupied);
bit spi_isOccupiedGet();
void spi_initialize(bit cpol,bit cpha,unsigned char clkDiv);
void spi_setup(bit cpol,bit cpha,unsigned char clkDiv);
unsigned char spi_send(unsigned char c);
unsigned char spi_recv();
unsigned char *spi_seqRecv(unsigned char *destination,unsigned int length);

#endif
