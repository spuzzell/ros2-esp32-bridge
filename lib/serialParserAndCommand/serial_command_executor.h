/*
 * serial_command_executor.h
 *
 * Declares the serialCommandExecutor class, which is responsible for executing
 * commands received over the serial interface. Supported commands include pin control,
 * encoder operations, motor control, and controller updates. The class provides a
 * modular interface for handling serial commands based on compile-time configuration.
 */

#pragma once
#include "serial_command.h"

class serialCommandExecutor
{
    public:

    long lastMotorCommand; // Timestamp of the last motor command sent
    serialCommandExecutor() : lastMotorCommand(0) {} // Constructor initializes lastMotorCommand
    void execute(const SerialCommand cmd); // Execute the given serial command
    
};