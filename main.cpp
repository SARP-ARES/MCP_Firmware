#include "mbed.h"
#include <cstdio>
#include <string>
#include "I2CSerial.h"

#include "Motor.h"
#include "PID.h"
#include "Distributor.h"


float leftExtension = 0;
float rightExtension = 0;


// main() runs in its own thread in the OS
int main()
{
    
    

    // printf("main\n");

    PID pid(0.1, 0, 0); // No idea if these values work

    // these are the mcps motor ports
    Motor motor1(PB_3, PA_14, PA_11, PA_10, PA_12, PA_9, &pid); // left motor WITH PA_14 SUBSTITUTION
    Motor motor2(PA_6, PA_5, PB_14, PB_15, PB_13, PA_8, &pid); // right motor

    // Motor motor(PA_8, PA_10, PB_2, PB_1, PB_15, PB_14, &pid); // test bench


    // Now with no input!
    Distributor distributor;


    // DEBUG CODE
    motor2.motorPower(0.2);
    
    

    while (true) {

        ThisThread::sleep_for(200ms);        
        

        // std::pair<float, float> spoolExtensions = distributor.getMotorOutputs();
        
        // // // if the first float is a NAN, keep each extension value the same4
        // leftExtension = (isnan(spoolExtensions.first)) ? leftExtension : spoolExtensions.first;
        // rightExtension = (isnan(spoolExtensions.first)) ? rightExtension: spoolExtensions.second;

        // motor1.lineTo(leftExtension, 50);
        // motor2.lineTo(rightExtension, 50);
    }
}