#ifndef SPI_H_
#define SPI_H_

#include<Sources/Main/STC15W4K48S4.h>
#include<stdio.h>

bit spi_transmissionCompleteGet();
void spi_isOccupiedSet(bit occupied);
bit spi_isOccupiedGet();
void spi_initialize(bit cpol,bit cpha,unsigned char clkDiv);
void spi_setup(bit cpol,bit cpha,unsigned char clkDiv);
unsigned char spi_send(unsigned char c);
unsigned char spi_recv();
unsigned char *spi_seqRecv(unsigned char *destination,unsigned int length);

#endif
