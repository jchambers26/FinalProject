#include "servo.h"
#include <avr/io.h>
#include "Arduino.h"

void Init_Servo(){

  DDRH |= (1 << DDH5); // set as output
  PORTH &= ~(1 << PH6); // set as low
  
}

void lock_s(){

    PORTH |= (1 << PH6);
    delayMs(1800);    //position
    PORTH &= ~(1 << PH6);
    delayMs(18000);   //balance of 20000 cycle
}
void unlock_s(){

    PORTH |= (1 << PH6);
    delayMs(800);    //position
    PORTH &= ~(1 << PH6);
    delayMs(18600);   //balance of 20000 cycle
}
