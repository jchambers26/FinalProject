// Jacob Chambers
// Provide implementation for the keypad, including initialization, setting an entire column to low, 
// and reading a row in order to determine which key was pressed.
//----------------------------------------------------------------------//
#ifndef KEYPAD_H
#define KEYPAD_H
#include "keypad.h"
#include <avr/io.h>
#include "Arduino.h"
#include "timer.h"

/*
Initialize the keypad by setting the appropriate pins to output and input.
 */
void initKeypad(){

    // Rows are set to inputs
    DDRD &= ~((1<<DDD0) | (1 << DDD1) | (1 << DDD2) | (1 << DDD3)); // set direction for input
    PORTD |= (1 << PORTD0) | (1 << PORTD1) | (1 << PORTD2) | (1 << PORTD3);  // enable the pullup resistor for stable input

    // Columns are set to outputs
    // Set pins 14, 15, 16, and 17 to output and set them to high
    DDRH |= (1 << DDH0) | (1 << DDH1);
    PORTH |= (1 << PORTH0) | (1 << PORTH1);
    DDRJ |= (1 << DDJ0) | (1 << DDJ1);
    PORTJ |= (1 << PORTJ0) | (1 << PORTJ1);
  

    // // Enable external interrupts for any logical change
    // EICRA &=  ~( 1 << ISC01);
    // EICRA |=  ( 1 << ISC00);

    // // enable the external interrupt mask register so that INT0 bit is enabled for interrupts
    // EIMSK |= ( 1 << INT0);
    // EIMSK |= ( 1 << INT1);
    // EIMSK |= ( 1 << INT2);
    // EIMSK |= ( 1 << INT3);
}

/*
Set a column to low in order to allow for polling of all rows to determine which key was pressed
*/
void setColumnLow(unsigned char col) {

    // Depending on the column, set the appropriate pins to low
    switch(col) {
        case 0:
            PORTH &= ~(1 << PORTH0);
            PORTH |= (1 << PORTH1);
            PORTJ |= (1 << PORTJ0) | (1 << PORTJ1);
            break;
        case 1:
            PORTH &= ~(1 << PORTH1);
            PORTH |= (1 << PORTH0);
            PORTJ |= (1 << PORTJ0) | (1 << PORTJ1);
            break; 
        case 2:
            PORTJ &= ~(1 << PORTJ0);
            PORTH |= (1 << PORTH0) | (1 << PORTH1);
            PORTJ |= (1 << PORTJ1);
            break;
        case 3:
            PORTJ &= ~(1 << PORTJ1);
            PORTH |= (1 << PORTH0) | (1 << PORTH1);
            PORTJ |= (1 << PORTJ0);
            break;
    }
}

/*
Check all rows to determine the row in which the key was pressed. Pressing a key pulls the row low.
*/
int checkRows() {

    // Check each row to see if it is low
    if (!(PIND & (1 << PIND0))) {
        return 0;
    }
    else if (!(PIND & (1 << PIND1))) {
        return 1;
    }
    else if (!(PIND & (1 << PIND2))) {
        return 2;
    }
    else if (!(PIND & (1 << PIND3))) {
        return 3;
    }
    else {
        return -1;
    }

}

#endif