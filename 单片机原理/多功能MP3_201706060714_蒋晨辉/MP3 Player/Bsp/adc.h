#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include <stdbool.h>

void ADC0_Init(void);
uint16_t ADC0_Read(int);
void ADC0_Test(void);
void EXADC_Init(void);
uint16_t EXADC_Read(void);
void EXADC_Test(void);

#endif


