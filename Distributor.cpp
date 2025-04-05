#include "mbed.h"
#include <string>
#include "Distributor.h"


float Distributor::getInput() {
    inputPin DigitalIn()
    return 1;
}

std::pair<float, float> Distributor::getMotorOutputs(PinName pin)  {


            float input = getInput();
            float leftPull = 0.5 * (-input) + 0.5;
            float rightPull = 0.5 * input + 0.5;

            return std::make_pair(leftPull, rightPull);
}