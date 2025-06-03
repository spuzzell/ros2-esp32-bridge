#include "pin_control.h"


bool pinControl::validInputPin(int pin){
    const int inputPins[] = {2,4,5,12,13,14,15,16,17,18,19,21,
                             22,23,25,26,27,32,33,34,35,36,39};
    for (int i = 0; i < sizeof(inputPins) / sizeof(inputPins[0]); i++) {
        if (pin == inputPins[i]) {
            return true;
        }
    }
    return false;
}

bool pinControl::validOutputPin(int pin){
    const int outputPins[] = {2,4,5,12,13,14,15,16,17,18,19,21,
                             22,23,25,26,27,32,33};
        for (int i = 0; i < sizeof(outputPins) / sizeof(outputPins[0]); i++) {
        if (pin == outputPins[i]) {
            return true;
        }
    }
    return false;
}

void pinControl::setPinMode(int pin, int mode) {
    if (mode == 0 && validInputPin(pin)) pinMode(pin, INPUT);
    else if (mode == 1 && validOutputPin(pin)) pinMode(pin, OUTPUT);
}

void pinControl::writeDigital(int pin, int value) {
    if (!validOutputPin(pin)) return;
    digitalWrite(pin, value ? HIGH : LOW);
}

void pinControl::writeAnalog(int pin, int value) {
    if (!validOutputPin(pin)) return;
    analogWrite(pin, value);
}

int pinControl::readDigital(int pin) {
    if (!validInputPin(pin)) return -1;
    return digitalRead(pin);
}

int pinControl::readAnalog(int pin) {
    if (!validInputPin(pin)) return -1;
    return analogRead(pin);
}