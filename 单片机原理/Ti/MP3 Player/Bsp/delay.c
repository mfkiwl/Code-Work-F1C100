/*
 * delay.c
 *
 *  Created on: 2019��10��15��
 *      Author: Chenhui
 */
#include "delay.h"
#include "driverlib/sysctl.h"

void delay()
{
    int i,j;
    for(i=0; i<1000; i++)
    {
        for(j=0; j<300; j++);
    }
}

void delay_us(int n)
{
	SysCtlDelay(n*SysCtlClockGet()/3000000);
}
