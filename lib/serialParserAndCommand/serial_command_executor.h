#pragma once
#include "config.h"
#include "serial_command.h"



class serialCommandExecutor
{
    public:

    long lastMotorCommand;
    
    void execute(const SerialCommand cmd);
    
};