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
     *        Calls the private init2() function if not already initialized.
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

  /**
 * @brief Sets the speed of both motors at once.
 * 
 * @param leftSpeed   Speed for left motor (-MAX_PWM to MAX_PWM)
 * @param rightSpeed  Speed for right motor (-MAX_PWM to MAX_PWM)
 */
	static void setMotorSpeeds(int leftSpeed, int rightSpeed);

/**
 * @brief Sets the speed of a single motor (left or right).
 * 
 * @param motor  LEFT or RIGHT (see config.h)
 * @param speed  Speed value (-MAX_PWM to MAX_PWM)
 */
  static void setMotorSpeed(int motor, int speed);

  private:
  
/**
 * @brief Performs the actual hardware initialization (pinMode, PWM setup).
 *        Called only once by init().
 */
    static void init2();

/**
 * @brief Drives a single motor using PWM and direction pins. 
 * Currently set up for L298N Motor driver
 * 
 * @param pwmChannel   PWM channel to use for speed control.
 * @param forwardPin   Pin to set HIGH for forward direction.
 * @param reversePin   Pin to set HIGH for reverse direction.
 * @param speed        Motor speed (-MAX_PWM to MAX_PWM). Negative for reverse.
 */
    static void driveMotor(int pwmChannel, int forwardPin, int reversePin, int speed);
};