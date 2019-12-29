#include "timer.h"

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "uartstdio.h"
#include "adc.h"
#include "PCA9557.h"
#include "delay.h"
#include "led.h"

#include "FreeRTOS.h"
#include "task.h"

extern int SysClock ;
extern volatile uint32_t tcount ;

void TimerIntInitial(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
		TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PERIODIC);
		TimerLoadSet(TIMER0_BASE, TIMER_B, SysClock/10000);
		TimerIntRegister(TIMER0_BASE,TIMER_B,Timer0BIntHandler);
		TimerIntEnable(TIMER0_BASE,TIMER_TIMB_TIMEOUT);
		IntEnable(INT_TIMER0B);
		TimerEnable(TIMER0_BASE,TIMER_B);
}

void Timer0BIntHandler(void)
{

    unsigned long Status;
		
   	Status = TimerIntStatus(TIMER0_BASE, true);

    if(Status == TIMER_TIMB_TIMEOUT)
    {
        tcount++;
    }
		TimerIntClear(TIMER0_BASE, Status);
}

