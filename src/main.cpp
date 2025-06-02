#include <Arduino.h>
#include "config.h"

#include "encoder_driver.h"
#include "motor_driver.h"
#include "diff_pid_controller.h"

#include "serial_parser.h"
#include "serial_command.h"
#include "serial_command_executor.h"


roverEncoders encoders;
roverMotors motors;
diffController controller;

serialParser parser;
serialCommandExecutor executor;


void setup() {
    motors.init();
    encoders.init();
    controller.reset();
    parser.init();
}


void loop(){
    SerialCommand cmd;
    if (parser.poll(cmd)) {
        if (cmd.valid) {
            executor.execute(cmd);
        } else {
            serialErrorMSG(cmd);
        }
    }

    if (millis() > controller.getNextPID()) {
        controller.update();
        controller.writeNextPID(controller.getNextPID() + PID_INTERVAL);
    }

    if ((millis() - executor.lastMotorCommand) > AUTO_STOP_INTERVAL)
        motors.setMotorSpeeds(0, 0);
}