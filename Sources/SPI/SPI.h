#ifndef __SPI_H_
#define __SPI_H_

#include<Sources/Main/STC15W4K48S4.h>
#include<stdio.h>

bit spiTransmissionComplete();
void spiSetIsOccupied(bit occupied);
bit spiGetIsOccupied();
void spiInitialize(bit cpol,bit cpha,unsigned char clkDiv);
void spiSetup(bit cpol,bit cpha,unsigned char clkDiv);
unsigned char spiSend(unsigned char c);
unsigned char spiRecv();
unsigned char *spiSeqRecv(unsigned char *destination,unsigned int length);
bit spiTransmissionComplete();

#endif
