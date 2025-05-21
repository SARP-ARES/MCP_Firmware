#include "ThisThread.h"
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

    Distributor dstb;


    // // not sure if we need this
    // float degrees = 0;
    // float current_angle = 0;
    // float target_position = 0.1; // degrees
    // pc.printf("Target Angle: %f\n\n", target_position);


    std::pair<float, float> extensions;
    int seconds = 30;


    //      CONTROL SEQUENCE
    
    Timer t;
    t.start();

    while (t.read_ms() < 1000*seconds) {
        led = !led;
        pc.printf("cmd: 0");
        motor1.toPosition(0, 10);
        motor2.toPosition(0, 10);
        pc.printf("Loop 1\n");
        ThisThread::sleep_for(10ms);
    }

    t.reset();
    extensions = dstb.getMotorOutputsManual(1);
    while (t.read_ms() < 1000*seconds) {
        led = !led;
        pc.printf("\tcmd: 1");
        motor1.toPosition(extensions.first, 10);
        motor2.toPosition(extensions.second, 10);
        pc.printf("\tLoop 2\n");
        ThisThread::sleep_for(10ms);
    }

    extensions = dstb.getMotorOutputsManual(0.5);
    t.reset();
    while (t.read_ms() < 1000*seconds) {
        led = !led;
        pc.printf("State: 0.5\t");
        motor1.toPosition(extensions.first, 10);
        motor2.toPosition(extensions.second, 10);
        pc.printf("\tLoop 3\n");
        ThisThread::sleep_for(10ms);
    }

    extensions = dstb.getMotorOutputsManual(0);
    t.reset();
    while (t.read_ms() < 1000*seconds) {
        led = !led;
        pc.printf("State: 0\t");
        motor1.toPosition(extensions.first, 10);
        motor2.toPosition(extensions.second, 10);
        ThisThread::sleep_for(10ms);
        pc.printf("\tLoop 4\n");
    }

    extensions = dstb.getMotorOutputsManual(-0.5);
    t.reset();
    while (t.read_ms() < 1000*seconds) {
        led = !led;
        pc.printf("State: -0.5\t");
        motor1.toPosition(extensions.first, 10);
        motor2.toPosition(extensions.second, 10);
        pc.printf("\tLoop 5\n");
        ThisThread::sleep_for(10ms);
    }

    extensions = dstb.getMotorOutputsManual(-1);
    t.reset();
    while (t.read_ms() < 1000*seconds) {
        led = !led;
        pc.printf("State: -1\t");
        motor1.toPosition(extensions.first, 10);
        motor2.toPosition(extensions.second, 10);
        pc.printf("\tLoop 6\n");
        ThisThread::sleep_for(10ms);
    }

    extensions = dstb.getMotorOutputsManual(1);
    while (t.read_ms() < 1000*seconds) {
        led = !led;
        pc.printf("State: 1\t");
        motor1.toPosition(extensions.first, 10);
        motor2.toPosition(extensions.second, 10);
        pc.printf("\tLoop 7\n");
        ThisThread::sleep_for(10ms);
    }

    while (true) {
        led = !led;
        pc.printf("State: full open");
        motor1.toPosition(0, 10);
        motor2.toPosition(0, 10);
        pc.printf("\tLoop 8 (END)\n");
        ThisThread::sleep_for(10ms);
    }

    // ===================================

    // while (t.read_ms() < 1000*seconds) {
    //     led = !led;
    //     pc.printf("State: full open");
    //     motor1.toPosition(1, 10);
    //     pc.printf("Loop 1\n");
    //     ThisThread::sleep_for(10ms);
    // }


    // while (t.read_ms() < 1000*seconds) {
    //     led = !led;
    //     pc.printf("State: full open");
    //     motor1.toPosition(0.5f, 10);
    //     pc.printf("Loop 1\n");
    //     ThisThread::sleep_for(10ms);
    // }


    // while (t.read_ms() < 1000*seconds) {
    //     led = !led;
    //     pc.printf("State: full open");
    //     motor1.toPosition(0.2, 10);
    //     pc.printf("Loop 1\n");
    //     ThisThread::sleep_for(10ms);
    // }


    // while (t.read_ms() < 1000*seconds) {
    //     led = !led;
    //     pc.printf("State: full open");
    //     motor1.toPosition(0, 10);
    //     pc.printf("Loop 1\n");
    //     ThisThread::sleep_for(10ms);
    // }

    // while (t.read_ms() < 1000*seconds) {
    //     led = !led;
    //     pc.printf("State: full open");
    //     motor1.toPosition(1, 10);
    //     pc.printf("Loop 1\n");
    //     ThisThread::sleep_for(10ms);
    // }

    // while (true) {
    //     led = !led;
    //     pc.printf("State: full open");
    //     motor1.toPosition(0, 10);
    //     pc.printf("Loop 1\n");
    //     ThisThread::sleep_for(10ms);
    // }

}