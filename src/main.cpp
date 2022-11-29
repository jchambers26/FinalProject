#include <Arduino.h>
#include "keypad.h"


const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
volatile unsigned char col = 0;



void main() {
	
	Serial.begin(9600);
	initKeypad();

	while(1) {

		setColumnHigh(col);
		col++;
		if(col > 3) {
			col = 0;
		}
		delay(1000);
	}

}

ISR(INT0_vect) {
	Serial.println(keys[0][col]);
}
ISR(INT1_vect) {
	Serial.println(keys[1][col]);
}
ISR(INT2_vect) {
	Serial.println(keys[2][col]);
}
ISR(INT3_vect) {
	Serial.println(keys[3][col]);
}