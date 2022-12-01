/*
Jacob Chambers
Provide implementation for functions to run SPI protocol and drive 8x8 led matrix
*/
#include <Arduino.h>
#include "SPI.h"
#include <avr/interrupt.h>
#include <avr/io.h>

void initSPI() {

    /*
    LED MATRIX PIN INFORMATION:
    LED pin VCC -> Arduino pin 5V
    LED pin GND -> Arduino pin GND
    LED pin DIN -> Arduino pin 51
    LED pin CS -> Arduino pin 53
    LED pin CLK -> Arduino pin 52
    */

    // Set MOSI SS and SCK as output
    DDRB |= (1 << DDB2) | (1 << DDB1) | (1 << DDB0);

    // Turn chip select off by setting SS high
    PORTB |= (1 << PORTB0);

    // enable SPI, master mode, CPOL, CPHA, default clock and fosc/128
    SPCR |= (1 << SPE) | (1 << MSTR) | (1 << CPOL) | (1 << CPHA) | (1 << SPR1) | (1 << SPR0);

}

void writeSPI(unsigned char command, unsigned char data) {

    // Turn chip select on by setting SS low
    PORTB &= ~(1 << PORTB0);

    // Send command
    SPDR = command;
    while (!(SPSR & (1 << SPIF)));

    // Send data
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));

    // Turn chip select off by setting SS high
    PORTB |= (1 << PORTB0);

}

void clearSPI() {

    // Clear all rows
    for (int i = 1; i <= 9; i++) {
        writeSPI(i, 0b00000000);
    }

}

void writeX() {

    Serial.println("Writing X");

    clearSPI();
    // Write X to the matrix
    writeSPI(1, 0b10000001);
    writeSPI(2, 0b01000010);
    writeSPI(3, 0b00100100);
    writeSPI(4, 0b00011000);
    writeSPI(5, 0b00011000);
    writeSPI(6, 0b00100100);
    writeSPI(7, 0b01000010);
    writeSPI(8, 0b10000001);
}

void writeCheck() {

    Serial.println("Writing check");
    
    clearSPI();
    // Write check to the matrix
    writeSPI(1, 0b00000000);
    writeSPI(2, 0b00000000);
    writeSPI(3, 0b00000001);
    writeSPI(4, 0b00000010);
    writeSPI(5, 0b00000100);
    writeSPI(6, 0b10001000);
    writeSPI(7, 0b01010000);
    writeSPI(8, 0b00100000);
}

void writeNumber(int num){ // we may not need this 
    clearSPI();
    switch(num){
        case 0:
            writeSPI(1, 0x3C);
            writeSPI(2, 0x42);
            writeSPI(3, 0x42);
            writeSPI(4, 0x42);
            writeSPI(5, 0x42);
            writeSPI(6, 0x42);
            writeSPI(7, 0x42);
            writeSPI(8, 0x3C);
            break;
        case 1:
            writeSPI(1, 0x10);
            writeSPI(2, 0x30);
            writeSPI(3, 0x50);
            writeSPI(4, 0x10);
            writeSPI(5, 0x10);
            writeSPI(6, 0x10);
            writeSPI(7, 0x10);
            writeSPI(8, 0x7C);
            break;
        case 2:
            writeSPI(1, 0x3C);
            writeSPI(2, 0x42);
            writeSPI(3, 0x02);
            writeSPI(4, 0x04);
            writeSPI(5, 0x08);
            writeSPI(6, 0x10);
            writeSPI(7, 0x20);
            writeSPI(8, 0x7E);
            break;
        case 3:
            writeSPI(1, 0x3C);
            writeSPI(2, 0x42);
            writeSPI(3, 0x02);
            writeSPI(4, 0x1C);
            writeSPI(5, 0x02);
            writeSPI(6, 0x02);
            writeSPI(7, 0x42);
            writeSPI(8, 0x3C);
            break;
        case 4:
            writeSPI(1, 0x04);
            writeSPI(2, 0x0C);
            writeSPI(3, 0x14);
            writeSPI(4, 0x24);
            writeSPI(5, 0x44);
            writeSPI(6, 0x7E);
            writeSPI(7, 0x04);
            writeSPI(8, 0x04);
            break;
        case 5:
            writeSPI(1, 0x7E);
            writeSPI(2, 0x40);
            writeSPI(3, 0x40);
            writeSPI(4, 0x7C);
            writeSPI(5, 0x02);
            writeSPI(6, 0x02);
            writeSPI(7, 0x42);
            writeSPI(8, 0x3C);
            break;
        case 6:
            writeSPI(1, 0x3C);
            writeSPI(2, 0x42);
            writeSPI(3, 0x40);
            writeSPI(4, 0x7C);
            writeSPI(5, 0x42);
            writeSPI(6, 0x42);
            writeSPI(7, 0x42);
            writeSPI(8, 0x3C);
            break;
        case 7:
            writeSPI(1, 0x7E);
            writeSPI(2, 0x02);
            writeSPI(3, 0x04);
            writeSPI(4, 0x08);
            writeSPI(5, 0x10);
            writeSPI(6, 0x10);
            writeSPI(7, 0x10);
            writeSPI(8, 0x10);
            break;
        case 8:
            writeSPI(1, 0x3C);
            writeSPI(2, 0x42);
            writeSPI(3, 0x42);
            writeSPI(4, 0x3C);
            writeSPI(5, 0x42);
            writeSPI(6, 0x42);
            writeSPI(7, 0x42);
            writeSPI(8, 0x3C);
            break;
        case 9:
            writeSPI(1, 0x3C);
            writeSPI(2, 0x42);
            writeSPI(3, 0x42);
            writeSPI(4, 0x42);
            writeSPI(5, 0x3E);
            writeSPI(6, 0x02);
            writeSPI(7, 0x42);
            writeSPI(8, 0x3C);
            break;
        default:
            break;
    }
}
