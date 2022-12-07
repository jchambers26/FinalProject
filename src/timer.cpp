#include "timer.h"

/* Initialize timer 0, you should not turn the timer on here.
* You will need to use CTC mode */
void initTimer0(){

    // WGM bits should be 010 for CTC mode
    TCCR0A &= ~( 1 << WGM00);
    TCCR0A |=  ( 1 << WGM01);
    TCCR0B &= ~( 1 << WGM02);   

}


/* This delays the program an amount specified by unsigned int delay.
* Use timer 0. Keep in mind that you need to choose your prescalar wisely
* such that your timer is precise to 1 millisecond and can be used for
* 100-200 milliseconds
*/
void delayMs(unsigned int delay){

    int count = delay;

    // set up prescaler and output compare register value with formula
    // OCR0A = (Td * Clk Freq )/ (Prescaler)
    // 0CR0A = 16000 / Prescaler
    // Td = 1 s, clk freq = 16Mhz, Prescaler = 64
    // OCR0A = 249;
    //set the output compare register OCR0A to 249;

    OCR0A = 249;


    // Turn on clock with prescaler of 64
    TCCR0B  |= (1 << CS00) | (1 << CS01);
    TCCR0B  &= ~ (1 << CS02);

    // Each loop is a 1ms delay, so loop up to count to get a (count) ms delay
    for (int i = 0; i < count; i++){
        // clear 0CF0A flag
        TIFR0 |= (1 << OCF0A);

        // Clear tcnt to restart counting at 0
        TCNT0 = 0;
        
        // Do nothing until the flag goes up (TCNT0 == 249)
        while ( ! (TIFR0 & ( 1 << OCF0A)));
    }

    // turn clock off
    TCCR0B  &= ~ ((1 << CS00) | (1 << CS01) | ( 1 << CS02));

}

// Initialize Timer 1 for CTC mode //
void initTimer1(){
	//specify WGM bits for CTC mode
    //for CTC mode, WGM10 = 0, WGM11 = 0, WGM12 = 1, WGM13 = 0
    TCCR1A &= ~(1 << WGM10);
    TCCR1A &= ~(1 << WGM11);
    TCCR1B|= (1 << WGM12);
    TCCR1B &= ~(1 << WGM13);
}

/* This delays the program an amount of microseconds specified by unsigned int delay.
*/
void delayUs(unsigned int delay) {
    unsigned int count = 0;

//calculate value goes to OCR0A per ms delay (OCR0A = 2) and a 8 prescalar
//OCR1A = 2;
OCR1AH = 0;
OCR1AL = 2;

//set prescalar to 8 to start timer
TCCR1B |= (1 << CS11);
TCCR1B &= ~((1 << CS12) | (1 << CS10));

while(count < delay){
    //set flag down (logic = 1)
    TIFR1 |= (1 << OCF1A);

    //clear timer
    TCNT1 = 0;

    //loop until flag is changed (logic = 0)
    while(!(TIFR1 & (1 << OCF1A)));
    //exit loop when flag changes

    count++;
    // turn timer off
    }
}