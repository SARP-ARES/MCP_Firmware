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

    if (!bUp) {
        encoderCounter++;
    } else {
        encoderCounter--;
    }
}


void Motor::bRiseCallback() {
    bUp = true;

    if (aUp) {
        encoderCounter++;
    } else {
        encoderCounter--;
    }
}

void Motor::aFallCallback() {
    aUp = false;

    if (bUp) {
        encoderCounter++;
    } else {
        encoderCounter--;
    }
}

void Motor::bFallCallback() {
    bUp = false;

    if (!aUp) {
        encoderCounter++;
    } else {
        encoderCounter--;
    }
}


Motor::Motor(PinName PIN_A, PinName PIN_B, PinName MOTOR_1, PinName MOTOR_2, PinName MOTOR_3,
            PinName MOTOR_4, const PID& pidObject) : encoderA(PIN_A), encoderB(PIN_B), motorPin1(MOTOR_1), 
            motorPin3(MOTOR_3), motorPin2(MOTOR_2), motorPin4(MOTOR_4), motorPID(pidObject) {

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

void Motor::motorPower(float power) {
    printf("motor power\n");
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

// Takes an integer for degrees to spin from current position
void Motor::spinDegrees(int degrees) {
    printf("spin degrees\n");
    int initial = getDegrees();
    if (degrees > 0) {
        printf("positive\n");
        motorPower(1);
        printf("on\n");
        while (angle < (initial + degrees)) {
            printf("a: %d\n", (int)encoderA.read());
            printf("b: %d\n", (int)encoderB.read());
            ThisThread::sleep_for(10ms);
            updateGlobals();
        }
        printf("off\n");
        motorPower(0);
    } else if (degrees < 0) {
        printf("negative motor power\n");
        motorPower(-1);
        ThisThread::sleep_for(100ms);
        while (angle > (initial + degrees)) {
            ThisThread::sleep_for(10ms);
            updateGlobals();
        }
        printf("off");
        motorPower(0);
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
        motorPower(power);

}