/*
 * serial_command_executor.cpp
 * 
 * This file implements the serialCommandExecutor class, which executes commands
 * received over the serial interface. Supported commands include pin control,
 * encoder operations, motor control, and controller updates. The available features
 * depend on compile-time configuration flags (e.g., ENABLE_PIN_CONTROL, ENABLE_ENCODERS).
 * 
 * Each command is dispatched and handled accordingly, allowing for modular and
 * configurable robot control via serial communication.
 */



#include <Arduino.h>
#include "config.h"
#include "serial_command_executor.h"
#include "serial_command.h"

#ifdef ENABLE_PIN_CONTROL
#include "pin_control.h"
#endif

#ifdef ENABLE_ENCODERS
#include "encoder_driver.h"
extern roverEncoders encoders;
#endif

#ifdef ENABLE_MOTORS
#include "motor_driver.h"
extern roverMotors motors;
#endif

#ifdef ENABLE_CONTROLLER
#include "diff_pid_controller.h"
extern diffController controller;
#endif

void serialCommandExecutor::execute(const SerialCommand cmd){

    switch(cmd.cmd) {
        case GET_BAUDRATE:
            // Print the current serial baudrate
            Serial.println(BAUDRATE);
            break;

        case ECHO:
            // Echo command for testing serial communication
            Serial.println("********************");
            Serial.println("OK: ECHO");
            Serial.println(cmd.cmd);
            Serial.println(cmd.arg1);
            Serial.println(cmd.arg2);
            Serial.println("********************");
            break;

// PIN CONTROL CLASS (pin_control.h)  
// Enabled via config 
#ifdef ENABLE_PIN_CONTROL        
        case ANALOG_READ:
            pinControl::readAnalog(cmd.arg1);
            break;

        case DIGITAL_READ:
            pinControl::readDigital(cmd.arg1);
            break;

        case ANALOG_WRITE:
            pinControl::writeAnalog(cmd.arg1, cmd.arg2);
            break;

        case DIGITAL_WRITE:
            pinControl::writeDigital(cmd.arg1, cmd.arg2);
            break;

        case PIN_MODE:
            pinControl::setPinMode(cmd.arg1, cmd.arg2);
            break;
#endif


// ENCODER CLASS (encoder_driver.h)
// Enabled via config  
#ifdef ENABLE_ENCODERS
        case READ_ENCODERS:
            // Transmit encoder values over serial
            encoders.transmit();
            break;
        
        case RESET_ENCODERS:
            // Reset encoder counts, and controller if enabled
            encoders.resetEncoders();
            #ifdef ENABLE_CONTROLLER 
            // This is necessary because the controller compares previous and current encoder counts.
            // If not reset, it may cause incorrect logic or velocity calculations.
            controller.reset();
            #endif
            break;
#endif

// MOTOR CONTROL (motor_drive.h & diff_pid_controller.h)
// Enabled via config 
#ifdef ENABLE_CONTROLLER
        case MOTOR_SPEEDS:
            // Set motor speeds using controller, or stop if both args are zero
            lastMotorCommand=millis();
            if (cmd.arg1 == 0 && cmd.arg2 == 0) {
                motors.setMotorSpeeds(0, 0);
                controller.reset();
            }
            else{ 
                controller.setMoving(true);
                controller.update(cmd.arg1,cmd.arg2);
                Serial.println("OK"); 
            }
            break;
        
        case UPDATE_PID:
            // Update the PID controller manually (no arguments). 
            // Typically not needed if PID_INTERVAL and PID coefficients are properly configured.
            controller.update();
            break;
#endif

#ifdef ENABLE_MOTORS
        case MOTOR_RAW_PWM:
            // Set raw PWM values to motors, reset controller if enabled 
            // Method for by passing controller 
            lastMotorCommand=millis();
            #ifdef ENABLE_CONTROLLER 
            controller.reset();
            #endif
            motors.setMotorSpeeds(cmd.arg1,cmd.arg2);
            break;
#endif

// OTHER
        default:
            // Handle unknown or invalid command (should normally be caught in serial_parser.h)
            Serial.println("ERROR: Invalid Command passed");
            break;
    }
}