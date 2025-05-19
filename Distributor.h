// Class which translates a float input (from the flight controller) into two floats for each 
// motors linear displacement
#ifndef _DISTRIBUTOR_H_
#define _DISTRIBUTOR_H_


#include "mbed.h"
#include "I2CSerial.h"

#define DISTRIBUTOR_BUFFER_SIZE 8

class Distributor {
    private:
        I2CSerial* ser;

    public:
        Distributor(I2CSerial* ser);
        Distributor();

        std::pair<float, float> getMotorOutputs();
        std::pair<float, float> getMotorOutputsManual(float ctrl);
};


#endif // _DISTRIBUTOR_H_