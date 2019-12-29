//*****************************************************************************
//
// led_task.c - A simple flashing LED task.
//
// Copyright (c) 2009-2017 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.4.178 of the DK-TM4C129X Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "idle_task.h"
#include "led_task.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
//*****************************************************************************
//
// The stack size for the LED toggle task.
//
//*****************************************************************************
#define STACKSIZE_LEDTASK       128

//*****************************************************************************
//
// The amount of time to delay between toggles of the LED.
//
//*****************************************************************************
uint32_t g_ui32LEDDelay = 500;

//*****************************************************************************
//
// This task simply toggles the user LED at a 1 Hz rate.
//
//*****************************************************************************
static void
LEDTask(void *pvParameters)
{
    portTickType ui32LastTime;

    //
    // Get the current tick count.
    //
    ui32LastTime = xTaskGetTickCount();

    //
    // Loop forever.
    //
    while(1)
    {
        //
        // Turn on the user LED.
        //
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);

        //
        // Wait for the required amount of time.
        //
//vTaskDelayUntil(&ui32LastTime, g_ui32LEDDelay / portTICK_RATE_MS);
				vTaskDelay(1000);
			

        //
        // Turn off the user LED.
        //
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);

        //
        // Wait for the required amount of time.
        //
        //vTaskDelayUntil(&ui32LastTime, g_ui32LEDDelay / portTICK_RATE_MS);
			vTaskDelay(1000);

    }
}

//*****************************************************************************
//
// Initializes the LED task.
//
//*****************************************************************************
uint32_t
LEDTaskInit(void)
{
    //
    // Initialize the GPIO used to drive the user LED.
    //
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);

    //
    // Create the LED task.
    //
    if(xTaskCreate(LEDTask, (const portCHAR *)"LED", STACKSIZE_LEDTASK, NULL,
                   tskIDLE_PRIORITY + PRIORITY_LED_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}
