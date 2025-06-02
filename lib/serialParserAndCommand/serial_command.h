#pragma once



enum class SerialError {
    NONE = 0,
    INVALID_COMMAND,
    ARG1_NOT_NUMERIC,
    ARG2_NOT_NUMERIC,
    ARG1_TOO_LONG,
    ARG2_TOO_LONG,
    OVERFLOW,
    EXECUTION_FAILED,
    UNKNOWN
};

struct SerialCommand {
    char cmd = '\0';
    long arg1 = 0;
    long arg2 = 0;
    bool valid = false;
    SerialError error = SerialError::NONE;
};

inline void serialErrorMSG(SerialCommand cmd){
    switch (cmd.error) {
        case SerialError::INVALID_COMMAND:
                Serial.println("ERROR: Invalid command received.");
                break;
            case SerialError::ARG1_NOT_NUMERIC:
                Serial.println("ERROR: Argument 1 is not numeric.");
                break;
            case SerialError::ARG2_NOT_NUMERIC:
                Serial.println("ERROR: Argument 2 is not numeric.");
                break;
            case SerialError::ARG1_TOO_LONG:
                Serial.println("ERROR: Argument 1 too long.");
                break;
            case SerialError::ARG2_TOO_LONG:
                Serial.println("ERROR: Argument 2 too long.");
                break;
            default:
                Serial.println("ERROR: Unknown");
                break;
        }
}