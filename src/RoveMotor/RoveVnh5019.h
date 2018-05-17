#ifndef ROVEVNH5019_H_
#define ROVEVNH5019_H_

#include "wiring_private.h"

//////////////////////////////////////////////////////////////////

class RoveVnh5019
{
  public:

    void begin(uint8_t INA_PIN, uint8_t INB_PIN, uint8_t PWM_PIN);
    void drive(int speed); 
    void brake(int brake_speed);
    void coast();  
    
  private:    
    uint8_t _INA_PIN;
    uint8_t _INB_PIN;
    uint8_t _PWM_PIN;
};

#endif // ROVEVNH5019_H_