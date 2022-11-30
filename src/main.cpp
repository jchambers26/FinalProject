#include <Arduino.h>
#include "keypad.h"
#include "timer.h"
#include "spi.h"


const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
volatile unsigned char col = 0;

typedef enum stateType_enum {
  wait_press, debounce_press, wait_release, debounce_release
} stateType;

volatile stateType state1 = wait_press;
volatile stateType state2 = wait_press;
volatile stateType state3 = wait_press;
volatile stateType state4 = wait_press;

stateType states[4] = {state1, state2, state3, state4};



int main() {
	sei();
	
	Serial.begin(9600);
	initKeypad();
	initTimer0();
	initSPI();
	clearSPI();

	while(1) {

		setColumnLow(col);

		int row = checkRows();
		if(row != -1) {

			if ((row < 3 && col < 3) || (row == 3 && col == 1)) {
				writeNumber(keys[row][col] - '0');	
			}
			Serial.println(keys[row][col]);

			while (checkRows() == row);
		}



		for (int i = 0; i < 4; i++) {
			switch(states[i]) {
				case wait_press:
					break;
				case debounce_press:
					delayMs(1);
					states[i] = wait_release;
					break;
				case wait_release:
					break;
				case debounce_release:
					delayMs(1);
					states[i] = wait_press;
					break;
			}
		}

		col++;
		if(col > 3) {
			col = 0;
		}
	}

	return 1;

}

// ISR(INT0_vect) {
// 	if (state1 == wait_press) {
// 		Serial.println(keys[0][col]);
// 		state1 = debounce_press;
// 	}
// 	else if (state1 == wait_release) {
// 		state1 = debounce_release;
// 	}
// }

// ISR(INT1_vect) {
// 	if (state2 == wait_press) {
// 		Serial.println(keys[1][col]);
// 		state2 = debounce_press;
// 	}
// 	else if (state2 == wait_release) {
// 		state2 = debounce_release;
// 	}
// }

// ISR(INT2_vect) {
// 	if (state3 == wait_press) {
// 		Serial.println(keys[2][col]);
// 		state3 = debounce_press;
// 	}
// 	else if (state3 == wait_release) {
// 		state3 = debounce_release;
// 	}
// }

// ISR(INT3_vect) {
// 	if (state4 == wait_press) {
// 		Serial.println(keys[3][col]);
// 		state4 = debounce_press;
// 	}
// 	else if (state4 == wait_release) {
// 		state4 = debounce_release;
// 	}
// }