#include "mbed.h"
#include <string>
#include "Distributor.h"
#include <cmath>
#include "I2CSerial.h"



Distributor::Distributor() {}

std::pair<float, float> Distributor::getMotorOutputs()  {
    printf("get motor outputs: ");

    char buf[DISTRIBUTOR_BUFFER_SIZE];
    buf[0] = '5';
    buf[1] = '0';
    
    printf("%c\n", buf[0]);

    std::string numStr = std::string() + buf[0] + buf[1];


    printf("Both char as a string: %s\n", numStr.c_str());
    printf("that str as an int: %d\n", (int) numStr.c_str());

    // string bufString = buf[0] + buf[1] + buf[2] + buf[3];

    float ctrl = ( (float) ((int) numStr.c_str()) ) / 100.0;
    printf("ctrl: %d\n", (int)ctrl*100);
    
    float leftPull = 0.5 * (-ctrl) + 0.5;
    float rightPull = 0.5 * ctrl + 0.5;

    return std::make_pair(leftPull, rightPull);
}