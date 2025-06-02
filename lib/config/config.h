#pragma once


// ESP32 Rover Configuration Header File
// This file contains configuration settings for the ESP32 Rover project.
// It includes definitions for pin assignments, motor control, encoder settings, and serial communication commands.


// Pin assignments for the ESP32 Rover :                               
// The pin numbers are based on the ESP32 DevKitC board.               
// The pin numbers may vary based on the specific ESP32 board used.    


#define LEFT_ENC_A 23                     
#define LEFT_ENC_B 22                     
//#define UNUSED 1                        //(Tx pin, used for flashing and debugging - DONT USE FOR OTHER PURPOSES)
//#define UNUSED 3                        //(Rx pin, used for flashing and debugging - DONT USE FOR OTHER PURPOSES)
#define RIGHT_ENC_A 21            
#define RIGHT_ENC_B 19              
//#define UNUSED 18   
#define LEFT_MOTOR_ENABLE_PIN 5          //(must be HIGH during boot)
#define RIGHT_MOTOR_ENABLE_PIN 17    
#define RIGHT_MOTOR_FORWARD_IN1_PIN 16     
#define RIGHT_MOTOR_REVERSE_IN2_PIN 4      
#define LEFT_MOTOR_FORWARD_IN3_PIN 2                        //(must be LOW during boot and also connected to the on-board LED)
#define LEFT_MOTOR_REVERSE_IN4_PIN 15          //(must be HIGH during boot, prevents startup log if pulled LOW)

//#define UNUSED 36                       //(Input only GPIO, cannot be configured as output)
//#define UNUSED 39                       //(Input only GPIO, cannot be configured as output)
//#define UNUSED 34                       //(Input only GPIO, cannot be configured as output)
//#define UNUSED 35                       //(Input only GPIO, cannot be configured as output)
//#define UNUSED 32                       
//#define UNUSED 33                       
//#define UNUSED 25                       
//#define UNUSED 26                      
//#define UNUSED 27                     
//#define UNUSED 14                      
//#define UNUSED 12                       //(must be LOW during boot)
//#define UNUSED 13                     






#define AUTO_STOP_INTERVAL   2000
#define PID_INTERVAL          30


#define BAUDRATE           115200
#define MAX_ARG_LENGTH         16
#define LEFT                    0
#define RIGHT                   1

//PWM SETTINGS
#define PWM_FREQ                    5000
#define RESOLUTION                     8
#define MAX_PWM                      255  
#define RIGHT_PWM_CHANNEL              0
#define LEFT_PWM_CHANNEL               1

// SERIAL COM COMMANDS
#define ANALOG_READ    'a'
#define GET_BAUDRATE   'b'
#define PIN_MODE       'p'
#define DIGITAL_READ   'd'
#define READ_ENCODERS  'e'
#define MOTOR_SPEEDS   'm'
#define MOTOR_RAW_PWM  'o'
#define RESET_ENCODERS 'r'
#define UPDATE_PID     'u'
#define DIGITAL_WRITE  'w'
#define ANALOG_WRITE   'x'
#define ECHO           'Y'
#define ERROR          '!'