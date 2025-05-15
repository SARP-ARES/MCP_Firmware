#include "mbed.h"
#include "EUSBSerial.h"

int encoder_counter = 0;

void fallCallback() {
        encoder_counter++;
    }

int main() {

    DigitalOut led(PC_13);
    DigitalOut led_ext(PB_8);
    led.write(0);

    DigitalOut motor1(PA_2);
    DigitalOut motor2(PA_3);
    InterruptIn encoderA(PA_6);
    InterruptIn encoderB(PA_7);

    EUSBSerial pc(false);

    ThisThread::sleep_for(1s);
    pc.printf("Serial Port Connected!\n");

    // attach callbacks to the interupts
    encoderA.fall(fallCallback);
    encoderB.fall(fallCallback);

    // bool m1 = true;
    // bool m2 = false;

    // drive motor 
    motor1.write(1);
    motor2.write(0); 
    // ThisThread::sleep_for(500ms);
    // pc.printf("Starting Main Loop!\n");
    while (true) {
        ThisThread::sleep_for(100ms);
        // led_ext = !led_ext;

        pc.printf("Encoder Count: %d\n", encoder_counter);
        if (encoder_counter >= 150 * 64 * 5) { // 5 revolutions (150:1 gear ratio, 64 CPR)
            led_ext.write(1);
            pc.printf("Encoder Threshold Reached!\n");
            motor1.write(0);
            break;
        } 


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