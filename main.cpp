#include "DigitalOut.h"
#include "ThisThread.h"
#include "mbed.h"
#include "MotorCOTS.h"
#include "EUSBSerial.h"
#include "PID.h"
#include "Distributor.h"
#include <atomic>

#define MCPS_ADDR 0x02 << 1
#define LOOP_PERIOD_MS 20

// Debug Led
DigitalOut led(PC_13);

// Serial
EUSBSerial pc;
    
// Initializing the PID controller for both motors
PID pid(0.1, 0, 5, 0.05);

MotorCOTS motor1(PA_6, PA_5, PA_1, PB_8, PB_9, &pid, &pc); // Motor B
MotorCOTS motor2(PB_0, PA_7, PB_1, PC_14, PC_15, &pid, &pc); // Motor A

Distributor dstb;


const float DEFAULT_CTRL_VALUE = 999.0f;

// Relative extensions for both motors
// 0 = retracted, 1 = fully exteded
std::pair<float, float> extensions;

Thread i2cThread;

// Buffer for I2C transmission
// 32 bytes for 4 floats (motor positions and powers)
char i2c_tx_buf[32];

Mutex mutex;
std::atomic<float> cmd_ctrl{DEFAULT_CTRL_VALUE};

struct {
    float leftDegrees;
    float rightDegrees;
    float leftPower;
    float rightPower;
} motorPacket;


bool is_nan_safe(float f) {
    uint32_t i;
    memcpy(&i, &f, sizeof(i));
    return (i & 0x7F800000) == 0x7F800000 && (i & 0x007FFFFF) != 0;
}


void update_motorPacket(float leftDegrees, float rightDegrees, float leftPower, float rightPower) {
    ScopedLock<Mutex> lock(mutex);
    motorPacket.leftDegrees = leftDegrees;
    motorPacket.rightDegrees = rightDegrees;
    motorPacket.leftPower = leftPower;
    motorPacket.rightPower = rightPower;
}

//              SDA, SCL
I2CSlave slave(PB_7, PB_6);

// I2C handler thread takes motor control inputs and upon request sends motor status packet
void i2c_handler(void) {
    slave.address(MCPS_ADDR); 

    while(true) {
        int event = slave.receive();

        switch(event) {

            case I2CSlave::WriteAddressed: {
                
                char temp[sizeof(float)];
                if(slave.read(temp, sizeof(float)) == 0) {
                    float val;
                    memcpy(&val, temp, sizeof(float));
                    cmd_ctrl.store(val); // Atomic non-blocking store
                }
                break;
            }

            case I2CSlave::ReadAddressed: {
                memcpy(i2c_tx_buf, &motorPacket, sizeof(motorPacket));
                slave.write(i2c_tx_buf, sizeof(motorPacket));
                break;
            }

            default: break;
        }
    }
}


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

// LED debug indicator method
void led_if_deflection_pos(float ctrl) {
    if (ctrl > 0) led.write(1);
    else          led.write(0);
}


/* ------------------------- */

int main() {

    Timer t;
    i2cThread.start(i2c_handler);

    while (true) {
        t.reset();
        t.start();
        
        // COMMS
        float ctrl = cmd_ctrl.load();

        // RUN COMMANDS
        /* ext.first -> left : ext.second -> right */
        // check for the default value so that motors don't spin until a command is received from the flight computer
        if (ctrl != DEFAULT_CTRL_VALUE) {
            extensions = dstb.getMotorOutputs(ctrl);
            float lpower = motor1.toPosition(extensions.first, 10);  // Left cmd
            float rpower = motor2.toPosition(extensions.second, 10); // Right cmd
        }

        led_if_deflection_pos(ctrl);

        // UPDATE FLASH DATA
        update_motorPacket(
            motor1.getPosition(), // Left Motor
            motor2.getPosition(), // Right Motor
            extensions.first,         
            extensions.second
        );

        // Event Scheduling
        if(t.read_ms() < LOOP_PERIOD_MS) ThisThread::sleep_for(LOOP_PERIOD_MS - t.read_ms());
    }   

}