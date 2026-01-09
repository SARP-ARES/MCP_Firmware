// Class which translates a float inputs (ideally from the flight controller) into two floats 
// for each motors linear displacement
#ifndef _DISTRIBUTOR_H_
#define _DISTRIBUTOR_H_


#include "mbed.h"

#define DISTRIBUTOR_BUFFER_SIZE 8

class Distributor {

    public:
        Distributor();

        std::pair<float, float> getMotorOutputsManual(float ctrl);
};


#endif // _DISTRIBUTOR_H_