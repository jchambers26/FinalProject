#ifndef SPI_H
#define SPI_H

void initSPI();
void writeSPI(unsigned char command, unsigned char data);
void clearSPI();
void writeNumber(int num);

#endif