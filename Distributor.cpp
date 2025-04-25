#include "mbed.h"
#include <string>
#include "Distributor.h"
#include <cmath>
#include "I2CSerial.h"



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


    // Buf processing
    int digit1 = 100*( buf[1] - '0' );
    int digit2 = 10*( buf[2] - '0' );
    int digit3 = buf[3] - '0';
    int sum = digit1 + digit2 + digit3;
    if (buf[0] == '-') {sum*=-1;} 

    printf("Distributor turn angle: %d\n", sum);


    float ctrl = ( (float) sum ) / 100.0;    
    float leftPull = 0.5 * (-ctrl) + 0.5;
    float rightPull = 0.5 * ctrl + 0.5;

    return std::make_pair(leftPull, rightPull);    
}