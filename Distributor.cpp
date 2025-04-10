#include "mbed.h"
#include <string>
#include "Distributor.h"

Distributor::Distributor(I2CSerial* ser, size_t bufferSize) : ser(ser) {
    this->bufferSize = bufferSize;
}

std::pair<float, float> Distributor::getMotorOutputs()  {
    // Integer from -100 to 100
    char* buf[bufferSize];
    ser->read(buf, bufferSize);
    float ctrl = ( (float) ((int) buf) ) / 100.0;

    float leftPull = 0.5 * (-ctrl) + 0.5;
    float rightPull = 0.5 * ctrl + 0.5;

    return std::make_pair(leftPull, rightPull);
}