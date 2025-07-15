/**
 * @file main.cpp
 * @brief Main entry point for the ROSEspBridge firmware.
 *
 * This file initializes and manages the main control loop for the ESP32-based
 * ROS2 serial bridge. It conditionally initializes hardware modules (motors,
 * encoders, PID controller) based on compile-time configuration, and processes
 * incoming serial commands to control the robot. The loop handles command parsing,
 * execution, error reporting, periodic PID updates, and motor safety (auto-stop).
 *
 * Modules used:
 *  - Motor driver (optional)
 *  - Encoder driver (optional)
 *  - Differential PID controller (optional)
 *  - Serial command parser and executor
 *  - Additional modules can be integrated by defining commands in config.h and 
 *    implementing their handling in serial_command_executor.h
 *
 * Usage:
 *  - Configure features in config.h (ENABLE_MOTORS, ENABLE_ENCODERS, ENABLE_CONTROLLER).
 *  - Upload to ESP32 and communicate via serial.
 */

#include <Arduino.h>
#include "config.h"

// Encoder driver and object, enabled via config
#ifdef ENABLE_ENCODERS
#include "encoder_driver.h"
roverEncoders encoders;
#endif

// Motor driver and object, enabled via config
#ifdef ENABLE_MOTORS
#include "motor_driver.h"
roverMotors motors;
#endif

// Differential PID controller, enabled via config
#ifdef ENABLE_CONTROLLER
#include "diff_pid_controller.h"
diffController controller;
#endif

#include "serial_parser.h"
#include "serial_command.h"
#include "serial_command_executor.h"

// Serial parser and command executor objects
serialParser parser;
serialCommandExecutor executor;

volatile bool emergencyStop = false;



void setup() {

#ifdef ENABLE_MOTORS
    motors.init();       // Initialize motors if enabled
#endif

#ifdef ENABLE_ENCODERS
    encoders.init();     // Initialize encoders if enabled
#endif

#ifdef ENABLE_CONTROLLER
    controller.reset();  // Reset controller if enabled
#endif

    parser.init();       // Initialize serial parser
    executor.lastMotorCommand = millis(); // Initialize to current time
}


void loop(){
    // Check if emergency stop has been triggered
    if (emergencyStop) {
        // Immediately stop all motors (add stop commands for other modules here if needed e.g servos)
        #ifdef ENABLE_MOTORS
        motors.setMotorSpeeds(0, 0); 
        #endif
        if(parser.allClear()) emergencyStop = false; 
        delay(100);
        return;
    }

    SerialCommand cmd{};
    // Poll for new serial command
    if (parser.poll(cmd)) {
        if (cmd.valid) {
            Serial.println(cmd.cmd);
            // If emergency stop command received, set flag and exit loop iteration
            if (cmd.cmd == EMERGENCY_STOP) {
                emergencyStop = true; 
                parser.flushCmdBuffer(cmd);
                return;
            }
            executor.execute(cmd); // Execute valid command
        } else {
            #ifdef ENABLE_ERROR_MESSAGES
            serialErrorMSG(cmd);   // Handle invalid command
            #endif
        }
    }

#ifdef ENABLE_CONTROLLER
    // Periodically update PID controller based on PID_INTERVAL set via config
    if (millis() > controller.getNextPID()) {
      controller.update();
        controller.writeNextPID(controller.getNextPID() + PID_INTERVAL);
    }
#endif

#ifdef ENABLE_MOTORS
    // Auto-stop motors if no command received recently based on AUTO_STOP_INTERVAL set via config
    if ((millis() - executor.lastMotorCommand) > AUTO_STOP_INTERVAL)
        motors.setMotorSpeeds(0, 0);
        #ifdef ENABLE_CONTROLLER
        controller.reset();
        #endif
#endif

}


#ifdef TEST_DESKTOP
int main(){return 0;}
#endif


