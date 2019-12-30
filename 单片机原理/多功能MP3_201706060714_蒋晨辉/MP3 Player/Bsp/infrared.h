#ifndef INFRARED_H
#define INFRARED_H

#include <stdint.h>
#include <stdbool.h>

#define irKEY1 0x22
#define irKEY2 0x02
#define irKEY3 0xc2
#define irKEY4 0xa8
#define irKEY5 0xe0



#define PN4_STATUS GPIOPinRead(GPIO_PORTN_BASE,GPIO_PIN_4)
void INFRARED_Init(void);
void IrOutput(void);
bool getdata(void);
uint8_t Ir_Server(void);
#endif
