#include "infrared.h"

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "utils/uartstdio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

extern int SysClock;
extern volatile uint32_t tcount;
extern uint8_t irdata[33];   //用于记录两个下降沿之间的时间
extern volatile bool receiveComplete; //接收完成标志位
extern uint8_t idx;          //用于索引接收到的数值
extern bool startflag;       //表示开始接收
extern uint8_t remote_code[4];
extern SemaphoreHandle_t InfraredReceive_Handle;

/*
ch-:				0x45  0x8b
ch:					0x46  0x8d
ch+:				0x47
prev:				0x44
next:				0x40
play/pause:	0x43
vol-:				0x07
vol+:				0x15
eq:					0x09
0:					0x16  0x2d  0xb7
100+:				0x19
200+:				0x0d
1:					0x0c
2:					0x18
3:					0x5e
4:					0x08
5:					0x1c
6:					0x5a
7:					0x42
8:					0x52
9:					0x4a
*/



void 
GPION_IRQ(void)
{

	uint16_t ir_time;
	BaseType_t pxHigherPriorityTaskWoken;
  if(startflag)
  {
      ir_time = tcount; 
      if(tcount < 160 && tcount >= 50 ) // 接收到同步头
      {
          idx=0;  // 数组下标清零
      }

      irdata[idx] = tcount;  // 获取计数时间
      tcount = 0;            // 清零计数时间，以便下次统计
      idx++;                     // 接收到一个数据，索引加1

      if(idx==33)       // 如果接收到33个数据，包括32位数和以一个同步头
      {
          idx=0;
          tcount = 0;
          xSemaphoreGiveFromISR(InfraredReceive_Handle,&pxHigherPriorityTaskWoken);
      }
  }
  else   // 下降沿第一次触发
  {
      idx = 0;
      tcount = 0;
      startflag = 1;
  }


	
	GPIOIntClear(GPIO_PORTN_BASE,GPIO_INT_PIN_4);
	
}

void INFRARED_Init()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    GPIODirModeSet(GPIO_PORTN_BASE, GPIO_PIN_4, GPIO_DIR_MODE_IN);
		GPIOPadConfigSet(GPIO_PORTN_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
		GPIOIntRegister(GPIO_PORTN_BASE, GPION_IRQ);
    GPIOIntTypeSet(GPIO_PORTN_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTN_BASE, GPIO_INT_PIN_4);
		IntEnable(INT_GPION);
		IntPrioritySet(INT_GPION,0);
		GPIOIntClear(GPIO_PORTN_BASE,GPIO_INT_PIN_4);
}

uint8_t Ir_Server()
{
  uint8_t i,j,idx=1; //idx 从1 开始表示对同步头的时间不处理
  uint8_t temp;
  for(i=0; i<4; i++)
  {
      for(j=0; j<8; j++)
      {
          if(irdata[idx] >=8 && irdata[idx] < 15)   //表示 0
          {
              temp = 0;
          }
          else if(irdata[idx] >=18 && irdata[idx]<25) //表示 1
          {
              temp = 1;
          }
          remote_code[i] <<= 1;
          remote_code[i] |= temp;
          idx++;
      }
  }

  return remote_code[2];  // 该数组中记录的是控制码，每个按键不一样
}
