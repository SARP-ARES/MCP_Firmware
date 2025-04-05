// Class which translates a float input (from the flight controller) into two floats for each 
// motors linear displacement
#ifndef _DISTRIBUTOR_H_
#define _DISTRIBUTOR_H_


#include "mbed.h"
#include <iostream>
#include <utility>

class Distributor {
    private:
        float getInput();
    
    public:
        std::pair<float, float> getMotorOutputs(PinName pin);
};


#endif // _DISTRIBUTOR_H_