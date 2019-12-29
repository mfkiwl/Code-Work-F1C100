#include "usart.h"

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
#include "driverlib/uart.h"
#include "driverlib/timer.h"
#include "utils/uartstdio.h"

extern uint32_t SysClock;

void USART0_Init()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTConfigSetExpClk(UART0_BASE, SysClock, 115200,	
						(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}

void USART6_Init()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART6);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
	GPIOPinConfigure(GPIO_PP0_U6RX);
	GPIOPinConfigure(GPIO_PP1_U6TX);
	GPIOPinTypeUART(GPIO_PORTP_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTConfigSetExpClk(UART6_BASE, SysClock, 9600,	
						(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));			
}


void UARTWriteCmd(uint32_t base,char * buffer,uint8_t len)
{
	for(int i=0;i<len;i++)
	{
		UARTCharPut(base,buffer[i]);
	}
}

void UARTReceiveData(uint32_t base,char*buffer,uint8_t len)
{
	uint8_t i=0;
	while(i!=len)
	{
			buffer[i++] = UARTCharGet(base);
	}
}