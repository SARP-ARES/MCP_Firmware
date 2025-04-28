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

    PID pid(0.017, 0, 1); // No idea if these values work
    DigitalOut led1(PC_14);


    ThisThread::sleep_for(500ms);
    led1.write(1);
    ThisThread::sleep_for(500ms);
    led1.write(0);

    // these are the mcps motor ports
    // Motor motor1(PB_3, PB_5, PA_11, PA_12, PA_10, PA_9, &pid); // left motor 
    Motor motor1(PB_3, PB_5, PA_11, PA_12, PA_10, PA_9, &pid); // test
    // Motor motor2(PA_6, PA_5, PB_14, PB_15, PB_13, PA_8, &pid); // right motor
    
    
    // Now with no input!
    // Distributor distributor;


    while (true) {
        ThisThread::sleep_for(500ms);
        led1.write(1);
        ThisThread::sleep_for(500ms);
        led1.write(0);


        // std::pair<float, float> spoolExtensions = distributor.getMotorOutputs();
        
        // // if the first float is a NAN, keep each extension value the same4
        // leftExtension = (isnan(spoolExtensions.first)) ? leftExtension : spoolExtensions.first;
        // rightExtension = (isnan(spoolExtensions.first)) ? rightExtension: spoolExtensions.second;

        // motor1.lineTo(leftExtension, 10);
        // motor2.lineTo(rightExtension, 10);
    }
}