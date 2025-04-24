#include "mbed.h"
#include <string>
#include "Distributor.h"
#include <cmath>
#include "I2CSerial.h"



Distributor::Distributor() {}

std::pair<float, float> Distributor::getMotorOutputs()  {
    printf("get motor outputs\n");

    char buf[DISTRIBUTOR_BUFFER_SIZE];
    buf[0] = '5';
    buf[1] = '0';
    
    printf((string) buf[0]);

    // string bufString = buf[0] + buf[1] + buf[2] + buf[3];

    float ctrl = ( (float) ((int) buf) ) / 100.0;
    printf("ctrl: %d\n", (int)ctrl*100);
    
    float leftPull = 0.5 * (-ctrl) + 0.5;
    float rightPull = 0.5 * ctrl + 0.5;

    return std::make_pair(leftPull, rightPull);
}