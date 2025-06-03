#pragma once

enum class SerialError {
    NONE = 0,
    INVALID_COMMAND,
    ARG_NOT_NUMERIC,
    ARG_OVERFLOW,
    EXECUTION_FAILED,
    UNKNOWN
};

struct SerialCommand {
    char cmd = '\0';
    long arg1 = 0;
    long arg2 = 0;
    bool valid = true;
    SerialError error = SerialError::NONE;
};

inline void serialErrorMSG(SerialCommand cmd){
    switch (cmd.error) {
        case SerialError::INVALID_COMMAND:
                Serial.println("ERROR: Invalid command received.");
                break;
            case SerialError::ARG_NOT_NUMERIC:
                Serial.println("ERROR: Argument 1 is not numeric.");
                break;
            case SerialError::ARG_OVERFLOW:
                Serial.println("ERROR: Argument 1 too long.");
                break;
            default:
                Serial.println("ERROR: Unknown");
                break;
        }
}