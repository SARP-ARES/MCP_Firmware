#include "mbed.h"
#include "MotorCOTS.h"

int main() {
    DigitalOut led(PC_13);
    DigitalOut led_ext(PB_0);

    // direction 1, direction 2, throttle, encoder a, b
    MotorCOTS motor(PB_8, PB_9, PA_1, PA_8, PA_9);

    bool m1 = true;
    bool m2 = false;

    float power = 0;
    while (1) {
        // ThisThread::sleep_for(500ms);
        led = !led;
        led_ext = !led_ext;
        ThisThread::sleep_for(5s);
        power += 0.25;
        if (power > 1) {power -= 1;}
        motor.motorPower(power);
    }
}