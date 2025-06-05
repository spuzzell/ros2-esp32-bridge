#include "config.h"
#include <Arduino.h>
#include "encoder_driver.h"

#ifndef TEST_DESKTOP
#include <esp_attr.h>
#else
#include "fake_esp_attr.h"
#endif

// Encoder position and state variables
volatile long left_enc_pos = 0L;
volatile long right_enc_pos = 0L;
volatile long left_enc_state = 0;
volatile long right_enc_state = 0;
static const int8_t ENC_STATES [] = {0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0};

/**
 * @brief Interrupt Service Routine for the left encoder.
 *        Updates the left encoder's state and position based on pin changes.
 */
void IRAM_ATTR leftEncoderISR() { 
  left_enc_state <<= 2;
  left_enc_state |= ((digitalRead(LEFT_ENC_A) << 1) | digitalRead(LEFT_ENC_B));
  left_enc_pos += ENC_STATES[left_enc_state & 0x0F];
}

/**
 * @brief Interrupt Service Routine for the right encoder.
 *        Updates the right encoder's state and position based on pin changes.
 */
void IRAM_ATTR rightEncoderISR() {
  right_enc_state <<= 2;
  right_enc_state |= ((digitalRead(RIGHT_ENC_A) << 1) | digitalRead(RIGHT_ENC_B));
  right_enc_pos += ENC_STATES[right_enc_state & 0x0F];
}

/**
 * @brief Initializes the encoder pins and attaches interrupts for both encoders.
 */
void roverEncoders::init2() {
  pinMode(LEFT_ENC_A,INPUT_PULLUP); 
  pinMode(LEFT_ENC_B,INPUT_PULLUP);
  pinMode(RIGHT_ENC_A,INPUT_PULLUP);
  pinMode(RIGHT_ENC_B,INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(LEFT_ENC_A), leftEncoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LEFT_ENC_B), leftEncoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC_A), rightEncoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC_B), rightEncoderISR, CHANGE);
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
  value = (i == LEFT) ? left_enc_pos : right_enc_pos;
  interrupts();
  return value;
}

/**
 * @brief Resets the count of the specified encoder to zero.
 * @param i Index of the encoder (LEFT or RIGHT)
 */
void roverEncoders::resetEncoder(int i) {
  if (i == LEFT) left_enc_pos = 0L;
  else right_enc_pos = 0L;
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