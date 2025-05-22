#include "ThisThread.h"
#include "mbed.h"
#include "MotorCOTS.h"
#include "EUSBSerial.h"
#include "PID.h"
#include "Distributor.h"



void ctrl(float cmd, int seconds, MotorCOTS* motor1, MotorCOTS* motor2, Distributor* dstb, EUSBSerial* pc) {
    Timer t;
    t.start();
    std::pair<float, float> extensions;
    extensions = dstb->getMotorOutputsManual(cmd);
    while (t.read_ms() < 1000*seconds) {
        pc->printf("\tcmd: %f", cmd);
        motor1->toPosition(extensions.first, 10);
        motor2->toPosition(extensions.second, 10);
        pc->printf("\n");
        ThisThread::sleep_for(10ms);
    }
}

void ctrl_manual(float cmd1, float cmd2, int seconds, MotorCOTS* motor1, MotorCOTS* motor2, EUSBSerial* pc) {
    Timer t;
    t.start();
    while (t.read_ms() < 1000*seconds) {
        pc->printf("cmd: full open");
        motor1->toPosition(cmd1, 10);
        motor2->toPosition(cmd2, 10);
        pc->printf("\n");
        ThisThread::sleep_for(10ms);
    }
}



int main() {
    DigitalOut led(PC_13);
    led.write(0);
    PID pid(1, 0, 5);
    EUSBSerial pc;
    ThisThread::sleep_for(1s);
    pc.printf("\nSerial Port Connected!\n");

    // direction 1, direction 2, throttle, encoder a, encoder b
    MotorCOTS motor1(PB_8, PB_9, PA_1, PA_6, PA_7, &pid, &pc);
    MotorCOTS motor2(PA_10, PA_9, PA_8, PA_15, PB_3, &pid, &pc);
    float dt = 0.1;
    Distributor dstb;
    std::pair<float, float> extensions;


    // tools:
    // ctrl_manual(0, 0, 30, &motor1, &motor2, &pc); // cmd1, cmd2, duration (seconds)
    // while (true) {
    //     motor1.motorPower(-1);
    //     // motor2.motorPower(1);
    // }
    // ctrl_manual(1, 0, 300, &motor1, &motor2, &pc);


    // CONTROL SEQUENCE:
    // ctrl(cmd1, cmd2, ...)
    ctrl(0.5, 60, &motor1, &motor2, &dstb, &pc);
    ctrl(1, 120, &motor1, &motor2, &dstb, &pc);
    ctrl(0, 60, &motor1, &motor2, &dstb, &pc);
    ctrl(-0.5, 60, &motor1, &motor2, &dstb, &pc);
    ctrl(-1, 60, &motor1, &motor2, &dstb, &pc);
    ctrl(1, 999999999, &motor1, &motor2, &dstb, &pc);

}