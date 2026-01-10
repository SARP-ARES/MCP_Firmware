#include "DigitalOut.h"
#include "ThisThread.h"
#include "mbed.h"
#include "MotorCOTS.h"
#include "EUSBSerial.h"
#include "PID.h"
#include "Distributor.h"

#define MCPS_ADDR 0x02 << 1
#define LOOP_PERIOD_MS 20

// Debug Led
DigitalOut led(PC_13);

// Serial
EUSBSerial pc;
    
// Object init
PID pid(1, 0, 5);

// New Driver board pinout
MotorCOTS motor1(PB_0, PA_7, PB_1, PC_14, PC_15, &pid, &pc); //motor A
MotorCOTS motor2(PA_6, PA_5, PA_1, PB_8, PB_9, &pid, &pc); //motor B
Distributor dstb;

// Defleciton cmds
std::pair<float, float> extensions;

Thread i2cThread; // setup thread

char i2c_tx_buf[32];
char i2c_rx_buf[32];

Mutex mutex;
float cmd_ctrl = 0;

struct {
    float leftDegrees;
    float rightDegrees;
    float leftPower;
    float rightPower;
} motorPacket;

void update_motorPacket(float leftDegrees, float rightDegrees, float leftPower, float rightPower) {
    ScopedLock<Mutex> lock(mutex);
    motorPacket.leftDegrees = leftDegrees;
    motorPacket.rightDegrees = rightDegrees;
    motorPacket.leftPower = leftPower;
    motorPacket.rightPower = rightPower;
}

//              SDA, SCL
I2CSlave slave(PB_7, PB_6);

void i2c_handler(void) {
    slave.address(MCPS_ADDR); 

    while(true) {
        int event = slave.receive();

        switch(event) {

            case I2CSlave::WriteAddressed: {

                int err = slave.read(i2c_rx_buf, sizeof(float));

                { 
                    ScopedLock<Mutex> lock(mutex);
                    memcpy(&cmd_ctrl, i2c_rx_buf, sizeof(float));
                }

                break;
            }

            case I2CSlave::ReadAddressed: {

                {
                    ScopedLock<Mutex> lock(mutex);
                    memcpy(i2c_tx_buf, &motorPacket, sizeof(motorPacket));
                }

                slave.write(i2c_tx_buf, sizeof(motorPacket));
                break;
            }

            default: break;
        }
    }
}

/* -------- DEBUG --------- */

// Steers ARES in a set turn angle for a set ammount of time
// Takes:   float for steering (-1 full left 1 full right), an int of seconds to hold for, two motor pointer 
//          for the left and right motors, a distributor pointer, and a USB serial pointer
void ctrl(float cmd, int seconds, MotorCOTS* motor1, MotorCOTS* motor2, Distributor* dstb, EUSBSerial* pc) {
    Timer t;
    t.start();
    std::pair<float, float> extensions;
    extensions = dstb->getMotorOutputs(cmd);
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

void led_if_deflection_pos(float ctrl) {
    if (ctrl > 0) led.write(1);
    else          led.write(0);
}

/* ------------------------- */

int main() {

    Timer t;
    i2cThread.start(i2c_handler);

    while (true) {
        t.start();
        
        float ctrl;

        {   // Safely grab commanded ctrl 
            ScopedLock<Mutex> lock(mutex);
            ctrl = cmd_ctrl;
        }
        
        // RUN COMMANDS
        /* ext.first -> left : ext.second -> right */
        extensions = dstb.getMotorOutputs(ctrl);
        float lpower = motor1.toPosition(extensions.first, 10);  // Left cmd
        float rpower = motor2.toPosition(extensions.second, 10); // Right cmd

        led_if_deflection_pos(ctrl);

        // UPDATE FLASH DATA
        update_motorPacket(
            motor1.getDegrees(), // Left Motor
            motor2.getDegrees(), // Right Motor
            lpower,         
            rpower
        );

        // Event Scheduling
        if(t.read_ms() < LOOP_PERIOD_MS) ThisThread::sleep_for(LOOP_PERIOD_MS - t.read_ms());
    }

}