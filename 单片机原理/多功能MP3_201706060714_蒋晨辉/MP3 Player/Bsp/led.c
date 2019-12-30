/*
 * led.c
 *
 *  Created on: 2019Äê10ÔÂ14ÈÕ
 *      Author: Chenhui
 */

#include "led.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"

#include "sys.h"
#include "delay.h"
#include "key.h"
void LED_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
    GPIODirModeSet(GPIO_PORTL_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(GPIO_PORTL_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD);
}

void LED_Open(int i)
{
    switch(i)
    {
    case 1:
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
        break;
    case 2:
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
        break;
    case 3:
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
        break;
    case 4:
        GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_0, GPIO_PIN_0);
        break;
    case 5:
        GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_1, GPIO_PIN_1);
        break;
    case 6:
        GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_2, GPIO_PIN_2);
        break;
    case 7:
        GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_3, GPIO_PIN_3);
        break;
    case 8:
        GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_4, GPIO_PIN_4);
        break;
    default:
        break;

    }
}

void LED_Close(int i)
{
    switch(i)
    {
    case 1:
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
        break;
    case 2:
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
        break;
    case 3:
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
        break;
    case 4:
        GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_0, 0);
        break;
    case 5:
        GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_1, 0);
        break;
    case 6:
        GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_2, 0);
        break;
    case 7:
        GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_3, 0);
        break;
    case 8:
        GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_4, 0);
        break;
    default:
        break;

    }
}

void LED_Test(void)
{
//    int i=0,t=0;
//    int flag = 0;
//    for(;;)
//    {
//        t=KEY_Read();
//        if(t==1)
//        {
//            flag=1;
//        }
//        if(t==2)
//        {
//            flag=0;
//        }
//        if(flag == 1) {
//            i++;
//            if(i>8)
//            {
//                i=1;
//            }
//            LED_Open(i);
//            ROM_SysCtlDelay(16000000/3);
//            LED_Close(i);
//        }
//    }

}
