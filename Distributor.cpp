#include "mbed.h"
#include <string>
#include "Distributor.h"
#include <cmath>


Distributor::Distributor() {}

std::pair<float, float> Distributor::getMotorOutputs(float ctrl)  {
    
    float control_input = 0;
    if (ctrl > 1) {
        control_input = 1;
    } else if (ctrl < -1) {
        control_input = -1;
    } else {
        control_input = ctrl;
    }
    
    float leftPull = 0.5 * (-control_input) + 0.5;
    float rightPull = 0.5 * control_input + 0.5;

    return std::make_pair(leftPull, rightPull);
}