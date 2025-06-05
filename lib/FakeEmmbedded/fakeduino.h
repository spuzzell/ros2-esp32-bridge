#pragma once

#include <thread>
#include <chrono>
#include <queue>
#include <string>

#define HIGH 1
#define LOW  0
#define INPUT 0x01
#define OUTPUT 0x03
#define INPUT_PULLUP 0x05
#define CHANGE 0x03
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define IRAM_ATTR

typedef signed char int8_t;

//gpio stuff (nothing here works because doesn't exist)
// to test this functionality, test on [env:esp32dev]
void pinMode(int pin, int mode){return;}
void digitalWrite(int pin, int value){return;}
void analogWrite(int pin, int value){return;}
int digitalRead(int pin){return 1;}
int analogRead(int pin){return 0;}

//pwm stuff (nothing here works because doesn't exist)
// to test this functionality, test on [env:esp32dev]
void ledcSetup(int channel, int freq, int res){return;}
void ledcAttachPin(int pin, int channel){return;}
void ledcWrite(int pin, int speed){return;}

//encoder stuff (nothing here works because doesn't exist)
// to test this functionality, test on [env:esp32dev]
inline int digitalPinToInterrupt(int pin) { return pin; }
inline void attachInterrupt(int pin, void (*isr)(), int mode) {}
inline void noInterrupts() {}
inline void interrupts() {}

//replacement functions
int abs(int x) {return (x < 0) ? -x : x;}

inline int millis() {
    static auto start = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
}

inline void delay(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}


struct {
    void print(long v) {}
    void print(const char* s) {}
    void println(long v) {}
    void println(const char* s) {}
} Serial;