#include "servo.h"
#include <avr/io.h>
#include "Arduino.h"
#include "timer.h"

void Init_Servo(){

    DDRH |= (1 << DDH5); // set as output
    PORTH &= ~(1 << PH5); // set as low

    TCCR4A |= (1<<WGM40) | (1<<WGM41);
    TCCR4B |= (1<<WGM42) | (1<<WGM43);
    // table 17-4, Fast PWM non-inverting mode
    TCCR4A &= ~(1<<COM4C0);
    TCCR4A |= (1<<COM4C1);

    TCCR4B |= (1<<CS42);
    TCCR4B &= ~((1<<CS41) | (1<<CS40));
    // Pin 8
    DDRH |= (1<<DDH5);

    OCR4A = 1249;


  
}

void lockDoor() {
    OCR4C = 20;
}

void unlockDoor() {
    OCR4C = 150;
}

void lock_s(){

    PORTH |= (1 << PH5);
    delayMs(1800);    //position
    PORTH &= ~(1 << PH5);
    delayMs(18000);   //balance of 20000 cyclelp
}
void unlock_s(){

    PORTH |= (1 << PH5);
    delayMs(800);    //position
    PORTH &= ~(1 << PH5);
    delayMs(18600);   //balance of 20000 cycle
}
