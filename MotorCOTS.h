#ifndef _MOTOR_H_
#define _MOTOR_H_


#include "mbed.h"
#include "PID.h"

class Motor {
    private:
        DigitalOut directionOne;
        DigitalOut directionTwo;
        PwmOut powerThrottle;
        InterruptIn encoderA;
        InterruptIn encoderB;

        // Encoder state boolean
        bool aUp = false;
        bool bUp = false;

        const float PI = 3.1415926535;

        volatile int encoderCounter = 0; // Counter for the encoder ticks
        const int countsPerRev = 64;
        const float gearRatio = 150; // Might need to be updated for different motors
        const float spoolDiameter = 0.5; // Spool Diameter IN INCHES. Sidenote in realidad this will not be const
        const float MAX_DEFLECTION = 36; // INCHES

        const float totalCounts = countsPerRev * gearRatio; // Total counts per rotation of motor output shaft
        long position;
        float angle;
        float rotations;
        bool powerPositive;

        void aRiseCallback();
        void bRiseCallback();
        void aFallCallback();
        void bFallCallback();

    public:

        void updateGlobals();


        

        Motor(PinName directionOne, PinNAme directionTwo, PinName powerThrottle, PinName encoderA, PinName encoderB);

        void direction(int direction);
        void motorPower(float power);

        int getDegrees();
};

#endif //_MOTOR_H_