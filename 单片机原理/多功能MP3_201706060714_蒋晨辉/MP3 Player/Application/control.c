#include "control.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "usart.h"

uint8_t PLAY_CMD[5]  						= 	{0x7E, 0X03, 0X11, 0X12, 0XEF};
uint8_t PAUSE_CMD[5] 						= 	{0X7E, 0X03, 0X12, 0X11, 0XEF};
uint8_t NEXT_CMD[5] 			 			= 	{0X7E, 0X03, 0X13, 0X10, 0XEF};
uint8_t PREVIOUS_CMD[5]		 			= 	{0X7E, 0X03, 0X14, 0X17, 0XEF};
uint8_t ADD_VOLUME_CMD[5]	 			= 	{0X7E, 0X03, 0X15, 0X16, 0XEF};
uint8_t SUB_VOLUME_CMD[5] 			= 	{0X7E, 0X03, 0X16, 0X15, 0XEF};
uint8_t PLAY_OR_PAUSE_CMD[5] 		= 	{0X7E, 0X03, 0X1C, 0X1F, 0XEF};
uint8_t STOP_CMD[5]							=		{0X7E, 0X03, 0X1E, 0X1D, 0XEF};
uint8_t HALF_VOLUME[6]          =   {0X7E, 0X04, 0X31, 0X0F, 0X3A, 0XEF};
uint8_t LOOP[6]                 =   {0X7E, 0X04, 0X33, 0X00, 0X37, 0XEF};

extern uint32_t SysClock;

void PLAYER_Init()
{
    UARTWriteCmd(UART6_BASE,(char *)LOOP, 6);
}

int PLAY(void)
{
    UARTWriteCmd(UART6_BASE,(char*)PLAY_CMD, 5);
}
int PAUSE(void)
{
    UARTWriteCmd(UART6_BASE,(char*)PAUSE_CMD, 5);
}
int NEXT(void)
{
    UARTWriteCmd(UART6_BASE,(char*)NEXT_CMD, 5);
}
int PREVIOUS(void)
{
    UARTWriteCmd(UART6_BASE,(char*)PREVIOUS_CMD, 5);
}
int ADD_VOLUME(void)
{
    UARTWriteCmd(UART6_BASE,(char*)ADD_VOLUME_CMD, 5);
}
int SUB_VOLUME(void)
{
    UARTWriteCmd(UART6_BASE,(char*)SUB_VOLUME_CMD, 5);
}

int STOP(void)
{
    UARTWriteCmd(UART6_BASE,(char*)STOP_CMD, 5);
}

int PLAY_OR_PAUSE(void)
{
    UARTWriteCmd(UART6_BASE,(char*)PLAY_OR_PAUSE_CMD, 5);
}

int SET_VOLUME(uint8_t volume)
{
		char check = volume^0x04^0x31;
		uint8_t buffer[6] = {0X7E, 0X04, 0X31, volume, check, 0XEF};
		UARTWriteCmd(UART6_BASE,(char*)buffer,6);
}