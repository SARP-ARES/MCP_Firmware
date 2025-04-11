#include "mbed.h"
#include <string>
#include "Distributor.h"
#include <cmath>


Distributor::Distributor(I2CSerial* ser) : ser(ser) {}

std::pair<float, float> Distributor::getMotorOutputs()  {
    // Integer from -100 to 100
    char buf[DISTRIBUTOR_BUFFER_SIZE];
    if ( !ser->readline(buf, DISTRIBUTOR_BUFFER_SIZE) ) {
        return std::make_pair(NAN, 0);          // whats' the differnece bewteen a nan("1") and a different nan
    }
    int input;
    int res = sscanf(buf, "%d", &input);
    if (res != 1) {
        return std::make_pair(NAN, 1);
    }
    
    float ctrl = ( (float) ((int) buf) ) / 100.0;
    
    float leftPull = 0.5 * (-ctrl) + 0.5;
    float rightPull = 0.5 * ctrl + 0.5;

    return std::make_pair(leftPull, rightPull);
}