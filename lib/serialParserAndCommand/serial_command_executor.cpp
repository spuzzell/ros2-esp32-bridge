#ifndef TEST_DESKTOP
#include <Arduino.h>
#endif

#include "config.h"
#include "serial_command_executor.h"
#include "serial_command.h"

#include "pin_control.h"
#include "encoder_driver.h"
#include "motor_driver.h"
#include "diff_pid_controller.h"

// External encoder and motor objects (defined elsewhere, typically in main.cpp)
extern roverEncoders encoders;
extern roverMotors motors;
extern diffController controller;

void serialCommandExecutor::execute(const SerialCommand cmd){

    switch(cmd.cmd) {
        case GET_BAUDRATE:
            Serial.println(BAUDRATE);
            break;

        case ECHO:
            Serial.println("********************");
            Serial.println("OK: ECHO");
            Serial.println(cmd.cmd);
            Serial.println(cmd.arg1);
            Serial.println(cmd.arg2);
            Serial.println("********************");
            break;


// PIN CONTROL CLASS (pin_control.h)            
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


// ENCODER CLASS 
        case READ_ENCODERS:
            encoders.transmit();
            break;
        
        case RESET_ENCODERS:
            encoders.resetEncoders();
            controller.reset();
            break;

// MOTOR CONTROL
        case MOTOR_SPEEDS:
            // Sets speed via controller (implementation needed)
            lastMotorCommand=millis();
            if (cmd.arg1 == 0 && cmd.arg2 == 0) {
                motors.setMotorSpeeds(0, 0);
                controller.reset();
            }
            else controller.setMoving(true);
                controller.update(cmd.arg1,cmd.arg2);
                Serial.println("OK"); 
            break;

        case MOTOR_RAW_PWM:
            lastMotorCommand=millis();
            controller.reset();
            motors.setMotorSpeeds(cmd.arg1,cmd.arg2);
            break;

        case UPDATE_PID:
            controller.update();
            break;

// OTHER
        default:
            Serial.println("ERROR: Invalid Command");
            break;
    }
}