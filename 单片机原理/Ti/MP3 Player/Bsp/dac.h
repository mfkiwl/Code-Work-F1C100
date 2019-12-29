#ifndef DAC_H
#define DAC_H

#include <stdint.h>

void DAC_Init(void);
void DAC_Output(uint16_t out);
void DAC_Test(void);

#endif
