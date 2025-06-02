#ifndef TEST_DESKTOP
#include <Arduino.h>
#endif

#include "config.h"
#include "serial_parser.h"
#include "serial_command.h"

//----------------------------------------------------------
// Initializes the serial port with the configured baud rate
void serialParser::init2(){
    #ifndef TEST_DESKTOP
    Serial.begin(BAUDRATE);
    #endif
}


//--------------------------------------------------------------
// Reads and parses incoming serial data, then executes commands
bool serialParser::poll(SerialCommand &outCmd, Stream &input){
    while (input.available() > 0) {
        serialCommandChar = input.read(); // Read next character

        // If end-of-line received, finalize arguments and execute command
        if (serialCommandChar == '\r' || serialCommandChar == '\n'){
            if (argCount == 1) argv1[argIndex] = '\0'; // Null-terminate first argument
            else if (argCount == 2) argv2[argIndex] = '\0'; // Null-terminate second argument

            if (cmdComplete && isValidCommand(cmd) && !errorStateOverFlow) {
                char *endptr1 = nullptr;
                char *endptr2 = nullptr;
                arg1 = 0;
                arg2 = 0;
                
                if (argCount >= 1 && argv1[0] != '\0') {
                    arg1 = strtol(argv1, &endptr1, 10);
                    if (*endptr1 != '\0') {
                        outCmd.error = SerialError::ARG2_NOT_NUMERIC;
                        outCmd.valid = false;
                        reset();
                        return false;
                    }
                }

                if (argCount == 2 && argv2[0] != '\0') {
                    arg2 = strtol(argv2, &endptr2, 10);
                    if (*endptr2 != '\0') {
                        outCmd.error = SerialError::ARG2_NOT_NUMERIC;
                        outCmd.valid = false;
                        reset();
                        return false;
                    }
                }

                outCmd.cmd = cmd;
                outCmd.arg1 = arg1;
                outCmd.arg2 = arg2;
                outCmd.valid = true;
                outCmd.error = SerialError::NONE;
                reset();
                return true;
            }
            else{
                outCmd.cmd = '\0';
                outCmd.arg1 = 0;
                outCmd.arg2 = 0;
                outCmd.valid = false;
                if(!errorStateOverFlow) outCmd.error = SerialError::INVALID_COMMAND;
                reset();
                return false;
            }
        }

        // If space or comma, move to next argument
        else if (serialCommandChar == ' '|| serialCommandChar == ',') {
            if (argCount == 0) argCount = 1; // Start first argument
            else if (argCount == 1)  {
                argv1[argIndex] = '\0'; // Null-terminate first argument
                argCount = 2;           // Move to second argument
                argIndex = 0;           // Reset index for argv2
            }
            continue;
        }

        // Otherwise, parse command and arguments
        else{
            // First character is the command
            if (argCount == 0 && serialCommandChar != ' ' && cmdComplete== false) {
                cmd = serialCommandChar;
                cmdComplete = true;
            }
            // Fill first argument buffer
            else if (argCount == 1 && !errorStateOverFlow)
                if(argIndex < MAX_ARG_LENGTH - 1) {
                    argv1[argIndex++] = serialCommandChar;
                } else {
                    outCmd.error = SerialError::ARG1_TOO_LONG;
                    outCmd.valid = false;
                    errorStateOverFlow=true;
            }
            // Fill second argument buffer
            else if (argCount == 2 && !errorStateOverFlow) {
                if (argIndex < MAX_ARG_LENGTH - 1) {
                    argv2[argIndex++] = serialCommandChar;
                } else {
                    outCmd.error = SerialError::ARG1_TOO_LONG;
                    outCmd.valid = false;
                    errorStateOverFlow=true;
                }
            }

        }
    }

    return false;
}

//------------------------------------------------------
// Resets all parsing state and buffers for the next command
void serialParser::reset(){
    cmd = '\0';
    serialCommandChar = '\0';
    cmdComplete = false;
    memset(argv1, 0, sizeof(argv1));
    memset(argv2, 0, sizeof(argv2));
    argCount = 0;
    argIndex = 0;
    errorStateOverFlow = false;
}

bool isValidCommand(char c) {
    switch (c) {
        case ANALOG_READ:
        case GET_BAUDRATE:
        case PIN_MODE:
        case DIGITAL_READ:
        case READ_ENCODERS:
        case MOTOR_SPEEDS:
        case MOTOR_RAW_PWM:
        case RESET_ENCODERS:
        case UPDATE_PID:
        case DIGITAL_WRITE:
        case ANALOG_WRITE:
        case ECHO:
        case ERROR:
            return true;
        default:
            return false;
    }
}
