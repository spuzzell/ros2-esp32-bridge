#pragma once   
#define HIGH 1
#define LOW  1
#define INPUT 1
#define OUTPUT 1
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

void pinMode(int pin, int mode){return;}
void digitalWrite(int pin, int mode){return;}
int digitalRead(int pin){return 1;}
int analogRead(int pin){return 0;}
void ledcSetup(int channel, int freq, int res){return;}
void ledcAttachPin(int pin, int channel){return;}
void ledcWrite(int pin, int speed){return;}
int abs(int x) {return (x < 0) ? -x : x;}
