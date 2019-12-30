#include "PCA9557.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "utils/uartstdio.h"
#include "delay.h"
#include "adc.h"

#include "FreeRTOS.h"
#include "task.h"

#define bool _Bool
#define milisecond 4000

extern int SysClock ;
unsigned char a[2];


bool I2C0Send(unsigned char slaveAddr, char *data, unsigned char size)
{
    unsigned char a;
    a=size;
//    char *send_data = data;
	
    switch(a) {
    case 0:
        return false;
    case 1: {	//当前数据地址的单字节发送
        //设置控制字节：从机地址，写操作false（读为true）
        I2CMasterSlaveAddrSet (I2C0_MASTER_BASE, slaveAddr, false);
        I2CMasterDataPut (I2C0_MASTER_BASE, *data); //设置数据
        I2CMasterControl (I2C0_MASTER_BASE,I2C_MASTER_CMD_SINGLE_SEND);  //启动”总线单字节发送”
        while(I2CMasterBusy(I2C0_MASTER_BASE))  { } // wait for ACK
        if (I2CMasterErr(I2C0_MASTER_BASE) != I2C_MASTER_ERR_NONE)
            return false;
        return true;
    }
    default: {   //非当前地址发送，参数data的第一个字节为子地址其余为数据
        //发送第一个字节（控制字节+子地址字节）
//		I2CMasterEnable(I2C0_MASTER_BASE); //使能I2C0主机模式
        I2CMasterSlaveAddrSet (I2C0_MASTER_BASE, slaveAddr, false);
        I2CMasterDataPut (I2C0_MASTER_BASE,*(data++)); //设置子地址
        I2CMasterControl (I2C0_MASTER_BASE,I2C_MASTER_CMD_BURST_SEND_START);  //启动”总线发送开始”
        while (I2CMasterBusy(I2C0_MASTER_BASE))  { }  // wait for ACK
        if (I2CMasterErr(I2C0_MASTER_BASE) != I2C_MASTER_ERR_NONE)
            return false;
        while (--size > 1) {  //发送中间字节（数据字节）
            I2CMasterDataPut (I2C0_MASTER_BASE, *(data++)); //设数据
            I2CMasterControl (I2C0_MASTER_BASE,I2C_MASTER_CMD_BURST_SEND_CONT);  //启动”总线发送继续”
            while (I2CMasterBusy(I2C0_MASTER_BASE))  { }	 // wait for ACK
            if (I2CMasterErr(I2C0_MASTER_BASE) != I2C_MASTER_ERR_NONE)
                return false;
        }
        //发送最后一个字节（数据字节）
        I2CMasterDataPut (I2C0_MASTER_BASE, *data); //设置数据
        I2CMasterControl (I2C0_MASTER_BASE,I2C_MASTER_CMD_BURST_SEND_FINISH);  //启动”总线发送结束”
        while (I2CMasterBusy(I2C0_MASTER_BASE))  { }  //wait for ACK
        if (I2CMasterErr(I2C0_MASTER_BASE) != I2C_MASTER_ERR_NONE)
        {
//				I2CMasterDisable(I2C0_MASTER_BASE); //使能I2C0主机模式
//				while (I2CMasterBusBusy(I2C0_MASTER_BASE))  { }  //wait for ACK
            return false;
        }
//			while (I2CMasterBusBusy(I2C0_MASTER_BASE))  { }  //wait for ACK
//			I2CMasterDisable(I2C0_MASTER_BASE); //使能I2C0主机模式
        return true;
    }//end of default
    }//end of switch
		
}



//	设置米字管的管选信号
void I2C0TubeSelSet(char data)
{
    char dataBuf[2] = {PCA9557_REG_OUTPUT, data};
    I2C0Send(I2C0_ADDR_TUBE_SEL, dataBuf, 2);
}
//	点亮米字管的相应码段
void I2C0TubeLowSet(char data)
{
    char dataBuf[2] = {PCA9557_REG_OUTPUT, data};
    I2C0Send(I2C0_ADDR_TUBE_SEG_LOW, dataBuf, 2);
}
void I2C0TubeHighSet(char data)
{
    char dataBuf[2] = {PCA9557_REG_OUTPUT, data};
    I2C0Send(I2C0_ADDR_TUBE_SEG_HIGH, dataBuf, 2);
}


//**********************************************************************
void I2C0PinConfig(void) //配置I2C0模块的IO引脚，使其工作于开漏模式下
{   // Enable GPIO portB containing the I2C pins (PB2&PB3)
//    SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
//                                       SYSCTL_OSC_MAIN |
//                                       SYSCTL_USE_PLL |
//                                       SYSCTL_CFG_VCO_480), 1000000);   //设置系统时间为1MHZ,不能太高，会出问题。
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);
//	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);
    // Configure the PB2 and PB3 pins for I2C operation.
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
}

void I2C0MasterInit(void) //初始化I2C0主机模块
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0); //使能I2C0模块
    // 设I2C 主机模式, 使用系统时钟, 400kbps的快速模式（false普通模式）
    I2CMasterInitExpClk(I2C0_MASTER_BASE,10000000, true);
    I2CMasterEnable(I2C0_MASTER_BASE); //使能I2C0主机模式
}

void I2C0DeviceInit(void)
{
    char dataBuf[2] = {PCA9557_REG_CONFIG, 0x00};
//		I2C0Send(I2C0_ADDR_LED, dataBuf, 2);	//LED光柱
    I2C0Send(I2C0_ADDR_TUBE_SEL, dataBuf, 2);	//U21管选
    I2C0Send(I2C0_ADDR_TUBE_SEG_LOW, dataBuf, 2);  //U22
    I2C0Send(I2C0_ADDR_TUBE_SEG_HIGH, dataBuf, 2);  //U23
}


void PCA9557_Init(void)
{
    I2C0PinConfig();
    I2C0MasterInit();
    I2CSlaveEnable(I2C0_MASTER_BASE);
    I2C0DeviceInit();
}


void setnumber(int index,char value)
{


    switch(value) {
    case '0': {
        a[0]=tubeCodeTable[0][0];
        a[1]=tubeCodeTable[0][1];
        break;
    }
    case '1': {
        a[0]=tubeCodeTable[1][0];
        a[1]=tubeCodeTable[1][1];
        break;
    }
    case '2': {
        a[0]=tubeCodeTable[2][0];
        a[1]=tubeCodeTable[2][1];
        break;
    }
    case '3': {
        a[0]=tubeCodeTable[3][0];
        a[1]=tubeCodeTable[3][1];
        break;
    }
    case '4': {
        a[0]=tubeCodeTable[4][0];
        a[1]=tubeCodeTable[4][1];
        break;
    }
    case '5': {
        a[0]=tubeCodeTable[5][0];
        a[1]=tubeCodeTable[5][1];
        break;
    }
    case '6': {
        a[0]=tubeCodeTable[6][0];
        a[1]=tubeCodeTable[6][1];
        break;
    }
    case '7': {
        a[0]=tubeCodeTable[7][0];
        a[1]=tubeCodeTable[7][1];
        break;
    }
    case '8': {
        a[0]=tubeCodeTable[8][0];
        a[1]=tubeCodeTable[8][1];
        break;
    }
    case '9': {
        a[0]=tubeCodeTable[9][0];
        a[1]=tubeCodeTable[9][1];
        break;
    }
    case 'A': {
        a[0]=tubeCodeTable[10][0];
        a[1]=tubeCodeTable[10][1];
        break;
    }

    case 'B': {
        a[0]=tubeCodeTable[11][0];
        a[1]=tubeCodeTable[11][1];
        break;
    }
    case 'C': {
        a[0]=tubeCodeTable[12][0];
        a[1]=tubeCodeTable[12][1];
        break;
    }
    case 'D': {
        a[0]=tubeCodeTable[13][0];
        a[1]=tubeCodeTable[13][1];
        break;
    }
    case 'E': {
        a[0]=tubeCodeTable[14][0];
        a[1]=tubeCodeTable[14][1];
        break;
    }
    case 'F': {
        a[0]=tubeCodeTable[15][0];
        a[1]=tubeCodeTable[15][1];
        break;
    }
    case ' ': {
        a[0]=0x10;
        a[1]=0x3E;
        break;
    }

    }

    switch(index)
    {
    case(1): {
        I2C0TubeSelSet(~0x20);
        I2C0TubeSelSet(~0x20);
        break;
    }
    case(2): {
        I2C0TubeSelSet(~0x02);
        I2C0TubeSelSet(~0x02);
        break;
    }
    case(3): {
        I2C0TubeSelSet(~0x04);
        I2C0TubeSelSet(~0x04);
        break;
    }
    case(4): {
        I2C0TubeSelSet(~0x08);
        I2C0TubeSelSet(~0x08);
        break;
    }
    default:
        break;

    }


    I2C0TubeLowSet(a[0]);
    I2C0TubeHighSet(a[1]);
}


void PCA9557_Show_Voltage(char  s[])
{
    int k = 10000;
    setnumber(1,s[0]);
    SysCtlDelay(k);

    setnumber(2,s[1]);
    SysCtlDelay(k);

    setnumber(3,s[2]);
    SysCtlDelay(k);

    setnumber(4,s[3]);
    SysCtlDelay(k);

}


