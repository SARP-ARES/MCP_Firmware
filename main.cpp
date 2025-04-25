#include "mbed.h"
#include <cstdio>
#include <string>
#include "I2CSerial.h"

#include "Motor.h"
#include "PID.h"
#include "Distributor.h"


int leftExtension = 0;
int rightExtension = 0;


// main() runs in its own thread in the OS
int main()
{
    printf("main\n");

    PID pid(0.017, 0, 1); // No idea if these values work
    Motor motor(PA_8, PA_10, PB_2, PB_1, PB_15, PB_14, pid); // test bench
    //Motor motor1(PB_3, PB_5, PA_11, PA_12, PA_10, PA_9, pid); // these are the mcpcb
    //Motor motor2(PA_6, PA_5, PB_14, PB_15, PB_13, PA_8, pid);
    
    
    // Now with no input!
    Distributor distributor;


    while (true) {
        ThisThread::sleep_for(1000ms);
        /*              commented out for test bench */
        // get distributed pair
        std::pair<float, float> spoolExtensions = distributor.getMotorOutputs();
        // if the first float is a NAN, keep each extension value the same
        leftExtension = (isnan(spoolExtensions.first)) ? spoolExtensions.first : leftExtension;
        rightExtension = (isnan(spoolExtensions.first)) ? spoolExtensions.second : rightExtension;

        printf("Left: %d, Right: %d\n", (int) leftExtension*100, (int) rightExtension*100);
    }
}