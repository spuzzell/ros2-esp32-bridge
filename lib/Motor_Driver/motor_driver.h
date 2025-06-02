#pragma once

/**
 * @class roverMotors
 * @file motor_driver.h
 * @brief Provides static methods for controlling rover motors using PWM and a L298N.
 */
class roverMotors
{
  public:

    /**
     * @brief Initializes the motor driver system (pins and PWM channels).
     *        Safe to call multiple times; only initializes once.
     */
    static void init()
    {
        static bool initialized = 0;
        if (!initialized)
        {
            init2();
            initialized = true;
        }
    }

  
	static void setMotorSpeeds(int leftSpeed, int rightSpeed);
  static void setMotorSpeed(int i, int spd);

  private:

    static void init2();
    static void driveMotor(int pwmPin, int forwardPin, int reversePin, int speed);
};