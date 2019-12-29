#include "pn532.h"
#include "usart.h"
#include "inc/hw_memmap.h"

void PN532_Init()
{
	PN532_Active();
}
int PN532_Active()
{
		UARTWriteCmd(UART0_BASE,(char*)PN532_ACTIVE,24);
		UARTReceiveData(UART0_BASE,(char*)PN532_BUFFER,15);
	
    uint8_t i;
    for(i = 0; i<15; i++)
    {
        if(PN532_BUFFER[i] != PN532_ACTIVE_OK[i])
        {
            break;
        }
    }
    PN532_Clearbuffer();
    if(i == 15)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void PN532_Clearbuffer()
{
    for(int i=0; i<PN532_BUFFER_SIZE; i++)
    {
        PN532_BUFFER[i] = 0x00;
    }
}


uint32_t pn532_read()
{
//	HAL_UART_Transmit(&huart1,PN532_GETUID,11,100);
//  HAL_UART_Receive(&huart1,PN532_BUFFER,25,100);
	UARTWriteCmd(UART0_BASE,(char*)PN532_GETUID,11);
	UARTReceiveData(UART0_BASE,(char*)PN532_BUFFER,25);
  uint32_t uid=0;
	
	for(int i=0; i<4; i++)
  {
        uid = PN532_BUFFER[19+i] + (uid<<8);
  }
	
	PN532_Clearbuffer();
//	HAL_Delay(100);
	return uid;
}
uint32_t PN532_Readuid()
{
		uint32_t uid=0;
    for (int i=0;i<5;i++)
		{
			uid = pn532_read();
			if(uid != 0)
			{
				break;
			}
			
		}
   
    return uid;
}



