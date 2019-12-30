/*
 * buzzer.c
 *
 *  Created on: 2019骞�10鏈�15鏃�
 *      Author: Chenhui
 */

#include "buzzer.h"
#include "delay.h"
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

extern uint32_t SysClock;
void BUZZER_Init()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
    GPIODirModeSet(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD);
}

void BUZZER_Beep()
{
    for(int i =0; i<80; i++)
    {
        GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
        SysCtlDelay(SysClock/3000);
        GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
        SysCtlDelay(SysClock/3000);
    }

}

void BUZZER_Open()
{
    GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_PIN_5);
}

void BUZZER_Close()
{
    GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_5, 0);
}

void BUZZER_Test()
{
    while(1)
    {
        BUZZER_Beep();
//        delay();
    }

}
