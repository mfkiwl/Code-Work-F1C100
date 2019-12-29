#ifndef APPLICATION_H
#define APPLICATION_H


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
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/fpu.h"

#include "uartstdio.h"
#include "led.h"
#include "timer.h"
#include "usart.h"
#include "tft.h"
#include "infrared.h"
#include "touch.h"
#include "control.h"
#include "buzzer.h"
#include "sd.h"
#include "accelerator.h"
#include "KalmanFilter.h"
#include "bmp.h"
#include "adc.h"
#include "i2c.h"
#include "PCA9557.h"
#include "OLED_I2C.h"
#include "pn532.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"
#include "timers.h"

#define SysClockFreq configCPU_CLOCK_HZ
#define KEY1_EVENT (0x01 << 0)                    //按键事件掩码
#define KEY2_EVENT (0x01 << 1)
#define KEY3_EVENT (0x01 << 2)
#define KEY4_EVENT (0x01 << 3)
#define KEY5_EVENT (0x01 << 4)

void BSP_Init(void);															//BSP初始化
void FreeRTOS_Init(void);													//FreeRtos初始化

#endif