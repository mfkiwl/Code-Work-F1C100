/*
 * TOUCH_TSC2046.c
 *
 *  Created on: 2014Äê8ÔÂ28ÈÕ
 *      Author: Sam
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "tft.h"
#include "touch.h"
#include "uartstdio.h"
#include "KalmanFilter.h"
#include "buzzer.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"

extern int SysClock;
extern EventGroupHandle_t Command_Handle ;
extern SemaphoreHandle_t BUZZER_Handle;

#define NUM 1000

uint32_t GetData[NUM];

//
// TSC2046 control line GPIO definitions.


#define TOUCH_BUSY_PERIPH           	SYSCTL_PERIPH_GPIOB
#define TOUCH_BUSY_BASE            		GPIO_PORTB_BASE
#define TOUCH_BUSY_PIN              	GPIO_PIN_1
#define TOUCH_BUSY_PIN_REG           	0x40005008
#define TOUCH_PEN_PERIPH           		SYSCTL_PERIPH_GPIOB
#define TOUCH_PEN_BASE             		GPIO_PORTB_BASE
#define TOUCH_PEN_PIN                 GPIO_PIN_0
#define TOUCH_PEN_PIN_REG           	0x40005004


#define TOUCH_XMIN  261
#define TOUCH_XMAX  3820
#define TOUCH_YMIN  200
#define TOUCH_YMAX  3820

#define TFT_XSIZE   240
#define TFT_YSIZE   400

#define KEY1_CENTER_X  45
#define KEY1_CENTER_Y  290

#define KEY2_CENTER_X  182
#define KEY2_CENTER_Y  290

#define KEY3_CENTER_X  118
#define KEY3_CENTER_Y  290

#define KEY4_CENTER_X  110
#define KEY4_CENTER_Y  220

#define KEY5_CENTER_X  110
#define KEY5_CENTER_Y  360

#define KEY_AREA 22

#define KEY1_EVENT (0x01 << 0)
#define KEY2_EVENT (0x01 << 1)
#define KEY3_EVENT (0x01 << 2)
#define KEY4_EVENT (0x01 << 3)
#define KEY5_EVENT (0x01 << 4)

void SSIinit(uint32_t ui32SysClockSpeed)
{
    uint32_t g_pui32DataRx[3];

    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    GPIOPinConfigure(GPIO_PA4_SSI0XDAT0);
    GPIOPinConfigure(GPIO_PA5_SSI0XDAT1);

    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2);


    SSIConfigSetExpClk(SSI0_BASE, ui32SysClockSpeed, SSI_FRF_NMW, SSI_MODE_MASTER, 2000000, 12);


    SSIEnable(SSI0_BASE);

    while(SSIDataGetNonBlocking(SSI0_BASE, &g_pui32DataRx[0]))
    {
    }

}


// As TSC2046 DIN data are latched on the rising edge of DCLK
// while DOUT are shifted on the falling edge of DCLK
// we adopt software simulation SPI to transmit data.
void TOUCH_Init()
{

    SysCtlPeripheralEnable(TOUCH_BUSY_PERIPH);
    SysCtlPeripheralEnable(TOUCH_PEN_PERIPH);
    GPIOPinTypeGPIOInput(TOUCH_BUSY_BASE, TOUCH_BUSY_PIN);
    GPIOPinTypeGPIOInput(TOUCH_PEN_BASE, TOUCH_PEN_PIN);

    SSIinit(SysClock);

}



void TOUCH_PointAdjust(uint32_t *TouchXData, uint32_t *TouchYData)
{
    uint32_t x = 0, y = 0;
    if(*TouchXData < TOUCH_XMIN || *TouchXData > TOUCH_XMAX)
    {
        *TouchXData = 0xFFFF;
        return;
    }
    if(*TouchYData < TOUCH_YMIN || *TouchYData > TOUCH_YMAX)
    {
        *TouchYData = 0xFFFF;
        return;
    }
    x = *TouchXData;
    y = *TouchYData;
    *TouchXData = TFT_XSIZE * x / (TOUCH_XMAX - TOUCH_XMIN) - 15;
    *TouchYData = TFT_YSIZE - (TFT_YSIZE * y / (TOUCH_YMAX - TOUCH_YMIN)) + 24;
}


uint8_t Button_Reconize(uint16_t x, uint16_t y)
{
    if (abs(x - KEY1_CENTER_X) < KEY_AREA && abs(y - KEY1_CENTER_Y) < KEY_AREA)
    {
        return 1;
    }
    if (abs(x - KEY2_CENTER_X) < KEY_AREA && abs(y - KEY2_CENTER_Y) < KEY_AREA)
    {
        return 2;
    }
    if (abs(x - KEY3_CENTER_X) < KEY_AREA && abs(y - KEY3_CENTER_Y) < KEY_AREA)
    {
        return 3;
    }
    if (abs(x - KEY4_CENTER_X) < KEY_AREA && abs(y - KEY4_CENTER_Y) < KEY_AREA)
    {
        return 4;
    }
    if (abs(x - KEY5_CENTER_X) < KEY_AREA && abs(y - KEY5_CENTER_Y) < KEY_AREA)
    {
        return 5;
    }
    return 0;
}

void Get_Point()
{
    uint16_t ui32Loop = 0;
    uint32_t sum[2] = {0};
    SSIDataPut(SSI0_BASE, 0xd0);

    while(1)
    {
        if(!GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0))
        {
            uint32_t touch_x = 0;
            uint32_t touch_y = 0;

            uint32_t x = 0;
            uint32_t y = 0;

            KFP KFP_X = {0.02, 0, 0, 0, 0.001, 0.543};
            KFP KFP_Y = {0.02, 0, 0, 0, 0.001, 0.543};


            for(ui32Loop = 0; ui32Loop < NUM - 1; ui32Loop++)
            {
                SSIDataPut(SSI0_BASE, 0x90);
                SysCtlDelay(3);
                SSIDataGet(SSI0_BASE, &x);
                SysCtlDelay(3);
                SSIDataPut(SSI0_BASE, 0xd0);
                SysCtlDelay(3);
                SSIDataGet(SSI0_BASE, &y);
                SysCtlDelay(3);
                touch_x = kalmanFilter(&KFP_X, (float)x);
                touch_y = kalmanFilter(&KFP_Y, (float)y);

            }
						
            TOUCH_PointAdjust(&touch_x, &touch_y);
						////UARTprintf("x:%d,y:%d\n",touch_x,touch_y);
            
					
            if((touch_x <= 240) && (touch_y <= 400) )
            {
                
                uint8_t res = Button_Reconize(touch_x, touch_y);
                switch(res)
                {
                case 1:
                    xEventGroupSetBits(Command_Handle, KEY1_EVENT);
                    break;
                case 2:
                    xEventGroupSetBits(Command_Handle, KEY2_EVENT);
                    break;
                case 3:
                    xEventGroupSetBits(Command_Handle, KEY3_EVENT);
                    break;
                case 4:
                    xEventGroupSetBits(Command_Handle, KEY4_EVENT);
                    break;
                case 5:
                    xEventGroupSetBits(Command_Handle, KEY5_EVENT);
                    break;
                default:
                    break;
                }
								
                SysCtlDelay(7 * SysClock / 100);
            }

        }
    }
}
