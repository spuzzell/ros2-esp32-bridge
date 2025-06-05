#include "config.h"
#include "diff_pid_controller.h"
#include "encoder_driver.h"
#include "motor_driver.h"
#include <Arduino.h>


extern roverEncoders encoders;
extern roverMotors motors;
extern long lastMotorCommand;


void diffController::reset(){
    moving=0;
    leftPID.reset();
    rightPID.reset();
}

void diffController::setMoving(bool state){
    moving=state;
    leftPID.setMoving(state);
    rightPID.setMoving(state);

}

unsigned long diffController::getNextPID(){
    return nextPID;
}

void diffController::writeNextPID(unsigned long value){
    nextPID = value;
}

void diffController::update(int arg1, int arg2){
    leftPID.setTargetTicksPerFrame(arg1);
    rightPID.setTargetTicksPerFrame(arg2);

    leftPID.update();
    rightPID.update();
}

void diffController::update(){
    leftPID.update();
    rightPID.update();
}

PIDController::PIDController(int motorID)
    : motorID(motorID), Kp(20.0), Kd(20.0), Ki(0.0), Ko(50.0) {
    reset();
}

void PIDController::reset() {
    targetTicksPerFrame = 0.0;
    encVal = encoders.readEncoder(motorID);
    prevEnc = encVal;
    prevInput = 0;
    iTerm = 0;
    output = 0;
    moving = 0;
}

void PIDController::setTargetTicksPerFrame(double target) {
    targetTicksPerFrame = target;
}

void PIDController::setMoving(bool state){
    moving=state;
}

void PIDController::update() {
    encVal=encoders.readEncoder(motorID);

    if(moving){
        if(prevInput !=0) reset();
        return;
    }

    runPID();
    motors.setMotorSpeed(motorID, output);
}

void PIDController::runPID() {
    int input = encVal - prevEnc;
    long pError = targetTicksPerFrame - input;
    output = (Kp * pError - Kd * (input - prevInput) + iTerm) / Ko;
    prevEnc = encVal;
    if (output >= MAX_PWM)
        output = MAX_PWM;
    else if (output <= -MAX_PWM)
        output = -MAX_PWM;
    else
        iTerm += Ki * pError;
    prevInput = input;
}