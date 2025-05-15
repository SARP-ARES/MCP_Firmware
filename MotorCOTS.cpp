#include "mbed.h"
#include "MotorCOTS.h"

void MotorCOTS::updateGlobals() {
    position = encoderCounter;
    angle = (encoderCounter * 360.0) / totalCounts;
    rotations = angle / 360.0;

}

void MotorCOTS::aRiseCallback() {
    aUp = true;

    if (!bUp) {
        encoderCounter++;
    } else {
        encoderCounter--;
    }
}


void MotorCOTS::bRiseCallback() {
    bUp = true;

    if (aUp) {
        encoderCounter++;
    } else {
        encoderCounter--;
    }
}

void MotorCOTS::aFallCallback() {
    aUp = false;

    if (bUp) {
        encoderCounter++;
    } else {
        encoderCounter--;
    }
}

void MotorCOTS::bFallCallback() {
    bUp = false;

    if (!aUp) {
        encoderCounter++;
    } else {
        encoderCounter--;
    }
}

MotorCOTS::MotorCOTS(PinName directionOne, PinName directionTwo, PinName powerThrottle, PinName PINA, PinName PINB) : encoderA(PINA), encoderB(PINB), 
                directionOne(directionOne), directionTwo(directionTwo), powerThrottle(powerThrottle) {

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

int MotorCOTS::getDegrees() {
    updateGlobals();
    return (int)angle;
}