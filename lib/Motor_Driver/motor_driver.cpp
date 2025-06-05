
// Use fake pin control and replaces a few functions 
// for native (desktop) unit testing as platformIO
// wants to build all libs including stuff thats not included anywhere
#if defined(TEST_DESKTOP)
    #include "fakeduino.h"   
#else
    #include <Arduino.h>
#endif

#include "config.h"
#include "motor_driver.h"

void roverMotors::init2() {
  // Set right motor pins as outputs
  pinMode(RIGHT_MOTOR_ENABLE_PIN, OUTPUT);      
  pinMode(RIGHT_MOTOR_FORWARD_IN3_PIN, OUTPUT); 
  pinMode(RIGHT_MOTOR_REVERSE_IN4_PIN, OUTPUT); 

  // Set left motor pins as outputs
  pinMode(LEFT_MOTOR_ENABLE_PIN, OUTPUT);       
  pinMode(LEFT_MOTOR_FORWARD_IN1_PIN, OUTPUT);  
  pinMode(LEFT_MOTOR_REVERSE_IN2_PIN, OUTPUT);  

  // Setup and attach PWM channels for both motors
  ledcSetup(RIGHT_PWM_CHANNEL, PWM_FREQ, RESOLUTION);
  ledcAttachPin(RIGHT_MOTOR_ENABLE_PIN, RIGHT_PWM_CHANNEL);
  ledcSetup(LEFT_PWM_CHANNEL, PWM_FREQ, RESOLUTION);
  ledcAttachPin(LEFT_MOTOR_ENABLE_PIN, LEFT_PWM_CHANNEL);

  // Ensure motors are stopped on initialisation for safety
  digitalWrite(RIGHT_MOTOR_FORWARD_IN3_PIN, LOW);
  digitalWrite(RIGHT_MOTOR_REVERSE_IN4_PIN, LOW);
  digitalWrite(LEFT_MOTOR_FORWARD_IN1_PIN, LOW);
  digitalWrite(LEFT_MOTOR_REVERSE_IN2_PIN, LOW);
}

/**
 * @brief Drives a single motor using PWM and direction pins. 
 * Currently set up for L298N Motor driver
 * 
 * @param pwmChannel   PWM channel to use for speed control.
 * @param forwardPin   Pin to set HIGH for forward direction.
 * @param reversePin   Pin to set HIGH for reverse direction.
 * @param speed        Motor speed (-MAX_PWM to MAX_PWM). Negative for reverse.
 */
void roverMotors::driveMotor(int pwmChannel, int forwardPin, int reversePin, int speed) {
  bool reverse = speed < 0; // Determine direction
  speed = constrain(abs(speed), 0, MAX_PWM); // Clamp speed to valid range

  ledcWrite(pwmChannel, speed); // Set PWM duty cycle
  digitalWrite(forwardPin, reverse ? LOW : HIGH); // Set direction pins (only one can be high)
  digitalWrite(reversePin, reverse ? HIGH : LOW);
}

/**
 * @brief Sets the speed of a single motor (left or right).
 * 
 * @param motor  LEFT or RIGHT (see config.h)
 * @param speed  Speed value (-MAX_PWM to MAX_PWM)
 */
void roverMotors::setMotorSpeed(int motor, int speed) {
  if (motor == LEFT) {
    driveMotor(LEFT_PWM_CHANNEL, LEFT_MOTOR_FORWARD_IN1_PIN, LEFT_MOTOR_REVERSE_IN2_PIN, speed);
  } else if (motor == RIGHT) {
    driveMotor(RIGHT_PWM_CHANNEL, RIGHT_MOTOR_FORWARD_IN3_PIN, RIGHT_MOTOR_REVERSE_IN4_PIN, speed);
  }
}

/**
 * @brief Sets the speed of both motors at once.
 * 
 * @param leftSpeed   Speed for left motor (-MAX_PWM to MAX_PWM)
 * @param rightSpeed  Speed for right motor (-MAX_PWM to MAX_PWM)
 */
void roverMotors::setMotorSpeeds(int leftSpeed, int rightSpeed) {
  setMotorSpeed(LEFT, leftSpeed);
  setMotorSpeed(RIGHT, rightSpeed);
}