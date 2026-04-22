#include "mbed.h"

// LED PINS
const PinName LED_PIN = PC_13;

// MOTOR CONTROLLER PINS
const PinName DIRECTION_1_M1 = PA_6; 
const PinName DIRECTION_2_M1 = PA_5;



// MotorCOTS::MotorCOTS(PinName directionOne, PinName directionTwo, PinName powerThrottle, PinName PINA, PinName PINB, PID* pid, EUSBSerial* pc)