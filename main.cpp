#include "mbed.h"
#include <string>
#include "Motor.h"
#include "PID.h"
#include "EUSBSerial.h"

Motor motor(PA_8, PA_10, PB_2, PB_1, PB_15, PB_14); // test bench

size_t bufferSize = 8;

int leftExtension = 0;
int rightExtension = 0;

// main() runs in its own thread in the OS
int main()
{


    PID pid(0.017, 0, 1);
    Motor motor(PA_8, PA_10, PB_2, PB_1, PB_15, PB_14, pid); // test bench
    // Motor motor1(PB_3, PB_5, PA_11, PA_12, PA_10, PA_9); // I think this is the mcpb


    I2CSerial ser(PB_7, PB_8, 0x32, true);
    
     
    // on stm with breakout: PB_7 PB_8
    Distributor distributor(ser, bufferSize);





    // EUSBSerial pc(true); // New class Extended USB serial 
    printf("start :)\n");
    DigitalOut led1(PC_14);
    // Motor motor2(PA_6, PA_5, PB_14, PB_15, PB_13, PA_8);

    while (true) {
        ThisThread::sleep_for(10ms);
        // get distributed pair
        std::pair<float, float> spoolExtensions = distributor.getMotorOutputs();
        // get left float
        leftExtension = spoolExtensions.first;
        // get right float
        rightExtension = spoolExtensions.second;

        // go to position
        motor.lineTo(4, 10);
    }
}

