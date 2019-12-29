/*
 * key.c
 *
 *  Created on: 2019Äê10ÔÂ15ÈÕ
 *      Author: Chenhui
 */


#include "key.h"

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "delay.h"
#include "led.h"
#include "buzzer.h"
#include "tft.h"

void KEY_Init()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
    GPIOPinTypeGPIOInput(GPIO_PORTP_BASE, GPIO_PIN_2);
    GPIOPadConfigSet(GPIO_PORTP_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    GPIOPinTypeGPIOInput(GPIO_PORTN_BASE, GPIO_PIN_3|GPIO_PIN_2);
    GPIOPadConfigSet(GPIO_PORTN_BASE, GPIO_PIN_3|GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIODirModeSet(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD);
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0);
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
    GPIODirModeSet(GPIO_PORTH_BASE, GPIO_PIN_3|GPIO_PIN_2, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(GPIO_PORTH_BASE, GPIO_PIN_3|GPIO_PIN_2, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD);
    GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_3|GPIO_PIN_2, 0);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
    GPIODirModeSet(GPIO_PORTM_BASE, GPIO_PIN_3, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(GPIO_PORTM_BASE, GPIO_PIN_3, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD);
    GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_3, 0);
}

void KEY_Int_Init()
{

    GPIOIntRegister(GPIO_PORTP_BASE, PortPIntHandler);
    GPIOIntTypeSet(GPIO_PORTP_BASE, GPIO_PIN_2, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTP_BASE, GPIO_PIN_2);

    GPIOIntRegister(GPIO_PORTN_BASE, PortNIntHandler);
    GPIOIntTypeSet(GPIO_PORTN_BASE, GPIO_PIN_3|GPIO_PIN_2, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTN_BASE, GPIO_PIN_3|GPIO_PIN_2);


    GPIOIntRegister(GPIO_PORTD_BASE, PortDIntHandler);
    GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTD_BASE, GPIO_PIN_0);

}


void KEY_Pullup()
{
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_PIN_1);
    GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_3|GPIO_PIN_2, GPIO_PIN_3|GPIO_PIN_2);
    GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_3, GPIO_PIN_3);
}

void KEY_Pulldown()
{
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0);
    GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_3|GPIO_PIN_2, 0);
    GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_3, 0);
}

int KEY_Read()
{

    KEY_Pullup();

    GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_1, 0);
    if(0==GPIOPinRead(GPIO_PORTP_BASE, GPIO_PIN_2))
    {
        delay();
        if(0==GPIOPinRead(GPIO_PORTP_BASE, GPIO_PIN_2))
        {
            KEY_Pulldown();
            return 1;
        }

    }
    if(0==GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_3))
    {
        delay();
        if(0==GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_3))
        {
            KEY_Pulldown();
            return 5;
        }

    }

    if(0==GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_2))
    {
        delay();
        if(0==GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_2))
        {
            KEY_Pulldown();
            return 9;
        }

    }

    if(0==GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_0))
    {
        delay();
        if(0==GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_0))
        {
            KEY_Pulldown();
            return 13;
        }

    }

    GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_1, GPIO_PIN_1);


    GPIOPinWrite(GPIO_PORTH_BASE,GPIO_PIN_3, 0);
    if(0==GPIOPinRead(GPIO_PORTP_BASE, GPIO_PIN_2))
    {
        delay();
        if(0==GPIOPinRead(GPIO_PORTP_BASE, GPIO_PIN_2))
        {
            KEY_Pulldown();
            return 2;
        }

    }
    if(0==GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_3))
    {
        delay();
        if(0==GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_3))
        {
            KEY_Pulldown();
            return 6;
        }

    }

    if(0==GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_2))
    {
        delay();
        if(0==GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_2))
        {
            KEY_Pulldown();
            return 10;
        }

    }

    if(0==GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_0))
    {
        delay();
        if(0==GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_0))
        {
            KEY_Pulldown();
            return 14;
        }

    }

    GPIOPinWrite(GPIO_PORTH_BASE,GPIO_PIN_3, GPIO_PIN_3);


    GPIOPinWrite(GPIO_PORTH_BASE,GPIO_PIN_2, 0);
    if(0==GPIOPinRead(GPIO_PORTP_BASE, GPIO_PIN_2))
    {
        delay();
        if(0==GPIOPinRead(GPIO_PORTP_BASE, GPIO_PIN_2))
        {
            KEY_Pulldown();
            return 3;
        }

    }
    if(0==GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_3))
    {
        delay();
        if(0==GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_3))
        {
            KEY_Pulldown();
            return 7;
        }

    }

    if(0==GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_2))
    {
        delay();
        if(0==GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_2))
        {
            KEY_Pulldown();
            return 11;
        }

    }

    if(0==GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_0))
    {
        delay();
        if(0==GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_0))
        {
            KEY_Pulldown();
            return 15;
        }

    }

    GPIOPinWrite(GPIO_PORTH_BASE,GPIO_PIN_2, GPIO_PIN_2);

    GPIOPinWrite(GPIO_PORTM_BASE,GPIO_PIN_3, 0);
    if(0==GPIOPinRead(GPIO_PORTP_BASE, GPIO_PIN_2))
    {
        delay();
        if(0==GPIOPinRead(GPIO_PORTP_BASE, GPIO_PIN_2))
        {
            KEY_Pulldown();
            return 4;
        }

    }
    if(0==GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_3))
    {
        delay();
        if(0==GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_3))
        {
            KEY_Pulldown();
            return 8;
        }

    }

    if(0==GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_2))
    {
        delay();
        if(0==GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_2))
        {
            KEY_Pulldown();
            return 12;
        }

    }

    if(0==GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_0))
    {
        delay();
        if(0==GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_0))
        {
            KEY_Pulldown();
            return 16;
        }

    }

    GPIOPinWrite(GPIO_PORTM_BASE,GPIO_PIN_3, GPIO_PIN_3);

    KEY_Pulldown();
    return 0;


}

void KEY_Test()
{
    int n;
    while(1)
    {
        n = KEY_Read();
        if(n) {
            LED_Open(n);
            delay();
            LED_Close(n);
        }
    }



}


void PortNIntHandler(void)
{
    unsigned long Status;
    Status=GPIOIntStatus(GPIO_PORTN_BASE,true);
    if(Status==GPIO_INT_PIN_2)
    {   for(int i =0; i<5; i++) {
            LED_Open(2);
            SysCtlDelay(100*(16000000/3000));//2
            LED_Close(2);
            SysCtlDelay(100*(16000000/3000));//2
        }
    }
    if(Status==GPIO_INT_PIN_3)
    {   for(int i =0; i<5; i++) {
            LED_Open(4);
            SysCtlDelay(100*(16000000/3000));//2
            LED_Close(4);
            SysCtlDelay(100*(16000000/3000));//2
        }
    }
    GPIOIntClear(GPIO_PORTN_BASE,Status);

}

void PortDIntHandler(void)
{
    unsigned long Status;
    Status=GPIOIntStatus(GPIO_PORTD_BASE,true);
    if(Status==GPIO_INT_PIN_0)
    {   for(int i =0; i<5; i++) {
            LED_Open(3);
            SysCtlDelay(200*(16000000/3000));//2
            LED_Close(3);
            SysCtlDelay(200*(16000000/3000));//2
        }
    }
    GPIOIntClear(GPIO_PORTD_BASE,Status);
}

void PortPIntHandler(void)
{
    unsigned long Status;
    Status=GPIOIntStatus(GPIO_PORTP_BASE,true);
    if(Status==GPIO_INT_PIN_2)
    {   
//			for(int i =0; i<5; i++) {
//            LED_Open(6);
//            SysCtlDelay(300*(16000000/3000));//2
//            LED_Close(6);
//            SysCtlDelay(300*(16000000/3000));//2
//        }
			TFT_CLEAR(0);
    }
    GPIOIntClear(GPIO_PORTP_BASE,Status);
}
