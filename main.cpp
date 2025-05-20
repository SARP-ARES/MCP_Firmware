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
    DigitalOut led(PC_13);

    // MOTOR 1
    DigitalOut m1_dir1(PB_8);
    DigitalOut m1_dir2(PB_9);
    DigitalOut m1_throttle(PA_1);
    DigitalOut m1_encoderA_out(PA_6);
    DigitalOut m1_encoderB_out(PA_7);

    // MOTOR 2
    DigitalOut m2_dir1(PA_10);
    DigitalOut m2_dir2(PA_9);
    DigitalOut m2_throttle(PA_8);
    DigitalOut m2_encoderA_out(PA_15);
    DigitalOut m2_encoderB_out(PB_3);


    // direction 1, direction 2, throttle, encoder a, encoder b
    // MotorCOTS motor1(PB_8, PB_9, PA_1, PA_6, PA_7, &pid, &pc);
    // MotorCOTS motor2(PA_10, PA_9, PA_8, PA_15, PB_3, &pid, &рс);

    // set directions (constant)
    m1_dir1.write(0);
    m1_dir2.write(1);
    m2_dir1.write(0);
    m2_dir2.write(1);

    // DigitalOut motor1(PA_2);
    // DigitalOut motor2(PA_3);

    InterruptIn m1_encoderA(PA_6);
    InterruptIn m1_encoderB(PA_7);
    InterruptIn m2_encoderA(PA_15);
    InterruptIn m2_encoderB(PB_3);

    

    // attach callbacks to the interupts
    m1_encoderA.fall(fallCallback);
    m1_encoderB.fall(fallCallback);
    m2_encoderA.fall(fallCallback);
    m2_encoderB.fall(fallCallback);


    // ThisThread::sleep_for(500ms);
    pc.printf("Starting Main Loop!\n");
    while (true) {
        

        // ThisThread::sleep_for(3s);
        // // motor 1: on
        // // motor 2: off
        // pc.printf("Motor 1 On\n");
        // m1_throttle.write(1);
        // m2_throttle.write(0);

        // // // testing encoder pins
        // // m1_encoderA_out.write(1);
        // // m1_encoderB_out.write(1);
        // // m2_encoderA_out.write(0);
        // // m2_encoderB_out.write(0);

        // led.write(0); // on


        // ThisThread::sleep_for(3s);
        // // motor 1: off
        // // motor 2: on
        // pc.printf("Motor 2 On\n");
        // m1_throttle.write(0);
        // m2_throttle.write(1);

        // // // testing encoder pins
        // // m1_encoderA_out.write(0);
        // // m1_encoderB_out.write(0);
        // // m2_encoderA_out.write(1);
        // // m2_encoderB_out.write(1);

        // led.write(1); // off

        //===================================================================================

        ThisThread::sleep_for(10ms);

        // turn motor on
        led.write(0);
        m2_throttle.write(1);

        pc.printf("Encoder Count: %d\n", encoder_counter);
        if (encoder_counter >= 150 * 64 * 5) { // 5 revolutions (150:1 gear ratio, 64 CPR)
            led.write(1);
            pc.printf("Encoder Threshold Reached!\n");
            m2_throttle.write(0);
            break;
        } 


    }
}