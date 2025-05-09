#include "mbed.h"
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

    PID pid(0.017, 0, 1); // No idea if these values work
    // Blackpill Motor config
    Motor motor(PA_6, PA_7, PA_1, PA_2, PA_5, PA_3, &pid);


    // // on stm with breakout: PB_7 PB_8
    // I2CSerial ser(PB_7, PB_8, 0x32, true);
    
    
    // Distributor distributor(&ser);
    

        //  DEBUG  //
    // printf("start :)\n");
    DigitalOut led(PC_13);

    while (true) {
        ThisThread::sleep_for(250ms);
        led.write(1);
        ThisThread::sleep_for(250ms);
        led.write(0);
        // // get distributed pair
        // std::pair<float, float> spoolExtensions = distributor.getMotorOutputs();
        // // if the first float is a NAN, keep each extension value the same
        // leftExtension = (isnan(spoolExtensions.first)) ? spoolExtensions.first : leftExtension;
        // rightExtension = (isnan(spoolExtensions.first)) ? spoolExtensions.second : rightExtension;

        // // go to position
        // motor1.lineTo(leftExtension, 10);
        // motor2.lineTo(rightExtension, 10);
    }
}

