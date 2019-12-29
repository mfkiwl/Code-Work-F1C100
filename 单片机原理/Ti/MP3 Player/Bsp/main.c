

///**
// * main.c
// */
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
#include "driverlib/timer.h"
#include "utils/uartstdio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "led.h"
#include "key.h"
#include "buzzer.h"
#include "pwm.h"
#include "PCA9557.h"
#include "timer.h"
#include "usart.h"
#include "adc.h"
#include "dac.h"
#include "accelerator.h"
#include "sd.h"
#include "tft.h"
#include "infrared.h"
#include "touch.h"

int ui32SysClock;
uint32_t TouchXData[6];
uint32_t TouchYData[6];
uint32_t TouchZData[6]; 

extern void SysTickHandler();
extern void xPortSysTickHandler();
extern void xPortPendSVHandler();
void System_Init()
{
    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                       SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                                        SYSCTL_CFG_VCO_480), 16000000);
		FPUEnable();
    FPULazyStackingEnable();
		USART_Init();
		//SD_Init();
		//ACCELERATOR_Init();
		TFT_Init();
		//EXADC_Init();
    //PCA9557_Init();
		LED_Init();
		TOUCH_Init();
		KEY_Init();
		KEY_Int_Init();
    //TimerIntInitial();
		//BUZZER_Init();
		//PWM_Init();
	  
		//ADC0_Init();
		//DAC_Init();
		INFRARED_Init();
}

void NVIC_Configure()
{
    IntPrioritySet(INT_GPION,0x30);
    IntPrioritySet(INT_GPIOD,0x40);
    IntPrioritySet(INT_TIMER0B, 0xe0);

    IntEnable(INT_GPION);
    IntEnable(INT_GPIOD);
    IntEnable(INT_TIMER0B);
}


static void LED1Task(void *pvParameters)
{

    while(1)
    {
				LED_Open(1);
				vTaskDelay(500);
				LED_Close(1);
				vTaskDelay(500);
    }
}

static void LED2Task(void *pvParameters)
{

    while(1)
    {
				LED_Open(2);
				vTaskDelay(500);
				LED_Close(2);
				vTaskDelay(500);
    }
}


int main(void)
{
    System_Init();
//		//NVIC_Configure();
//		//IntMasterEnable();
//		TFT_Test();
//		//ACCELERATOR_Test();
//		//SD_Test();
//		//EXADC_Test();
//		//DAC_Test();
//		//ADC0_Test();
//		//PCA9557_Test();
//		//NVIC_Configure();
//		//IntMasterEnable();
//		//TimerEnable(TIMER0_BASE, TIMER_B);
//		//PWM_Test();
//		
////		xTaskCreate(LED1Task, (const portCHAR *)"LED1", 128, NULL,
////                   tskIDLE_PRIORITY + 10, NULL);
////		xTaskCreate(LED2Task, (const portCHAR *)"LED2", 128, NULL,
////                   tskIDLE_PRIORITY + 10, NULL);
////		vTaskStartScheduler();
//	//	LED_Test();
TOUCH_Test();
    while(1)
    {
			

    }
    return 0;


 




    

    

		

}



