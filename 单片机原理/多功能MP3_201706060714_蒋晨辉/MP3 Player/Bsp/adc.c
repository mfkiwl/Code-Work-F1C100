#include "adc.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/adc.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

#include "usart.h"
#include "utils/uartstdio.h"
#include "delay.h"
#include "PCA9557.h"

void ADC0_Init()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_7);
	
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_END | ADC_CTL_IE);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntClear(ADC0_BASE, 3);
	
	  ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_CH4 | ADC_CTL_END | ADC_CTL_IE);
    ADCSequenceEnable(ADC0_BASE, 2);
    ADCIntClear(ADC0_BASE, 2);

}

void EXADC_Init()
{
	//RD~PE0	CS~PE1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOE);
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1|GPIO_PIN_0);
	
	//AD_BUSY~PD6
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_6);
	
	//AD_BYTE~PD5	AD_CONVST~PD4
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_5|GPIO_PIN_4);
	
	
	
	/*
		DATA0  ~ PK3
		DATA1  ~ PK2
		DATA2  ~ PK1
		DATA3  ~ PK0
		DATA4  ~ PC7
		DATA5  ~ PC6
		DATA6  ~ PC5
		DATA7  ~ PC4
		DATA8  ~ PA6
		DATA9  ~ PA7
		DATA10 ~ PG1
		DATA11 ~ PG0
	*/
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOK);
	GPIOPinTypeGPIOInput(GPIO_PORTK_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0);
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOC);
	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4);
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_6|GPIO_PIN_7);
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOG);
	GPIOPinTypeGPIOInput(GPIO_PORTG_BASE, GPIO_PIN_1|GPIO_PIN_0);
}

uint16_t EXADC_Read()
{
	
	uint16_t exad_value=0;
	uint16_t input_buffer[12]={};
	
	//PULL CS & RD HIGH
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_PIN_0);
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1);
	
	//PULL AD_CONVST LOW  AD_BYTE LOW
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_4, 0);
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_5, 0);
			
	//PULL CS LOW
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);
	
	//PULL AD_CONVST HIGH
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_4, GPIO_PIN_4);
	SysCtlDelay(2);

	//PULL AD_CONVST LOW
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_4, 0);
	SysCtlDelay(3);	
		
	//PULL RD LOW
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0);
	SysCtlDelay(1);
	
	//DATA READ
	input_buffer[0] = GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_0);      //DATA11
	input_buffer[1] = GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_1)>>1;   //DATA10
	input_buffer[2] = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7)>>7;   //DATA9
	input_buffer[3] = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_6)>>6;   //DATA8
	input_buffer[4] = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4)>>4;   //DATA7
	input_buffer[5] = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5)>>5;   //DATA6
	input_buffer[6] = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_6)>>6;   //DATA5
	input_buffer[7] = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_7)>>7;   //DATA4
	input_buffer[8] = GPIOPinRead(GPIO_PORTK_BASE, GPIO_PIN_0);      //DATA3
	input_buffer[9] = GPIOPinRead(GPIO_PORTK_BASE, GPIO_PIN_1)>>1;   //DATA2
	input_buffer[10] = GPIOPinRead(GPIO_PORTK_BASE, GPIO_PIN_2)>>2;  //DATA1
	input_buffer[11] = GPIOPinRead(GPIO_PORTK_BASE, GPIO_PIN_3)>>3;  //DATA0
	
	//PULL CS & RD HIGH
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_PIN_0);
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1);
	
	exad_value = input_buffer[0]*2048+input_buffer[1]*1024+input_buffer[2]*512+input_buffer[3]*256+input_buffer[4]*128+input_buffer[5]*64+input_buffer[6]*32 \
							+input_buffer[7]*16+input_buffer[8]*8+input_buffer[9]*4+input_buffer[10]*2+input_buffer[11];
	
	return exad_value;
}

void EXADC_Test()
{
	uint16_t exad_value[10];
	uint16_t sum=0;
	float v;
	char s[20];
	while(1)
	{
		for(int i=0;i<10;i++)
		{
			exad_value[i] = EXADC_Read();
			sum+=exad_value[i];
			SysCtlDelay(SysCtlClockGet()/3);
		}
		
		UARTprintf("EXAD_Value: %4d\n",sum/10);
		sum=0;
		SysCtlDelay(1000*SysCtlClockGet()/3000);
	}
}

uint16_t ADC0_Read(int channel)
{
			uint32_t v;
			switch(channel)
			{
				case (0):
				{
					ADCProcessorTrigger(ADC0_BASE, 3);
					while(!ADCIntStatus(ADC0_BASE, 3, false)) ;
					ADCIntClear(ADC0_BASE, 3);
					ADCSequenceDataGet(ADC0_BASE, 3, &v);
					break;
				}
				case (4):
				{
					ADCProcessorTrigger(ADC0_BASE, 2);
					while(!ADCIntStatus(ADC0_BASE, 2, false)) ;
					ADCIntClear(ADC0_BASE, 2);
					ADCSequenceDataGet(ADC0_BASE, 2, &v);
					break;
				}
			}
			return v;
}

void ADC0_Test()
{
	uint16_t adc_value[10];
	float v[10];
	char s[10];
	float temperature[10];
	float temperature_avg,v_avg;
	float temp_sum;
	while(1)
	{
		for(int i=0;i<10;i++)
		{
			adc_value[i] = ADC0_Read(0);
			temperature[i] = (1.8663-((adc_value[i]/4096.0)*3.3))*1000/11.69;
			adc_value[i] = ADC0_Read(4);
			v[i] = adc_value[i]/4096.0*3.3*1000;
		}
		
		for(int i=0;i<10;i++)
		{
			temp_sum+=temperature[i];
		}
		temperature_avg = temp_sum/10;
		temp_sum = 0;
		for(int i=0;i<10;i++)
		{
			temp_sum+=v[i];
		}
		v_avg = temp_sum/10;
		temp_sum = 0;
		sprintf(s,"Temperature: %.4lfC  Voltage: %.4lfmV\n",temperature_avg,v_avg);
		UARTprintf(s);

		//PCA9557_Show_Voltage(s);
		//delay();
	}
}