#include "mbed.h"
#include <string>
#include "I2CSerial.h"

#include "Motor.h"
#include "PID.h"
#include "Distributor.h"
#include "EUSBSerial.h"


DigitalOut led(PC_13);
EUSBSerial pc;

int leftExtension = 0;
int rightExtension = 0;

Thread i2cThread; // setup thread

char i2c_tx_buf[32];
char i2c_rx_buf[32];

#define MCPS_ADDR 0x02 << 1

Mutex mutex;
float cmd_ctrl = 0;

struct {
    float leftDegrees;
    float rightDegrees;
    float leftPower;
    float rightPower;
} motorPacket;

void update_struct(float leftDegrees, float rightDegrees, float leftPower, float rightPower) {
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
                    ScopedLock<Mutex> lock();
                    memcpy(&cmd_ctrl, i2c_rx_buf, sizeof(float));
                }

                break;
            }

            case I2CSlave::ReadAddressed: {

                {
                    ScopedLock<Mutex> lock();
                    memcpy(i2c_tx_buf, &motorPacket, sizeof(motorPacket));
                }

                slave.write(i2c_tx_buf, sizeof(motorPacket));

                break;
            }

            default: break;
        }
    }
}

int main()
{

    // PID pid(0.017, 0, 1); // No idea if these values work
    // Motor motor(PA_8, PA_10, PB_2, PB_1, PB_15, PB_14, pid); // test bench
    // Motor motor1(PB_3, PB_5, PA_11, PA_12, PA_10, PA_9, pid); // these are the mcpcb
    // Motor motor2(PA_6, PA_5, PB_14, PB_15, PB_13, PA_8, pid);
    
    Distributor distributor;

    i2cThread.start(i2c_handler);

    // while (true) {
    //     ThisThread::sleep_for(10ms);
    //     // get distributed pair
    //     mutex.lock();
    //     float ctrl = f;
    //     mutex.unlock();
    //     std::pair<float, float> spoolExtensions = distributor.getMotorOutputs(ctrl);
        
    //     // if the first float is a NAN, keep each extension value the same
    //     leftExtension = (isnan(spoolExtensions.first)) ? spoolExtensions.first : leftExtension;
    //     rightExtension = (isnan(spoolExtensions.first)) ? spoolExtensions.second : rightExtension;

    //     // go to position
    //     float power1 = motor1.lineTo(leftExtension, 10);
    //     float power2 = motor2.lineTo(rightExtension, 10);

    //     update_struct(motor1.getDegrees(), motor2.getDegrees(), power1, power2);
    // }

    while (true) {

        float ctrl; 

        { 
            ScopedLock<Mutex> lock();
            ctrl = cmd_ctrl; 
        }

        std::pair<float, float> spoolExtensions = distributor.getMotorOutputs(ctrl);
        float extension = spoolExtensions.first;

        if (extension > 0.5)    led.write(1);
        else                    led.write(0);
        
        update_struct(1.0, 2.0, 3.0, 4.0);
        
        ThisThread::sleep_for(100ms);

    }
}

