#include "mbed.h"

int main() {
    DigitalOut led(PC_13);
    DigitalOut led_ext(PB_0);
    PwmOut motorPower(PA_1);
    DigitalOut motorIn1(PA_6);
    DigitalOut motorIn2(PA_5);

    motorIn1.write(1);
    motorIn2.write(0);
    motorPower.write(0.5);

    bool m1 = true;
    bool m2 = false;

    float power = 0;
    while (1) {
        // ThisThread::sleep_for(500ms);
        led = !led;
        led_ext = !led_ext;
        ThisThread::sleep_for(2s);
        power += 0.25;
        if (power > 1) {power -= 1;}
        motorPower.write(power);
    }
}