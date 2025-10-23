#include "PID.h"

// Initialize PID controller
// Takes:   float for proportional constant, float for integral constant
//          float derivative constant
PID::PID(float Kp, float Ki, float Kd) {
    positiveLast = true;
    errorLast = 0;
    integralError = 0;
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
}

// Compute PID output
// Takes:   float current angle, float target angle, float dt in milliseconds since last call
// Returns: float output power (-1 to 1)
float PID::compute(float currAngle, float targetAngle, float dt) {
    float error = targetAngle - currAngle;
    integralError += error*dt; // todo: solve integral windup later
    
    // Reset integralError if the error switches sign
    if ((positiveLast && error < 0) || (!positiveLast && error > 0)) {
        integralError = 0;
        positiveLast = !positiveLast;
    }


    float P = -Kp * error;
    float I = -Ki * integralError;
    float D = -Kd * (error - errorLast) / dt;
    errorLast = error;
    int output = P + I + D;
    if (output > 1) {
        return 1;
    } else if (output < -1) {
        return -1;
    }
    return output;
}