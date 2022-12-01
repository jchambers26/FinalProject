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

typedef enum passwordState {
	waitInitial,
	checkSecond,
	checkThird,
	checkFourth,
	setFirst,
	setSecond,
	setThird,
	setFourth
};



int main() {
	sei();
	
	Serial.begin(9600);
	initKeypad();
	initTimer0();
	initSPI();
	clearSPI();
	
	passwordState state = waitInitial;
	unsigned char col = 0;

	char password[4];
	password[0] = '0';
	password[1] = '0';
	password[2] = '0';
	password[3] = '0';
	char checkPassword[4];

	bool doCheck = false;


	while(1) {

		if (doCheck) {

			bool correct = true;
			for (int i = 0; i < 4; i++) {
				if (checkPassword[i] != password[i]) {
					correct = false;
				}
			}
			if (correct) {
				Serial.println("Correct");
			}
			else {
				Serial.println("Incorrect");
				Serial.println("Expected: [" + String(password[0]) + String(password[1]) + String(password[2]) + String(password[3]) + "]");
				Serial.println("Got: [" + String(checkPassword[0]) + String(checkPassword[1]) + String(checkPassword[2]) + String(checkPassword[3]) + "]");
			}
			doCheck = false;
		}

		setColumnLow(col);

		int row = checkRows();
		if(row != -1) {

			char pressed = keys[row][col];
			Serial.println(pressed);

			while (checkRows() == row);

			switch(state) {

				case(waitInitial):
					if(pressed == '#') {
						state = setFirst;
					}
					else {
						checkPassword[0] = pressed;
						state = checkSecond;
					}
					break;

				case(checkSecond):
					checkPassword[1] = pressed;
					state = checkThird;
					break;
				
				case(checkThird):
					checkPassword[2] = pressed;
					state = checkFourth;
					break;
				
				case(checkFourth):
					checkPassword[3] = pressed;
					doCheck = true;
					state = waitInitial;
					break;
				
				case(setFirst):
					password[0] = pressed;
					state = setSecond;
					break;

				case(setSecond):
					password[1] = pressed;
					state = setThird;
					break;

				case(setThird):
					password[2] = pressed;
					state = setFourth;
					break;

				case(setFourth):
					password[3] = pressed;
					state = waitInitial;
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