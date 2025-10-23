// Class which controls a COTS motor with encoder feedback

#ifndef _MOTORCOTS_H_
#define _MOTORCOTS_H_


#include "mbed.h"
#include "PID.h"
#include "EUSBSerial.h"


class MotorCOTS {
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
        const float spoolDiameter = 0.6345; // Spool Diameter IN INCHES. Sidenote in realidad this will not be const
        const float MAX_DEFLECTION = 54.0f; // INCHES

        const float totalCounts = countsPerRev * gearRatio; // Total counts per rotation of motor output shaft
        long position;
        float angle;
        float rotations;
        bool powerPositive;

        void aRiseCallback();
        void bRiseCallback();
        void aFallCallback();
        void bFallCallback();

        PID* pid;
        EUSBSerial* pc;

    public:

        void updateGlobals();


        

        // MotorCOTS(PinName directionOne, PinName directionTwo, PinName powerThrottle, PinName PINA, PinName PINB, PID* pid);
        MotorCOTS(PinName directionOne, PinName directionTwo, PinName powerThrottle, PinName PINA, PinName PINB, PID* pid, EUSBSerial* pc);

        void direction(int direction);
        void motorPower(float power);

        int getDegrees();
        float getPosition();

        void toPosition(float target, int dt);
};

#endif //_MOTORCOTS_H_