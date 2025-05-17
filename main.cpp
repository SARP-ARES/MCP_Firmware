#include "mbed.h"
#include "MotorCOTS.h"
#include "EUSBSerial.h"
#include "PID.h"


int main() {
    DigitalOut led(PB_0);
    led.write(1);
    PID pid(0.5, 0, 10);
    EUSBSerial pc;
    ThisThread::sleep_for(1s);
    pc.printf("\nSerial Port Connected!\n");

    // direction 1, direction 2, throttle, encoder a, encoder b

    // // ollie motor
    MotorCOTS motor(PB_8, PB_9, PA_1, PA_6, PA_7, &pid, &pc);
    led.write(0);
    // jimmy motor:
    // MotorCOTS motor(PA_2, PA_3, PB_1, PA_6, PA_7);

    float dt = 0.1;
    Timer t;
    t.start();


    float degrees = 0;
    float current_angle = 0;
    float target_position = 0.5; // degrees
    pc.printf("Target Angle: %f\n\n", target_position);

    // float power = 0;
    while (true) {
        ThisThread::sleep_for(10ms);

        motor.toPosition(target_position, 10);
        
        pc.printf("Current Pos: %f\t", motor.getPosition());

        // // NOT USING PWM SO JUST TURN TF OFF
        // if (power < 0) {
        //     motor.motorPower(0);
        //     break;
        // }


        // motor.direction(1);
        // led_ext.write(1);
        // for (int i=0; i<50; i++) { // 50 x 100ms = 5s
        //     degrees = motor.getDegrees();
        //     pc.printf("Direction: 1\n");
        //     pc.printf("Degrees: %f\n\n", degrees);
        //     ThisThread::sleep_for(100ms);
        // }

        // motor.direction(-1);
        // led_ext.write(0);
        // for (int i=0; i<50; i++) {
        //     degrees = motor.getDegrees();
        //     pc.printf("Direction: 2\n");
        //     pc.printf("Degrees: %f\n\n", degrees);
        //     ThisThread::sleep_for(100ms);
        // }
        


    }
}