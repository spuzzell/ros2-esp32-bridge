/*
 * serial_command.h
 *
 * Defines the SerialCommand structure and SerialError enum used for parsing and
 * representing commands received over the serial interface. This header provides
 * the data structures and error handling utilities for serial command processing.
 * 
 */

#pragma once

#include <Arduino.h>


// Enum for possible serial command errors
enum class SerialError {
    NONE = 0,            // No error
    INVALID_COMMAND,     // Command is not recognized (aka not in config.h)
    ARG_NOT_NUMERIC,     // Argument (arg1 or arg2) is not a valid number
    ARG_OVERFLOW,        // Argument is too long to fit (more digits than MAX_ARG_LENGTH)
    EXECUTION_FAILED,    // Command execution failed
    UNKNOWN              // Unknown Error
};

// Structure representing a parsed serial command
struct SerialCommand {
    char cmd = '\0';     // Command character
    long arg1 = 0;       // First argument (optional)
    long arg2 = 0;       // Second argument (optional)
    bool valid = true;   // Flag indicating if the command is valid
    SerialError error = SerialError::NONE; // Error code if invalid
};

// Prints a human-readable error message for a SerialCommand
// if ENABLE_ERROR_MESSAGES is defined in config
inline void serialErrorMSG(SerialCommand cmd){
    switch (cmd.error) {
        case SerialError::INVALID_COMMAND:
                Serial.println("ERROR: Undefined command character recived received.");
                break;
            case SerialError::ARG_NOT_NUMERIC:
                Serial.println("ERROR: One or more arguments is not numeric.");
                break;
            case SerialError::ARG_OVERFLOW:
                Serial.println("ERROR: One or more arguments are too long.");
                break;
            default:
                Serial.println("ERROR: An unknown error has occured.");
                break;
        }
}