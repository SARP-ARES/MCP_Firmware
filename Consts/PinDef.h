#include "mbed.h"

// LED PINS
const PinName LED_PIN = PC_13;

// -- COTS MOTOR CONTROLLER PINS -- 

// Motor 1
const PinName COTS_DIRECTION_1_M1 = PA_6; 
const PinName COTS_DIRECTION_2_M1 = PA_5;
const PinName COTS_POWER_THROTTLE_M1 = PA_1;
const PinName COTS_PINA_M1 = PB_8;
const PinName COTS_PINB_M1 = PB_9;

// Motor 2
const PinName COTS_DIRECTION_1_M2 = PB_0; 
const PinName COTS_DIRECTION_2_M2 = PA_7;
const PinName COTS_POWER_THROTTLE_M2 = PB_1;
const PinName COTS_PINA_M2 = PC_14;
const PinName COTS_PINB_M2 = PC_15;

// Motor motor1(PB_3, PB_5, PA_11, PA_12, PA_10, PA_9, pid); // these are the mcpcb
// Motor motor2(PA_6, PA_5, PB_14, PB_15, PB_13, PA_8, pid);

// -- Custom Board Motor Pins -- 

// Motor 1
const PinName PIN_A_M1 = PB_3; 
const PinName PIN_B_M1 = PB_5;
const PinName MOTOR_1_M1 = PA_5;
const PinName MOTOR_2_M1 = PA_7;
const PinName MOTOR_3_M1 = PB_10;
const PinName MOTOR_4_M1 = PA_9;

// Motor 2
const PinName PIN_A_M2 = PA_6; 
const PinName PIN_B_M2 = PA_1;
const PinName MOTOR_1_M2 = PB_14;
const PinName MOTOR_2_M2 = PB_15;
const PinName MOTOR_3_M2 = PB_13;
const PinName MOTOR_4_M2 = PA_8;

// -- I2C pins --
const PinName SDA_PIN = PB_7; 
const PinName SCL_PIN = PB_8;
