#include "mbed.h"
#include "MotorCOTS.h"
#include "EUSBSerial.h"
#include "PID.h"
#include "Distributor.h"


int main() {
    DigitalOut led(PB_0);
    led.write(1);
    PID pid(1, 0, 5);
    EUSBSerial pc;
    ThisThread::sleep_for(1s);
    pc.printf("\nSerial Port Connected!\n");

    // direction 1, direction 2, throttle, encoder a, encoder b

    // // ollie motor
    MotorCOTS motor1(PB_8, PB_9, PA_1, PA_6, PA_7, &pid, &pc);
    MotorCOTS motor2(PA_10, PA_9, PA_8, PA_15, PB_3, &pid, &pc);
    led.write(0);
    // jimmy motor:
    // MotorCOTS motor(PA_2, PA_3, PB_1, PA_6, PA_7);

    float dt = 0.1;
    Timer t;
    t.start();

    Distributor dstb;


    // not sure if we need this
    float degrees = 0;
    float current_angle = 0;
    float target_position = 0.1; // degrees
    pc.printf("Target Angle: %f\n\n", target_position);


    std::pair<int, int> extensions;
    int seconds = 30;


    //      CONTROL SEQUENCE
    
    for (int i = 0; i < seconds*100; i++) {
        motor1.toPosition(0, 10);
        pc.printf("Current Pos 1: %f\t", motor1.getPosition());
        motor2.toPosition(0, 10);
        pc.printf("Current Pos 2: %f\t", motor2.getPosition());
    }


    extensions = dstb.getMotorOutputsManual(1);
    for (int i = 0; i < seconds*100; i++) {
        motor1.toPosition(extensions.first, 10);
        pc.printf("Current Pos 1: %f\t", motor1.getPosition());
        motor2.toPosition(extensions.second, 10);
        pc.printf("Current Pos 2: %f\t", motor2.getPosition());
    }

    extensions = dstb.getMotorOutputsManual(0.5);
    for (int i = 0; i < seconds*100; i++) {
        motor1.toPosition(extensions.first, 10);
        pc.printf("Current Pos 1: %f\t", motor1.getPosition());
        motor2.toPosition(extensions.second, 10);
        pc.printf("Current Pos 2: %f\t", motor2.getPosition());
    }

    extensions = dstb.getMotorOutputsManual(0);
    for (int i = 0; i < seconds*100; i++) {
        motor1.toPosition(extensions.first, 10);
        pc.printf("Current Pos 1: %f\t", motor1.getPosition());
        motor2.toPosition(extensions.second, 10);
        pc.printf("Current Pos 2: %f\t", motor2.getPosition());
    }

    extensions = dstb.getMotorOutputsManual(-0.5);
    for (int i = 0; i < seconds*100; i++) {
        motor1.toPosition(extensions.first, 10);
        pc.printf("Current Pos 1: %f\t", motor1.getPosition());
        motor2.toPosition(extensions.second, 10);
        pc.printf("Current Pos 2: %f\t", motor2.getPosition());
    }

    extensions = dstb.getMotorOutputsManual(-1);
    for (int i = 0; i < seconds*100; i++) {
        motor1.toPosition(extensions.first, 10);
        pc.printf("Current Pos 1: %f\t", motor1.getPosition());
        motor2.toPosition(extensions.second, 10);
        pc.printf("Current Pos 2: %f\t", motor2.getPosition());
    }

    extensions = dstb.getMotorOutputsManual(1);
    for (int i = 0; i < seconds*100; i++) {
        motor1.toPosition(extensions.first, 10);
        pc.printf("Current Pos 1: %f\t", motor1.getPosition());
        motor2.toPosition(extensions.second, 10);
        pc.printf("Current Pos 2: %f\t", motor2.getPosition());
    }


    while (true) {
        ThisThread::sleep_for(10ms);
    }
}