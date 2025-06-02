// Use fake pin control for native (desktop) testing, otherwise use Arduino
#if defined(TEST_DESKTOP)
    #include "fakeduino.h"   
#else
    #include <Arduino.h>
#endif

#include "config.h"
#include"motor_driver.h"


void roverMotors::init2() {
  // Set right motor pins
  pinMode(RIGHT_MOTOR_ENABLE_PIN, OUTPUT);      
  pinMode(RIGHT_MOTOR_FORWARD_IN1_PIN, OUTPUT); 
  pinMode(RIGHT_MOTOR_REVERSE_IN2_PIN, OUTPUT); 

  // Set left motor pins
  pinMode(LEFT_MOTOR_ENABLE_PIN, OUTPUT);       
  pinMode(LEFT_MOTOR_FORWARD_IN3_PIN, OUTPUT);  
  pinMode(LEFT_MOTOR_REVERSE_IN4_PIN, OUTPUT);  

  // Setup and attach PWM channels
  ledcSetup(RIGHT_PWM_CHANNEL, PWM_FREQ, RESOLUTION);
  ledcAttachPin(RIGHT_MOTOR_ENABLE_PIN, RIGHT_PWM_CHANNEL);

  ledcSetup(LEFT_PWM_CHANNEL, PWM_FREQ, RESOLUTION);
  ledcAttachPin(LEFT_MOTOR_ENABLE_PIN, LEFT_PWM_CHANNEL);


  // Ensure motors are stopped on initialisation 
  digitalWrite(RIGHT_MOTOR_FORWARD_IN1_PIN, LOW);
  digitalWrite(RIGHT_MOTOR_REVERSE_IN2_PIN, LOW);
  digitalWrite(LEFT_MOTOR_FORWARD_IN3_PIN, LOW);
  digitalWrite(LEFT_MOTOR_REVERSE_IN4_PIN, LOW);
}

void roverMotors::driveMotor(int pwmPin, int forwardPin, int reversePin, int speed) {
  bool reverse = speed < 0;
  speed = constrain(abs(speed), 0, MAX_PWM);

  ledcWrite(pwmPin, speed);
  digitalWrite(forwardPin, reverse ? LOW : HIGH);
  digitalWrite(reversePin, reverse ? HIGH : LOW);
}

void roverMotors::setMotorSpeed(int motor, int speed) {
  if (motor == LEFT) {
    driveMotor(LEFT_PWM_CHANNEL, LEFT_MOTOR_FORWARD_IN3_PIN, LEFT_MOTOR_REVERSE_IN4_PIN, speed);
  } else if (motor == RIGHT) {
    driveMotor(RIGHT_PWM_CHANNEL, RIGHT_MOTOR_FORWARD_IN1_PIN, RIGHT_MOTOR_REVERSE_IN2_PIN, speed);
  }
}

void roverMotors::setMotorSpeeds(int leftSpeed, int rightSpeed) {
  setMotorSpeed(LEFT, leftSpeed);
  setMotorSpeed(RIGHT, rightSpeed);
}