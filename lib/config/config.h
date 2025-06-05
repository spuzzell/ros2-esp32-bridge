#pragma once
// ESP32 Rover Configuration Header File
// This file contains configuration settings for the ESP32 Rover project.
// It includes definitions for pin assignments, motor control, encoder settings, and serial communication commands.

// -------------------- Pin Assignments --------------------
// The pin numbers are based on the ESP32 DevKitC board.
// Adjust these if using a different ESP32 board.  
#define LEFT_ENC_A 23                        // Left encoder channel A       
#define LEFT_ENC_B 22                        // Left encoder channel B             
#define RIGHT_ENC_A 21                       // Right encoder channel A
#define RIGHT_ENC_B 19                       // Right encoder channel B
#define LEFT_MOTOR_ENABLE_PIN 5              // Left motor enable (must be HIGH during boot)
#define RIGHT_MOTOR_ENABLE_PIN 17            // Right motor enable
#define LEFT_MOTOR_FORWARD_IN1_PIN 16        // Left motor forward control  
#define LEFT_MOTOR_REVERSE_IN2_PIN 4         // Left motor reverse control
#define RIGHT_MOTOR_FORWARD_IN3_PIN 2        // Right motor forward control (must be LOW during boot, also on-board LED)
#define RIGHT_MOTOR_REVERSE_IN4_PIN 15       // Right motor reverse control (must be HIGH during boot, prevents startup log if LOW)
                 
// -------------------- Feature Toggles --------------------
// Comment out to disable specific features for smaller builds or debugging
#define ENABLE_ENCODERS        // Comment out to disable encoder support
#define ENABLE_MOTORS          // Comment out to disable motor support
#define ENABLE_CONTROLLER      // Comment out to disable PID controller support
#define ENABLE_PIN_CONTROL     // Comment out to disable read, write and pin mode change capability
#define ENABLE_ERROR_MESSAGES  // Comment out to desable error messages  

// -------------------- Timing Settings --------------------
#define AUTO_STOP_INTERVAL   2000   // Auto-stop motors and other modules if no command received in AUTO_STOP_INTERVAL milliseconds
#define PID_INTERVAL          30    // Periodically update PID controller every PID_INTERVAL milliseconds

// -------------------- Serial Communication Settings --------------------
#define BAUDRATE           115200
#define MAX_ARG_LENGTH         16

// -------------------- Motor and Encoder Constants --------------------
#define LEFT                    0  // Index for left motor/encoder
#define RIGHT                   1  // Index for right motor/encoder

// -------------------- PWM Settings --------------------
#define PWM_FREQ                    5000 // PWM frequency in Hz
#define RESOLUTION                     8 // PWM resolution in bits
#define MAX_PWM                      255 // Maximum PWM value (for 8-bit resolution)
#define RIGHT_PWM_CHANNEL              0 // PWM channel for right motor
#define LEFT_PWM_CHANNEL               1 // PWM channel for left motor

// -------------------- Serial Command Characters --------------------
// Used for parsing incoming serial commands
#define ANALOG_READ    'a'  // Analog read command
#define GET_BAUDRATE   'b'  // Get current baud rate
#define PIN_MODE       'p'  // Set pin mode
#define DIGITAL_READ   'd'  // Digital read command
#define READ_ENCODERS  'e'  // Read encoder values
#define MOTOR_SPEEDS   'm'  // Set motor speeds
#define MOTOR_RAW_PWM  'o'  // Set raw PWM values
#define RESET_ENCODERS 'r'  // Reset encoder counts
#define UPDATE_PID     'u'  // Update PID parameters
#define DIGITAL_WRITE  'w'  // Digital write command
#define ANALOG_WRITE   'x'  // Analog write command
#define ECHO           'Y'  // Echo command for testing
#define ERROR          '!'  // Error indicator

// -------------------- Safety Serial Command Characters ---------------------
// --------------------------------- WARNING ---------------------------------
// #define EMERGENCY_STOP 'Z' will trigger a stop state that halts all actions and requires 
// a restart to recover or a ALL_CLEAR command.
// When received, the firmware returns to start of loop() before code execution, effectively
// pausing the system.
// EMERGENCY_STOP also sets all modules that could cause harm to a stop state (e.g. motors). 
// If you add new modules that need to be stopped during emergency, include their stop logic 
// in the if(emergencyStop) block in main.cpp.
#define EMERGENCY_STOP 'Z'
#define ALL_CLEAR      'C' // Clears the EMERGENCY_STOP state and resumes normal opperation
