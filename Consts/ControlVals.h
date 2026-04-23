// Control Algorithm Const values 

const unsigned int MCPS_ADDR = 0x02 << 1; 
const unsigned int LOOP_PERIOD_MS = 20; 

const float Kp  = 1.2;
const float Ki  = 0;
const float Kd  = 0.1;
const float DEADZONE  = 0.05;

const float DEFAULT_CTRL_VALUE = 999.0f;