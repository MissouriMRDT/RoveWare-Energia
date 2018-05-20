#ifndef ROVEVNH5019_H_
#define ROVEVNH5019_H_

#include "wiring_private.h"

//////////////////////////////////////////////////////////////////

const int VNH5019_MAX_FORWARD =  1000;
const int VNH5019_MAX_REVERSE = -1000;

class RoveVnh5019
{
  public:

    void begin(uint8_t INA_PIN, uint8_t INB_PIN, uint8_t PWM_PIN);
    void drive(int motor_speed); 
    void brake(int brake_speed);
    void coast();  
    
  private:    
    uint8_t _INA_PIN;
    uint8_t _INB_PIN;
    uint8_t _PWM_PIN;
};

void rovePwmWrite(uint8_t pin, uint32_t period_micros, uint32_t pulse_width_micros);

#endif // ROVEVNH5019_H_