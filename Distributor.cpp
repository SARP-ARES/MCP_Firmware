#include "mbed.h"
#include <string>
#include "Distributor.h"
#include <cmath>

Distributor::Distributor() {}

// Takes a float [-1, 1] indicating full left to full right steering respectively
// Returns a tuple of floats representing the retraction of the left motor and right motor respectively 
std::pair<float, float> Distributor::getMotorOutputs(float ctrl)  {
    float leftPull = 0.5 * (-ctrl) + 0.5;
    float rightPull = 0.5 * ctrl + 0.5;

    return std::make_pair(leftPull, rightPull);
}