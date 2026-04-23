#include "DigitalOut.h"
#include "ThisThread.h"
#include "mbed.h"
#include "Motor.h"
#include "EUSBSerial.h"
#include "PID.h"
#include "Distributor.h"
#include <atomic>
#include "./Consts/PinDef.h"
#include "./Consts/ControlVals.h"

// Debug Led
DigitalOut led(LED_PIN);

// Serial
EUSBSerial pc;

const float Kp  = 1.2;
const float Ki  = 0;
const float Kd  = 0.1;
const float DEADZONE  = 0.05;

// Initializing the PID controller for both motors
PID pid(Kp, Ki, Kd, DEADZONE);

Motor m1(
    PIN_A_M1, PIN_B_M1, 
    MOTOR_1_M1, MOTOR_2_M1, MOTOR_3_M1, MOTOR_4_M1, 
    &pid
);

Motor m2(
    PIN_A_M2, PIN_B_M2,
    MOTOR_1_M2, MOTOR_2_M2, MOTOR_3_M2, MOTOR_4_M2,
    &pid
);

Distributor dstb;

const float DEFAULT_CTRL_VALUE = 999.0f;

// Relative extensions for both motors
// 0 = retracted, 1 = fully exteded
std::pair<float, float> extensions;

Thread i2cThread;

// Buffer for I2C transmission
// 32 bytes for 4 floats (motor positions and powers)
char i2c_tx_buf[32];

std::atomic<float> cmd_ctrl{DEFAULT_CTRL_VALUE};

struct MotorPacket {
    float leftDegrees;
    float rightDegrees;
    float leftPower;
    float rightPower;
};

// Double Buffering 
struct MotorPacket packet_1, packet_2;
std::atomic<MotorPacket*> read_from_ptr{&packet_1};
MotorPacket* write_to_ptr = &packet_2;

bool is_nan_safe(float f) {
    uint32_t i;
    memcpy(&i, &f, sizeof(i));
    return (i & 0x7F800000) == 0x7F800000 && (i & 0x007FFFFF) != 0;
}

/** @brief Updates the current motor packet with new data */
void update_motorPacket(MotorPacket* motorPtr, float leftDegrees, float rightDegrees, float leftPower, float rightPower) {
    motorPtr->leftDegrees = leftDegrees;
    motorPtr->rightDegrees = rightDegrees;
    motorPtr->leftPower = leftPower;
    motorPtr->rightPower = rightPower;
}

// Enables slave node functionality over i2c
I2CSlave slave(SDA_PIN, SCL_PIN);

/** @brief I2C interrupt handler, updates commanded deflection on write request, sends motor data on read request */
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
                
                // Pull fresh data from DB 
                MotorPacket* pkt = read_from_ptr.load();
                memcpy(i2c_tx_buf, pkt, sizeof(MotorPacket));

                slave.write(i2c_tx_buf, sizeof(MotorPacket));
                break;
            }

            default: break;
        }
    }
}


/** @brief Lights up hte onboard led for a deflection > 0 */
void led_if_deflection_pos(float ctrl) {
    if (ctrl > 0) led.write(1);
    else          led.write(0);
}


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
            float lpower = m1.toPosition(extensions.first, 10);  // Left cmd
            float rpower = m2.toPosition(extensions.second, 10); // Right cmd
        }

        led_if_deflection_pos(ctrl);

        // UPDATE FLASH DATA
        update_motorPacket(
            write_to_ptr,
            m1.getPosition(), // Left Motor
            m2.getPosition(), // Right Motor
            extensions.first,         
            extensions.second
        );

        // Update the double buffer
        MotorPacket* old_read = read_from_ptr.exchange(write_to_ptr); // Swap the Pointers
        write_to_ptr = old_read; // Set write to be the old ptr 

        // Event Scheduling
        if(t.read_ms() < LOOP_PERIOD_MS) ThisThread::sleep_for(LOOP_PERIOD_MS - t.read_ms());
    }   

}