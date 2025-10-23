#include "mbed.h"
#include "MotorCOTS.h"
#include "EUSBSerial.h"

// Updates class variables for motor state
void MotorCOTS::updateGlobals() {
    position = encoderCounter;
    angle = (encoderCounter * 360.0) / totalCounts;
    rotations = angle / 360.0;
}

// Tickes encoder count forward or backward on encoder a rise callback
void MotorCOTS::aRiseCallback() {
    aUp = true;

    if (!bUp) {
        encoderCounter++;
    } else {
        encoderCounter--;
    }
}

// Tickes encoder count forward or backward on encoder b rise callback
void MotorCOTS::bRiseCallback() {
    bUp = true;

    if (aUp) {
        encoderCounter++;
    } else {
        encoderCounter--;
    }
}

// Tickes encoder count forward or backward on encoder a fall callback
void MotorCOTS::aFallCallback() {
    aUp = false;

    if (bUp) {
        encoderCounter++;
    } else {
        encoderCounter--;
    }
}

// Tickes encoder count forward or backward on encoder b fall callback
void MotorCOTS::bFallCallback() {
    bUp = false;

    if (!aUp) {
        encoderCounter++;
    } else {
        encoderCounter--;
    }
}


// Initialize motor object, set encoder states
// Takes:  PinName for direction one control line, PinName for direction two control line, PinName for power throttle (PWM),
//         PinName for encoder A, PinName for encoder B, PID pointer for motor PID controller, EUSBSerial pointer for debugging serial
MotorCOTS::MotorCOTS(PinName directionOne, PinName directionTwo, PinName powerThrottle, PinName PINA, PinName PINB, PID* pid, EUSBSerial* pc) : encoderA(PINA), encoderB(PINB), 
                directionOne(directionOne), directionTwo(directionTwo), powerThrottle(powerThrottle), pid(pid), pc(pc) {

    // Init pins and set pin modes for encoders
    encoderA.mode(PullDown);
    encoderB.mode(PullDown);

    if (encoderA.read() == 1) {

        aUp = true;
    }
    if (encoderB.read() == 1) {
        bUp = true;
    }

    powerPositive = true;

    // Attach the address of the encoderCallback function to the each edge
    encoderA.rise([this]() {aRiseCallback();});
    encoderA.fall([this]() {aFallCallback();});
    encoderB.rise([this]() {bRiseCallback();});
    encoderB.fall([this]() {bFallCallback();});

}


// Set motor direction
// Takes:   int direction (1 for forward, -1 for backward)
void MotorCOTS::direction(int direction) {
    if (direction == 1) {
        ThisThread::sleep_for(2ms);
        directionTwo.write(0);
        directionOne.write(1);
        powerPositive = true;
    } else if (direction == -1) {
        ThisThread::sleep_for(2ms);
        directionOne.write(0);
        directionTwo.write(1);
        powerPositive = false;
    }
}

// Set motor power
// Takes:   float power (0 to 1 for forward, 0 to -1 for backward)
void MotorCOTS::motorPower(float power) {
    if (power > 0) {
        direction(1);
        powerThrottle.write(power);
    } else if (power < 0) {
        direction(-1);
        powerThrottle.write(-power);
    } else {
        powerThrottle.write(0);
    }
}

// Returns motor angle in degrees
int MotorCOTS::getDegrees() {
    updateGlobals();
    return (int)angle;
}

// Returns motor linear position in inches
float MotorCOTS::getPosition() {
    return static_cast<float>(getDegrees()) / 360.0 * PI * spoolDiameter;
}

// Moves motor to target position using PID controller
// Takes:   float target position in inches, int dt in milliseconds since last call
void MotorCOTS::toPosition(float pullPercent, int dt) {
    
    float currPos = getPosition();
    float targetPos = pullPercent*MAX_DEFLECTION;

    float power = pid->compute(currPos, targetPos, dt);

    if (currPos-targetPos < 0.5 && currPos-targetPos > -0.5) {
        motorPower(0);
    } else {
        motorPower(-power);
    }

    // Debug print statements 
    pc->printf("\t\tCurrent: %f\tTarget: %f", currPos, targetPos);
    // pc->printf("\tTarget pull percent: %f", pullPercent);
    // pc-printf("\tMax deflection: %f", MAX_DEFLECTION);
    // pc->printf("Difference: %f\t", currPos-targetPos);
}