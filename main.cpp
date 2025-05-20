#include "mbed.h"
#include "EUSBSerial.h"

int encoder_counter = 0;

void fallCallback() {
        encoder_counter++;
    }

int main() {

    EUSBSerial pc(false);
    ThisThread::sleep_for(1s);
    pc.printf("Serial Port Connected!\n");

    ThisThread::sleep_for(3s);
    DigitalOut dir1(PB_8);
    DigitalOut dir2(PB_9);
    DigitalOut throttle(PA_1);
    DigitalOut led(PC_13);

    // set direction (constant)
    dir1.write(0);
    dir2.write(1);



    // DigitalOut motor1(PA_2);
    // DigitalOut motor2(PA_3);
    // InterruptIn encoderA(PA_6);
    // InterruptIn encoderB(PA_7);

    

    // attach callbacks to the interupts
    // encoderA.fall(fallCallback);
    // encoderB.fall(fallCallback);


    // ThisThread::sleep_for(500ms);
    pc.printf("Starting Main Loop!\n");
    while (true) {
        // ThisThread::sleep_for(100ms);

        ThisThread::sleep_for(3s);

        // start motor
        pc.printf("Motor On\n");
        throttle.write(1);
        led.write(0);

        ThisThread::sleep_for(3s);

        // stop motor
        pc.printf("Motor Off\n");
        throttle.write(0);
        led.write(1);


        // led_ext = !led_ext;

        // pc.printf("Encoder Count: %d\n", encoder_counter);
        // if (encoder_counter >= 150 * 64 * 5) { // 5 revolutions (150:1 gear ratio, 64 CPR)
        //     led_ext.write(1);
        //     pc.printf("Encoder Threshold Reached!\n");
        //     motor1.write(0);
        //     break;
        // } 


        // led = !led;
        // led_ext = !led_ext;
        // motor1.write(0);
        // motor2.write(0);
        // ThisThread::sleep_for(50ms);
        // motor1.write(m1);
        // motor2.write(m2);
        // m1 = !m1;
        // m2 = !m2;
        // ThisThread::sleep_for(2s);

    }
}