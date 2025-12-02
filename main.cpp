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
float f;

struct {
    float leftDegrees;
    float rightDegrees;
    float leftPower;
    float rightPower;
} motorPacket;

int receivedWrite1 = 0;
int receivedWrite_not1 = 0;
int sentReadReqs = 0;

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
    slave.address(MCPS_ADDR); // Expects shifted into correct position

    while(true) {
        int event = slave.receive();

        switch(event) {

            case I2CSlave::WriteAddressed: {
                int err = slave.read(i2c_rx_buf, sizeof(float));
                { 
                    ScopedLock<Mutex> lock();
                    memcpy(&f, i2c_rx_buf, sizeof(float));
                }
                if (f == 1) {
                    receivedWrite1++;
                } else {
                    receivedWrite_not1++;
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

            default:
                // No event; just continue
                break;
        }
    }
}


// main() runs in its own thread in the OS
int main()
{


    // PID pid(0.017, 0, 1); // No idea if these values work
    // Motor motor(PA_8, PA_10, PB_2, PB_1, PB_15, PB_14, pid); // test bench
    // Motor motor1(PB_3, PB_5, PA_11, PA_12, PA_10, PA_9, pid); // these are the mcpcb
    // Motor motor2(PA_6, PA_5, PB_14, PB_15, PB_13, PA_8, pid);
    
    // // Distributor distributor(&ser);

    i2cThread.start(i2c_handler);


        //  DEBUG  //
    // printf("start :)\n");
    // DigitalOut led1(PC_14);

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
        ThisThread::sleep_for(500ms);
        led.write(1);

        update_struct(1.0, 2.0, 3.0, 4.0);
        // printf("Sleeping");
        // ThisThread::sleep_for(120s);
        // printf("Successfully recieved writes:\t%d", receivedWrite1);
        // printf("Unsuccessfully recieved writes:\t%d", receivedWrite_not1);
        // printf("Sent read requests:\t%d", sentReadReqs);
        
        ThisThread::sleep_for(500ms);
        led.write(0);
    }
}

