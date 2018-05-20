#include "RoveVnh5019.h"

#include "inc/hw_memmap.h"

#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const uint32_t TICKS_PER_MICRO = (120000000 / 1000000); // 120
static const uint32_t PERIOD_MICROS   =  2040;                 // todo => 490 hz only because of analogRead spec?

void RoveVnh5019::begin(uint8_t INA_PIN, uint8_t INB_PIN, uint8_t PWM_PIN)
{ 
  _INA_PIN = INA_PIN;
  _INB_PIN = INB_PIN;
  _PWM_PIN = PWM_PIN;
  
  pinMode(_INA_PIN, OUTPUT);  
  pinMode(_INB_PIN, OUTPUT);    
}

//////////////////////////////////////////////////////

void RoveVnh5019::drive(int motor_speed)
{ 
  uint32_t pulse_width_micros = map(motor_speed, VNH5019_MAX_REVERSE, VNH5019_MAX_FORWARD, 0, PERIOD_MICROS); 
  
  rovePwmWrite(_PWM_PIN, PERIOD_MICROS, pulse_width_micros);

  if (motor_speed == 0) // coast motor for speed 0
  {
    digitalWrite(_INA_PIN, LOW);
    digitalWrite(_INB_PIN, LOW);  
       
  } else if (motor_speed > 0) 
  {  
    digitalWrite(_INA_PIN, HIGH);
    digitalWrite(_INB_PIN, LOW); 
      
  } else if (motor_speed < 0)
  {
    digitalWrite(_INA_PIN, LOW);
    digitalWrite(_INB_PIN, HIGH); 
  }          
}

////////////////////////////////////////////////////////////

void RoveVnh5019::brake(int brake_speed)
{ 
  uint32_t pulse_width_micros = map(brake_speed, VNH5019_MAX_REVERSE, VNH5019_MAX_FORWARD, 0, PERIOD_MICROS); 
  
  rovePwmWrite(_PWM_PIN, PERIOD_MICROS, pulse_width_micros);
  digitalWrite(_INA_PIN, HIGH);
  digitalWrite(_INB_PIN, HIGH);           
}

///////////////////////////////////////////////////////////

void RoveVnh5019::coast()
{ 
  rovePwmWrite(_PWM_PIN, PERIOD_MICROS, 0);
  digitalWrite(_INA_PIN, LOW);
  digitalWrite(_INB_PIN, LOW);           
}



////////////////////////////////////////////////////////////////////////////////////////////////////////
// todo for 123G  PWMClockSet    => SysCtlPWMClockSet        
// todo for 123G PWMGenPeriodSet => 80000000 / period_micros

static bool pwm_periph_begin = false; 
  
void rovePwmWrite(uint8_t pin, uint32_t period_micros, uint32_t pulse_width_micros)
{   
  if (pulse_width_micros == 0) 
  {
    pinMode(     pin, OUTPUT);
    digitalWrite(pin, LOW);
    return;
  }
    
  if (pulse_width_micros >= period_micros) 
  {
    pinMode(     pin, OUTPUT);
    digitalWrite(pin, HIGH);
    return;
  }
  
  if (digitalPinToTimer(pin)) 
  {   
    PWMWrite(pin, period_micros, pulse_width_micros, (1000000 / period_micros));
    return;
  }
 
  uint32_t pwm_generator; 
  uint32_t pwm_pin_mux; 
  uint32_t pwm_pin_out; 
  uint32_t pwm_pin_out_bit; 
   
  switch(pin)
  {
    case PG_0:

      pwm_generator    = PWM_GEN_2; 
      pwm_pin_mux      = GPIO_PG0_M0PWM4;
      pwm_pin_out      = PWM_OUT_4;
      pwm_pin_out_bit  = PWM_OUT_4_BIT;
      break;
    
    case PF_3:

      pwm_generator    = PWM_GEN_1; 
      pwm_pin_mux      = GPIO_PF3_M0PWM3;
      pwm_pin_out      = PWM_OUT_3;
      pwm_pin_out_bit  = PWM_OUT_3_BIT;
      break;
      
    case PF_2:

      pwm_generator    = PWM_GEN_1; 
      pwm_pin_mux      = GPIO_PF2_M0PWM2;
      pwm_pin_out      = PWM_OUT_2;
      pwm_pin_out_bit  = PWM_OUT_2_BIT;
      break;
      
    case PF_1:

      pwm_generator    = PWM_GEN_0; 
      pwm_pin_mux      = GPIO_PF1_M0PWM1; 
      pwm_pin_out      = PWM_OUT_1;
      pwm_pin_out_bit  = PWM_OUT_1_BIT;
      break;
      
    case PK_5:
    
      pwm_generator    = PWM_GEN_3; 
      pwm_pin_mux      = GPIO_PK5_M0PWM7; 
      pwm_pin_out      = PWM_OUT_7;
      pwm_pin_out_bit  = PWM_OUT_7_BIT;
      break;

    case PK_4:
    
      pwm_generator    = PWM_GEN_3; 
      pwm_pin_mux      = GPIO_PK4_M0PWM6; 
      pwm_pin_out      = PWM_OUT_6;
      pwm_pin_out_bit  = PWM_OUT_6_BIT;
      break;
      
    case PG_1:  
    
      pwm_generator    = PWM_GEN_2; 
      pwm_pin_mux      = GPIO_PG1_M0PWM5; 
      pwm_pin_out      = PWM_OUT_5;
      pwm_pin_out_bit  = PWM_OUT_5_BIT;
      break;
      
    case PF_0: 
    
      pwm_generator    = PWM_GEN_0; 
      pwm_pin_mux      = GPIO_PF0_M0PWM0; 
      pwm_pin_out      = PWM_OUT_0;
      pwm_pin_out_bit  = PWM_OUT_0_BIT;
      break;
      
    default:
      break;
  }     

  uint32_t port_base_address = (uint32_t)portBASERegister(digitalPinToPort(pin));
  uint8_t  pin_bit_mask      = digitalPinToBitMask(pin);

  if(!pwm_periph_begin)
  {
    SysCtlPeripheralEnable(      SYSCTL_PERIPH_PWM0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0));
    PWMClockSet(PWM0_BASE, PWM_SYSCLK_DIV_1);
    pwm_periph_begin = true;
  }
    
  GPIOPinTypePWM(port_base_address, pin_bit_mask);  
  GPIOPinConfigure(pwm_pin_mux);
  
  PWMGenConfigure(   PWM0_BASE, pwm_generator, PWM_GEN_MODE_NO_SYNC | PWM_GEN_MODE_DOWN);
  PWMDeadBandDisable(PWM0_BASE, pwm_generator);
  PWMGenPeriodSet(   PWM0_BASE, pwm_generator, (period_micros * TICKS_PER_MICRO));          
  PWMGenEnable(      PWM0_BASE, pwm_generator);  
  PWMPulseWidthSet(  PWM0_BASE, pwm_pin_out, (pulse_width_micros * TICKS_PER_MICRO)); 
  PWMOutputState(    PWM0_BASE, pwm_pin_out_bit, true);
  return;
}


