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
    printf("main");

    PID pid(0.017, 0, 1); // No idea if these values work
    Motor motor(PA_8, PA_10, PB_2, PB_1, PB_15, PB_14, pid); // test bench
    Motor motor1(PB_3, PB_5, PA_11, PA_12, PA_10, PA_9, pid); // these are the mcpcb
    Motor motor2(PA_6, PA_5, PB_14, PB_15, PB_13, PA_8, pid);


    // on stm with breakout: PB_7 PB_8, only works on blackpill
    I2CSerial ser(PB_7, PB_8, 0x32, true);
    
    
    Distributor distributor(&ser);


        //  DEBUG  //
    // printf("start :)\n");
    // DigitalOut led1(PC_14);

    while (true) {
        ThisThread::sleep_for(10ms);

        std::pair<float, float> spoolExtensions = distributor.getMotorOutputs();
        
        // if the first float is a NAN, keep each extension value the same4
        leftExtension = (isnan(spoolExtensions.first)) ? leftExtension : spoolExtensions.first;
        rightExtension = (isnan(spoolExtensions.first)) ? rightExtension: spoolExtensions.second;

        motor1.lineTo(leftExtension, 10);
        motor2.lineTo(rightExtension, 10);

        printf("main loop\n");
    }
}