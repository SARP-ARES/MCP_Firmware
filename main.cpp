#include "mbed.h"

int main() {
    DigitalOut led(PC_13);
    DigitalOut led_ext(PB_0);
    DigitalOut motor1(PA_0);
    DigitalOut motor2(PA_1);

    bool m1 = true;
    bool m2 = false;

    while (1) {
        // ThisThread::sleep_for(500ms);
        led = !led;
        led_ext = !led_ext;
        motor1.write(0);
        motor2.write(0);
        ThisThread::sleep_for(50ms);
        motor1.write(m1);
        motor2.write(m2);
        m1 = !m1;
        m2 = !m2;
        ThisThread::sleep_for(2s);

    }
}