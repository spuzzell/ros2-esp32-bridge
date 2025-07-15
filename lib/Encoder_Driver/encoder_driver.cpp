#include "config.h"
#include <Arduino.h>
#include "encoder_driver.h"

#ifndef TEST_DESKTOP
#include <esp_attr.h>
#else
#include "fake_esp_attr.h"
#endif


volatile long encoderPulseLeft  = 0L;
volatile long encoderPulseRight = 0L;
static const int ENC_STATES [] = {0,-1,1,2,1,0,2,-1,-1,2,0,1,2,1,-1,0};

volatile int encPos_Left      = 0;
volatile int lastEncPos_Left  = 0;
volatile int encPos_Right     = 0;
volatile int lastEncPos_Right = 0;


void IRAM_ATTR ISR_encoder_left(){
  lastEncPos_Left=encPos_Left;
  int MSB = (GPIO.in >> LEFT_ENC_A) & 0x1;
  int LSB = (GPIO.in >> LEFT_ENC_B) & 0x1;
  encPos_Left = (MSB << 1) | LSB;
  if(ENC_STATES[lastEncPos_Left * 4 + encPos_Left]!=2) encoderPulseLeft+=ENC_STATES[lastEncPos_Left * 4 + encPos_Left];
}

void IRAM_ATTR ISR_encoder_right(){
  lastEncPos_Right=encPos_Right;
  int MSB = (GPIO.in >> RIGHT_ENC_A) & 0x1;
  int LSB = (GPIO.in >> RIGHT_ENC_B) & 0x1;
  encPos_Right = (MSB << 1) | LSB;
  if(ENC_STATES[lastEncPos_Right * 4 + encPos_Right]!=2) encoderPulseRight-=ENC_STATES[lastEncPos_Right * 4 + encPos_Right];
}












/**
 * @brief Initializes the encoder pins and attaches interrupts for both encoders.
 */
void roverEncoders::init2() {
  pinMode(LEFT_ENC_A,INPUT_PULLUP); 
  pinMode(LEFT_ENC_B,INPUT_PULLUP);
  pinMode(RIGHT_ENC_A,INPUT_PULLUP);
  pinMode(RIGHT_ENC_B,INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(LEFT_ENC_A), ISR_encoder_left, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LEFT_ENC_B), ISR_encoder_left, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC_A), ISR_encoder_right, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC_B), ISR_encoder_right, CHANGE);
}

/**
 * @brief Returns the encoder count for the specified encoder and resets it.
 * @param i Index of the encoder (LEFT or RIGHT)
 * @return The encoder count before reset
 */
long roverEncoders::getCountsAndReset(int i) {
  long enc_pos = readEncoder(i);
  resetEncoder(i);
  return enc_pos;
}

/**
 * @brief Reads the current count of the specified encoder.
 * @param i Index of the encoder (LEFT or RIGHT)
 * @return The current encoder count
 */
long roverEncoders::readEncoder(int i) {
  long value;
  noInterrupts();
  value = (i == LEFT) ? encoderPulseLeft : encoderPulseRight;
  interrupts();
  return value;
}



/**
 * @brief Resets the count of the specified encoder to zero.
 * @param i Index of the encoder (LEFT or RIGHT)
 */
void roverEncoders::resetEncoder(int i) {
  if (i == LEFT) encoderPulseLeft = 0L;
  else encoderPulseRight = 0L;
}

/**
 * @brief Resets both encoder counts to zero.
 */
void roverEncoders::resetEncoders() {
  resetEncoder(LEFT);
  resetEncoder(RIGHT);
}

void roverEncoders::transmit(){
        Serial.print(readEncoder(LEFT));
        Serial.print(" ");
        Serial.println(readEncoder(RIGHT));
    }














  