// Include Tiva Watchdog Driver Library
#include "RoveWatchdog.h"

#include <inc/hw_memmap.h>

#include <driverlib/sysctl.h>
#include <driverlib/watchdog.h>
#include <driverlib/rom_map.h>

//#define WATCHDOG0_BASE          0x40000000  // Watchdog0
//#define WATCHDOG1_BASE          0x40001000  // Watchdog1
// todo, beware of WATCHDOG0_BASE because we think it might conflict with Energia usage of SYSCTL_PERIPH_WTIMER0 ?
// Enable Watchdog Timer 1; supposedly timer 0 has a conflict in Energia, unconfirmed

// 5,500,000 => 6,000,000
// 500*120000000/1000 Why off by 10?

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//void RoveWatchdog::begin(void(*watchdogFunction)(void), unsigned int timeout_millis, unsigned int WatchdogTimerModule=WATCHDOG_1)
void RoveWatchdog::begin(void(*watchdogFunction)(void), unsigned int timeout_millis, unsigned int WatchdogTimerModule)
{     
  if(WatchdogTimerModule == WATCHDOG_0)
  {
    _WATCHDOG_BASE = WATCHDOG0_BASE;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);  
      
  } else if(WatchdogTimerModule == WATCHDOG_1)
  {
    _WATCHDOG_BASE = WATCHDOG1_BASE;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG1);      
  }  

  WatchdogUnlock(      _WATCHDOG_BASE);
  WatchdogReloadSet(   _WATCHDOG_BASE, (timeout_millis * (F_CPU / 1000)));
  WatchdogIntRegister( _WATCHDOG_BASE, watchdogFunction);
  WatchdogIntEnable(   _WATCHDOG_BASE);
  WatchdogResetDisable(_WATCHDOG_BASE);
  WatchdogEnable(      _WATCHDOG_BASE);  
}

//////////////////////////////////

void RoveWatchdog::clear()
{
  WatchdogIntClear(_WATCHDOG_BASE);
}
