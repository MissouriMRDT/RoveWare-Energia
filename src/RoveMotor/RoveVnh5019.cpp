#include "RoveVnh5019.h"

///////////////////////////////////////////////////////////////////////////////

void RoveVnh5019::begin(uint8_t INA_PIN, uint8_t INB_PIN, uint8_t PWM_PIN)
{ 
  _INA_PIN = INA_PIN;
  _INB_PIN = INB_PIN;
  _PWM_PIN = PWM_PIN;
  
  pinMode(PWM_PIN, OUTPUT);  
}

//////////////////////////////////////////////////////

void RoveVnh5019::drive(int motor_speed)
{ 
  if (motor_speed == 0) // coast motor for speed 0
  {
    PWMWrite(_PWM_PIN, 1, 0, 20000);
    digitalWrite(_INA_PIN, LOW);
    digitalWrite(_INB_PIN, LOW);  
       
  } else if (motor_speed > 0) 
  {  
    PWMWrite(_PWM_PIN, 1000, ((20*motor_speed) % 1000), 20000);
    digitalWrite(_INA_PIN, HIGH);
    digitalWrite(_INB_PIN, LOW); 
      
  } else if (motor_speed < 0)
  {
    PWMWrite(_PWM_PIN, 1, ((-20*motor_speed) % 1000), 20000);
    digitalWrite(_INA_PIN, LOW);
    digitalWrite(_INB_PIN, HIGH); 
  }          
}

///////////////////////////////////////////////////////////

void RoveVnh5019::coast()
{ 
  PWMWrite(_PWM_PIN, 1, 0, 20000);
  digitalWrite(_INA_PIN, LOW);
  digitalWrite(_INB_PIN, LOW);           
}

////////////////////////////////////////////////////////////

void RoveVnh5019::brake(int brake_speed)
{ 
  if (brake_speed == 0) // coast motor for brake speed
  {
    PWMWrite(_PWM_PIN, 1, 0, 20000);
    digitalWrite(_INA_PIN, LOW);
    digitalWrite(_INB_PIN, LOW);  
       
  } else if (brake_speed > 0) 
  {  
    PWMWrite(_PWM_PIN, 1, ((20*brake_speed) % 1000), 20000);
    digitalWrite(_INA_PIN, LOW);
    digitalWrite(_INB_PIN, LOW); 
      
  } else if (brake_speed < 0)
  {
    PWMWrite(_PWM_PIN, 1, ((-20*brake_speed) % 1000), 20000);
    digitalWrite(_INA_PIN, LOW);
    digitalWrite(_INB_PIN, LOW); 
  }          
}
