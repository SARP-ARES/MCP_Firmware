#include "DigitalOut.h"
#include "ThisThread.h"
#include "mbed.h"
#include "MotorCOTS.h"
#include "EUSBSerial.h"
#include "PID.h"
#include "Distributor.h"


// Steers ARES in a set turn angle for a set ammount of time
// Takes:   float for steering (-1 full left 1 full right), an int of seconds to hold for, two motor pointer 
//          for the left and right motors, a distributor pointer, and a USB serial pointer
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

// Pulls each control line set ammounts for a set ammount of time
// Takes:   float for left motor retraction, float for right motor retraction, an int of seconds to hold for,
//          two motor pointer for the left and right motors, a distributor pointer, and a USB serial pointer
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
    // Debug LED
    DigitalOut led(PC_13);
    
    // USB serial init
    EUSBSerial pc;
    ThisThread::sleep_for(1s);
    pc.printf("\nSerial Port Connected!\n");
    led.write(1);


    // Object init
    PID pid(1, 0, 5);
    // Direction 1, direction 2, throttle, encoder a, encoder b
    // MotorCOTS motor1(PB_8, PB_9, PA_1, PA_6, PA_7, &pid, &pc);
    // MotorCOTS motor2(PA_10, PA_9, PA_8, PA_15, PB_3, &pid, &pc);
    // New Driver board pinout
    MotorCOTS motor1(PB_0, PA_7, PB_1, PC_14, PC_15, &pid, &pc); //motor A
    MotorCOTS motor2(PA_6, PA_5, PA_1, PB_8, PB_9, &pid, &pc); //motor B
    Distributor dstb;
    std::pair<float, float> extensions;



    // // Control Trigger
    // DigitalIn ctrl_trigger(PB_7); // SDA1 (pulled high by default, trigger is low)
    // while(ctrl_trigger.read() == 1) {
    //     ThisThread::sleep_for(10ms);
    // }
    // pc.printf("\nTHE THING WAS TRIGGERED...\n STARTING CONTROL SEQUENCE...");
    // led.write(0);




    // CONTROL SEQUENCE:
    // ctrl(cmd1, time (seconds), ...)
    // cmd = -1 is full left, 1 is full right
    ctrl(0.25, 60, &motor1, &motor2, &dstb, &pc);
    ctrl(0.5, 60, &motor1, &motor2, &dstb, &pc);
    ctrl(0.75, 60, &motor1, &motor2, &dstb, &pc);
    ctrl(1, 120, &motor1, &motor2, &dstb, &pc);
    ctrl(0, 60, &motor1, &motor2, &dstb, &pc);
    ctrl(-0.5, 60, &motor1, &motor2, &dstb, &pc);
    ctrl(-1, 60, &motor1, &motor2, &dstb, &pc);
    while (true) {
        ctrl(1, 3600, &motor1, &motor2, &dstb, &pc);
    }

}