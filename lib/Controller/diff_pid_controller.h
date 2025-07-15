#pragma once
#include "config.h"
#include <Arduino.h>

class PIDController {
public:
    PIDController(int motorID);
    void reset();
    void setTargetTicksPerFrame(double target);
    void setMoving(bool state);
    void update();
private:
    int motorID;
    double targetTicksPerFrame;
    bool moving;
    long encVal;
    long prevEnc;
    int prevInput;
    int iTerm;
    long output;
    float Kp, Kd, Ki, Ko;
    void runPID();
};


/**
 * @class roverMotors
 * @file motor_driver.h
 * @brief Provides static methods for controlling rover motors using PWM and a L298N.
 */
class diffController
{
  public:
    
    PIDController leftPID;
    PIDController rightPID;
    bool moving;
    unsigned long nextPID;
    diffController() : leftPID(LEFT), rightPID(RIGHT), moving(false), nextPID(millis()+PID_INTERVAL) {}
    void reset();
    void update(int leftTarget, int rightTarget);
    void update();
    void setMoving(bool state);
    unsigned long getNextPID();
    void writeNextPID(unsigned long value);

};







