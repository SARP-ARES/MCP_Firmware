#include "mbed.h"
#include <string>
#include "Motor.h"
#include <math.h>
#include "PID.h"


void Motor::updateGlobals() {
    position = encoderCounter;
    angle = (encoderCounter * 360.0) / totalCounts;
    rotations = angle / 360.0;

}

void Motor::aRiseCallback() {
    aUp = true;

    if (!bUp)   encoderCounter++;
    else        encoderCounter--;
}


void Motor::bRiseCallback() {
    bUp = true;

    if (aUp)    encoderCounter++;
    else        encoderCounter--;
}

void Motor::aFallCallback() {
    aUp = false;

    if (bUp)    encoderCounter++;
    else        encoderCounter--;
}

void Motor::bFallCallback() {
    bUp = false;

    if (!aUp)   encoderCounter++;
    else        encoderCounter--;
}


Motor::Motor(PinName PIN_A, PinName PIN_B, PinName MOTOR_1, PinName MOTOR_2, PinName MOTOR_3,
            PinName MOTOR_4, PID* pid) : encoderA(PIN_A), encoderB(PIN_B), motorPin1(MOTOR_1), 
            motorPin3(MOTOR_3), motorPin2(MOTOR_2), motorPin4(MOTOR_4), pid(pid) {

    // Init pins and set pin modes for encoders
    encoderA.mode(PullDown);
    encoderB.mode(PullDown);

    if (encoderA.read() == 1) aUp = true;
    if (encoderB.read() == 1) bUp = true;

    powerPositive = true;

    // Attach the address of the encoderCallback function to the each edge
    encoderA.rise([this]() {aRiseCallback();});
    encoderA.fall([this]() {aFallCallback();});
    encoderB.rise([this]() {bRiseCallback();});
    encoderB.fall([this]() {bFallCallback();});

}

// Returns motor linear position in inches
float Motor::getPosition() {
    return static_cast<float>(getDegrees()) / 360.0 * PI * spoolDiameter;
}

void Motor::motorPower() {
    if (power > 0) {
        if (!powerPositive) {
                    motorPin1.write(0);
        motorPin2.write(0);
        motorPin3.write(0);
        motorPin4.write(0);

            ThisThread::sleep_for(2ms);
        }
        powerPositive = true;
        //set forward pins proportional to powe
        motorPin1.write(1);
        motorPin4.write(power);
    } else if (power < 0) {
        if (powerPositive) {
                    motorPin1.write(0);
        motorPin2.write(0);
        motorPin3.write(0);
        motorPin4.write(0);

            ThisThread::sleep_for(2ms);
        }
        powerPositive = false;
        //set backwards pins prop to |power|
        this->motorPin3.write(1);
        this->motorPin2.write(-power);

    } else {
        //set all pins to off
        motorPin1.write(0);
        motorPin2.write(0);
        motorPin3.write(0);
        motorPin4.write(0);
    }
}


int Motor::getDegrees() {
    updateGlobals();
    return (int)angle;
}

// Gets the current displacement of the line as opposed to degrees
// Returns inches because spool diameter is in inches
long Motor::getDisplacement() {
    updateGlobals();
    return angle * spoolDiameter * PI / 360;
}

float Motor::lineTo(float retraction, int delay) {
        float inches = MAX_DEFLECTION * retraction;
        float displacement = getDisplacement();
        float power = pid->compute(displacement, inches, delay);
        motorPower();
        return power;
}

/** 
 * @brief Sends the motor to a given position 
 * @param pullPercent Percent of line to pull in
 * @param dt Time differential for PID controller
 */
float Motor::toPosition(float pullPercent, int dt) {
    
    float currPos = getPosition();
    float targetPos = pullPercent*MAX_DEFLECTION;

    power = -pid->compute(currPos, targetPos, dt);

    if (currPos-targetPos < 0.5 && currPos-targetPos > -0.5) power = 0.0f;
    
    motorPower(); 
    
    return power; 
}