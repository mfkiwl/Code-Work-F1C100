#ifndef USART_H
#define USART_H

#include "stdint.h"

void USART0_Init(void);
void USART6_Init(void);
void UARTWriteCmd(uint32_t base,char * buffer,uint8_t len);
void UARTReceiveData(uint32_t base,char*buffer,uint8_t len);
#endif

