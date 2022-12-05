// This file implements the initialization of the PIR motion detector.
// ----------------------------------------------------------------- //

#include "pir.h"
#include <avr/io.h>

//initialize the PIR motion detector as an input from port H6
void initPIR() {
    DDRH &= ~(1 << PORTH6);
}

bool checkPIR() {
    if (PINH & (1 << PINH6)) {
        return true;
    }
    return false;
}

