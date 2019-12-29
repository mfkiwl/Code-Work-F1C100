#include "pwm.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/pwm.h"

void PWM_Init()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinConfigure(GPIO_PF1_M0PWM1);
    GPIOPinConfigure(GPIO_PF2_M0PWM2);
    GPIOPinConfigure(GPIO_PF3_M0PWM3);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);
    PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN |
                    PWM_GEN_MODE_NO_SYNC);
    PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN |
                    PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, PWM_Frequence);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, PWM_Frequence);
    PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT | PWM_OUT_2_BIT | PWM_OUT_3_BIT, true);
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);
    PWMGenEnable(PWM0_BASE, PWM_GEN_1);
}

void PWM_Set(int index,float t)
{
    uint32_t PWMPulseWidth = PWM_Frequence * t;
    switch(index)
    {
    case 1:
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1,PWMPulseWidth);
        break;
    case 2:
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2,PWMPulseWidth);
        break;
    case 3:
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3,PWMPulseWidth);
        break;
    default:
        break;
    }

}
void PWM_Test()
{
    float t=0;
    for(;;)
    {
        PWM_Set(1,t);
        PWM_Set(2,t);
        PWM_Set(3,t);
        SysCtlDelay(10*16000000/3000);
        t += 0.001;
        if(t >= 1)t=0;
    }
}
