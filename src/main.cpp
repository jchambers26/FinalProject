#include <Arduino.h>
#include "keypad.h"
#include "timer.h"
#include "spi.h"
#include "lcd.h"
#include "pir.h"
#include "Servo.h"


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
} passwordState;

typedef enum lockState {
	unlocked,
	locked
} lockState;



int main() {
	
	//initialize a counter
	int cnt = 0;
	bool motion = false;

	sei();
	
	Serial.begin(9600);
	initKeypad();
	initTimer0();
	initTimer1();
	initLCD();
	initPIR();
	initSPI();
	clearSPI();
	writeX();
	Init_Servo();
	lock_s();

	//initial write to the LCD
	moveCursor(0, 0); // moves the cursor to 0,0 position
  	writeString("Times Opened:");
  	moveCursor(1, 0);  // moves the cursor to 1,0 position
  	writeString("0");
	
	// Variables for the keypad and password
	passwordState state = waitInitial;
	unsigned char col = 0;
	char password[4];
	password[0] = '0';
	password[1] = '0';
	password[2] = '0';
	password[3] = '0';
	char checkPassword[4];
	bool doCheck = false;

	lockState lock = locked;


	while(1) {


		// If the door is unlocked and motion is detected for the firs time since unlock, increment the count
		// and update the LCD display
		if (checkPIR() && motion == true) {
			Serial.println("DEBUG: Motion detected");
			motion = false;
			cnt++;
			char cntStr[cnt%10+1];
			sprintf(cntStr, "%d", cnt);
			moveCursor(1, 0);
			writeString(cntStr);
		}

		// If we are supposed to check the entered password against the stored one, do so
		if (doCheck) {

			// Check each character of the entered and stored passwords
			bool correct = true;
			for (int i = 0; i < 4; i++) {
				if (checkPassword[i] != password[i]) {
					correct = false;
				}
			}

			// If the password is correct, toggle the lock state and update the LED matrix display
			if (correct) {
				Serial.println("DEBUG: Correct");
				lock = (lock == locked) ? unlocked : locked;
				if (lock == unlocked) {
					unlock_s();
					writeCheck();
					Serial.println("DEBUG: Unlocked");
					motion = true;
					// moveCursor(1, 0);  // moves the cursor to 1,0 position
    				// if (!(PINH & (1 << PINH6))) {

					// 	Serial.println("motion detected");
					// 	motion = false;
					// 	cnt = cnt + 1;

					// 	char cntStr[cnt%10+1];
					// 	sprintf(cntStr, "%d", cnt);
					// 	writeString(cntStr);
					// 	Serial.println(cntStr);
        			// }		
      				
				} else {
					lock_s();
					writeX();
					motion = false;
					Serial.println("DEBUG: Locked");
				}
			}
			// TODO: HANDLE LED MATRIX, LCD???
			else {
				Serial.println("DEBUG: Incorrect");
				Serial.println("DEBUG: Expected: [" + String(password[0]) + String(password[1]) + String(password[2]) + String(password[3]) + "]");
				Serial.println("DEBUG: Got: [" + String(checkPassword[0]) + String(checkPassword[1]) + String(checkPassword[2]) + String(checkPassword[3]) + "]");
			}
			// Check complete
			doCheck = false;
		}

		// Set a column low to check the buttons in that column
		setColumnLow(col);

		// Check the rows to see if a button was pressed
		int row = checkRows();
		// -1 means no button was pressed, otherwise a button was pressed
		if(row != -1) {

			// Gather the entered character
			char pressed = keys[row][col];
			// FOR DEBUGGING PURPOSES
			Serial.println(pressed);

			// Do nothing while the button is held. This is blocking, but intentionally so.
			// We don't want to do anything until the button is released.
			while (checkRows() == row);

			// Process the current state
			switch(state) {

				case(waitInitial):
					// In the initial state, if the user presses the # key and the door is unlocked, they are entering a new password
					if(pressed == '#') {
						if (lock == unlocked) {
							state = setFirst;
						}
						else {
							Serial.println("DEBUG: Door locked. Unlock door to enter new password");
							continue;
						}
					}
					// Otherwise, a password is being entered
					else {
						checkPassword[0] = pressed;
						state = checkSecond;
					}
					break;

				// In each of the check states, gather the entered character and move to the next state.
				// The checkPassword array is being built as the user enters the password
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
					// Once all four characters have been entered, check the password on the next loop iteration
					doCheck = true;
					state = waitInitial;
					break;
				
				// In each of the set states, gather the entered character and move to the next state.
				// The password array is being modified as the user enters the new password
				case(setFirst):
					if (pressed == '#') {
						Serial.println("DEBUG: # can't be the first character of a password");
						state = setFirst;
						continue;
					}
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

		// Check a new column for each iteration of the loop
		col++;
		if(col > 3) {
			col = 0;
		}
	}

	return 1;

}