#include <Arduino.h>
#include "config.h"

#ifdef ENABLE_ENCODERS
#include "encoder_driver.h"
roverEncoders encoders;
#endif

#ifdef ENABLE_MOTORS
#include "motor_driver.h"
roverMotors motors;
#endif

#ifdef ENABLE_CONTROLLER
#include "diff_pid_controller.h"
diffController controller;
#endif

#include "serial_parser.h"
#include "serial_command.h"
#include "serial_command_executor.h"


serialParser parser;
serialCommandExecutor executor;


void setup() {
#ifdef ENABLE_MOTORS
    motors.init();
#endif

#ifdef ENABLE_ENCODERS
    encoders.init();
#endif

#ifdef ENABLE_CONTROLLER
    controller.reset();
#endif
    parser.init();
}


void loop(){
    SerialCommand cmd{};
   // if (parser.poll(cmd)) {
   // }else{
    //    serialErrorMSG(cmd);
    //}

#ifdef ENABLE_CONTROLLER
    if (millis() > controller.getNextPID()) {
        controller.update();
        controller.writeNextPID(controller.getNextPID() + PID_INTERVAL);
    }
#endif    
#ifdef ENABLE_MOTORS
    if ((millis() - executor.lastMotorCommand) > AUTO_STOP_INTERVAL)
        motors.setMotorSpeeds(0, 0);
#endif

}



    //if (parser.poll(cmd)) {
        //if (cmd.valid) {
            //executor.execute(cmd);
       // } else {
            //serialErrorMSG(cmd);
       // }
   // }

