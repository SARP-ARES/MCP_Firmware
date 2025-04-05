#include "mbed.h"
#include <string>
#include "Motor.h"
#include "PID.h"
#include "EUSBSerial.h"

Motor motor(PA_8, PA_10, PB_2, PB_1, PB_15, PB_14); // test bench
// Motor motor1(PB_3, PB_5, PA_11, PA_12, PA_10, PA_9);
DigitalIn encoder_pin_a(PA_8);

// Motor angles
float theta1 = 0;
float theta2 = 0;

// PID pid(0.017, 0.00006, 3);
PID pid(0.017, 0, 1); // pretty fast and accurate to ~10 degrees
// 

// TODO: void toDegrees

// main() runs in its own thread in the OS
int main()
{

    // EUSBSerial pc(true); // New class Extended USB serial 
    printf("start :)\n");
    DigitalOut led1(PC_14);
    // Motor motor2(PA_6, PA_5, PB_14, PB_15, PB_13, PA_8);

    while (true) {
        ThisThread::sleep_for(10ms);

        float displacement = motor.getDisplacement();
        printf("Displacement: %d\t", (int)displacement);


        float power = pid.compute(displacement, 4, 10);
        printf("Power %d% \n", (int)(power*100));

        motor.motorPower(power);

    }
}

