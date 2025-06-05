
#include <Arduino.h>
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

bool serialParser::isEndLine(char c) {
    return (c == '\r' || c == '\n');
}

bool serialParser::isDelimiter(char c) {
    return (c == ' ' || c == ',');
}

static bool convertstr2num(const char* argv, long &arg, SerialCommand &outCmd){
    char *endptr1 = nullptr;
    arg=0;

    arg = strtol(argv, &endptr1, 10);
    if (*endptr1 != '\0') {
        outCmd.error = SerialError::ARG_NOT_NUMERIC;
        outCmd.valid = false;
        return false;
        }
    return true;
}

void serialParser::fillCmd(SerialCommand &outCmd){
    outCmd.cmd = cmd;
    outCmd.arg1 = arg1;
    outCmd.arg2 = arg2;
    outCmd.error = SerialError::NONE;
    reset();
}

void serialParser::clearCmd(SerialCommand &outCmd){
    outCmd.cmd = '\0';
    outCmd.arg1 = 0;
    outCmd.arg2 = 0;
    reset();
}

bool serialParser::allClear(Stream &input){
    if(input.read()==ALL_CLEAR){
        flushCmdBuffer();
        reset();
        return true;
    }else{
        return false;
    }
}


void serialParser::flushCmdBuffer(SerialCommand &outCmd, Stream &input){
    // Clear all incoming data from the serial buffer
    while (input.available()) {
        input.read();
    }
    // Reset the output command structure and parser state
    clearCmd(outCmd);
    reset();
    return;
}

void serialParser::flushCmdBuffer(Stream &input){
    // Clear all incoming data from the serial buffer
    while (input.available()) {
        input.read();
    }
    // Reset the output command structure and parser state
    reset();
    return;
}



//--------------------------------------------------------------
// Reads and parses incoming serial data, then executes commands
bool serialParser::poll(SerialCommand &outCmd, Stream &input){
    while (input.available() > 0) {
        serialCommandChar = input.read(); // Read next character

        // If end-of-line received, finalize arguments and execute command
        if (isEndLine(serialCommandChar)){
            if (argCount == 1) argv1[argIndex] = '\0'; // Null-terminate first argument
            else if (argCount == 2) argv2[argIndex] = '\0'; // Null-terminate second argument

            if (cmdComplete && outCmd.valid) {

                if (argCount >= 1 && argv1[0] != '\0'){
                    if(!convertstr2num(argv1, arg1, outCmd)){
                        reset();
                        return false;
                    }
                }

                if (argCount >= 2 && argv2[0] != '\0'){
                    if(!convertstr2num(argv2, arg2, outCmd)){
                        reset();
                        return false;
                    }
                }

                fillCmd(outCmd);
                return true;

            }
            else{
                clearCmd(outCmd);
                return false;
            }
        }

        // If space or comma, move to next argument
        else if (isDelimiter(serialCommandChar) && outCmd.valid && delimToggle) {
            delimToggle=false;
            if (argCount == 0) argCount = 1; // Start first argument
            else if (argCount == 1)  {
                argv1[argIndex] = '\0'; // Null-terminate first argument
                argCount = 2;           // Move to second argument
                argIndex = 0;           // Reset index for argv2
            }
            continue;
        }else{
            if (argCount == 0 && serialCommandChar != ' ' && cmdComplete==false) {
                
                cmd = serialCommandChar;
                if(!isValidCommand(cmd)){
                    outCmd.valid = false;
                    outCmd.error = SerialError::INVALID_COMMAND;
                }
                cmdComplete = true;
                delimToggle=true;

            }else if (argCount == 1 && outCmd.valid){
                delimToggle=true;
                if(argIndex < MAX_ARG_LENGTH - 1) {
                    argv1[argIndex++] = serialCommandChar;
                } else {
                    outCmd.error = SerialError::ARG_OVERFLOW;
                    outCmd.valid = false;
                }
            }else if (argCount == 2 && outCmd.valid) {
                delimToggle=true;
                if (argIndex < MAX_ARG_LENGTH - 1) {
                    argv2[argIndex++] = serialCommandChar;
                } else {
                    outCmd.error = SerialError::ARG_OVERFLOW;
                    outCmd.valid = false;
                }
            }
        }
    }
    reset();
    return false;
}

//------------------------------------------------------
// Resets all parsing state and buffers for the next command
void serialParser::reset(){
    cmd = '\0';
    serialCommandChar = '\0';
    cmdComplete = false;
    arg1 =0;
    arg2 =0;
    memset(argv1, 0, sizeof(argv1));
    memset(argv2, 0, sizeof(argv2));
    argCount = 0;
    argIndex = 0;
    delimToggle = true;
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
