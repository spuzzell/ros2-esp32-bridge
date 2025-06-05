#pragma once
#include "config.h"
#include <Arduino.h>





class pinControl {
public:

    // Set pin mode (0 = INPUT, 1 = OUTPUT)
    static void setPinMode(int pin, int mode);

    // Write digital value (0 = LOW, 1 = HIGH)
    static void writeDigital(int pin, int value);

    // Write analog value (PWM)
    static void writeAnalog(int pin, int value);

    // Read digital value
    static int readDigital(int pin);

    // Read analog value
    static int readAnalog(int pin);
private:
        // Check if a pin is valid for input
    static bool validInputPin(int pin);

    // Check if a pin is valid for output
    static bool validOutputPin(int pin);

};