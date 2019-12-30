#include "i2c.h"

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
#include "utils/uartstdio.h"


void _NOP()
{
	for(int i=0;i<20;i++);
}


//**************************************
//延时5个指令周期
//不同的工作环境,需要调整此函数，注意时钟过快时需要修改
//**************************************
void Delay5us()
{
    _NOP();_NOP();_NOP();_NOP();_NOP();
}

//**************************************

//IIC 接受数据临时缓冲区
 char I2C_RECV_DATA[] =
				{
					0x00,
					0x00,
					0x00,
					0x00,
					0x00,
					0x00
				};

/*******************************************
		拉高 SDA 信号
********************************************/
void I2C_Set_sda_high( void )
{
	GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_4,GPIO_PIN_4); //拉高PE4
    _NOP();
    _NOP();
    return;
}

/*******************************************
		拉低SDA 信号
********************************************/
void I2C_Set_sda_low ( void )
{
	GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_4,0X00000000); //拉低PE4
    _NOP();
    _NOP();
    return;
}

/*******************************************
		拉高SCL 信号
********************************************/
void I2C_Set_scl_high( void )
{
	GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_5,GPIO_PIN_5); //拉高PB5
    _NOP();
    _NOP();
    return;
}

/*******************************************
		拉低SCL 信号
********************************************/
void I2C_Set_scl_low ( void )
{
	GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_5,0X00000000); //拉低PB5
    _NOP();
    _NOP();
    return;
}

/*******************************************
		IIC 信号结束信号函数
********************************************/
void I2C_STOP(void)
{
    int i;
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4); //配置PE4为输出口
    I2C_Set_sda_low();
    for(i = NUM;i > 0;i--);
    I2C_Set_scl_low();
    for(i = NUM;i > 0;i--);
    I2C_Set_scl_high();
    for(i = NUM;i > 0;i--);
    I2C_Set_sda_high();
    for(i = NUM+1;i > 0;i--);
    return;
}

/*******************************************
		IIC 信号初始化
********************************************/
void I2C_Initial( void )
{
    I2C_Set_scl_low();
    I2C_STOP();
    return;
}

/*******************************************
		IIC 信号起始信号函数
********************************************/
void I2C_START(void)
{
    int i;
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4); //配置PE4为输出口
    I2C_Set_sda_high();
    for(i = NUM;i > 0;i--);
    I2C_Set_scl_high();
    for(i = NUM;i > 0;i--);
    I2C_Set_sda_low();
    for(i = NUM;i > 0;i--);
    I2C_Set_scl_low();
    return;
}

/*******************************************
		IIC 获取应答函数
********************************************/
int  I2C_GetACK(void)
{
    int j;
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4); //配置PE4为输出口
    _NOP();
    _NOP();
    I2C_Set_scl_low();
    for(j = NUM;j> 0;j--);
    I2C_Set_scl_high();
    for(j = NUM;j> 0;j--);
    I2C_Set_sda_low();
    for(j = NUM;j > 0;j--);
    I2C_Set_scl_low();
    return 1;
}

/*******************************************
		IIC 设置应答函数
********************************************/
void I2C_SetNAk(void)
{
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4); //配置PE4为输出口
    I2C_Set_scl_low();
    I2C_Set_sda_high();
    I2C_Set_scl_high();
    I2C_Set_scl_low();
    return;
}

/*******************************************
		IIC 发送字节函数
		参数 	1：要发送字节值
		return ：无返回
********************************************/
void I2C_TxByte(unsigned char nValue)
{
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4);//配置PE4为输出口
    int i;
    int j;
    for(i = 0;i < 8;i++)
    {
    	if(nValue & 0x80) //逐位判断，1拉高SDA，0拉低SDA
    	I2C_Set_sda_high();
    	else
    	I2C_Set_sda_low();
    	for(j = NUM;j > 0;j--);
    	I2C_Set_scl_high();
    	nValue <<= 1;    //左移1位
    	for(j = NUM;j > 0;j--);
    	I2C_Set_scl_low();
    }

    return;
}

/*******************************************
		IIC 接收字节函数
		参数 		无
		return ：无返回
********************************************/
unsigned char  I2C_RxByte(void)
{
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4);//配置PE4为输入口
     char nTemp=0 ;
    int i;

    I2C_Set_sda_high();
    _NOP();
    _NOP();
    _NOP();
    _NOP();
    for(i = 0;i < 8;i++)
    {
    	I2C_Set_scl_high(); //模拟SCL信号
    	Delay5us();
    	if(GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_4) == 0x10) //依次判断PE4（SDA）输入
     	{
    	    nTemp |= (0x01 << (7-i));  //8位SDA数据哪一位为高就置1
    	}
    	I2C_Set_scl_low();
    	Delay5us();
    }
    return nTemp;
}

/*******************************************
		IIC 发送数组函数
	参数  	1 num : 发送字节数
	    2 device_addr : iic目标地址
	    3 *data	：发送数组地址
	return ：无返回
********************************************/
void i2c_write(int num, unsigned char device_addr, char *data)
{
    int i = 0;
    int count = num;
     char *send_data = data;
    unsigned char write_addr = device_addr;

    I2C_Set_scl_high();
    for(i = NUM;i > 0;i--);
    I2C_Set_sda_high();
    for(i = NUM;i > 0;i--);

    for(i = 0;i < count;i++)
    {
      I2C_START();           //模拟I2C写数据的时序
      I2C_TxByte(write_addr);
      I2C_GetACK();
      I2C_TxByte(send_data[i]);
      I2C_GetACK();
      i++;
      I2C_TxByte(send_data[i]);
      I2C_GetACK();
      I2C_STOP();
      Delay5us();
    }

}

/*******************************************
		IIC 读取数组函数
	参数  	1 num : 发送字节数
	    2 device_addr : iic目标地址
	    3 *data	：接收数组地址
	return ：无返回
********************************************/
void i2c_read(int num, unsigned char device_addr, char *data)
{
    int i = 0;
    int count = num;
     char *send_data = data;
    unsigned char read_addr = device_addr;

    I2C_Set_scl_high();
    for(i = NUM;i > 0;i--);
    I2C_Set_sda_high();
    for(i = NUM;i > 0;i--);

    for(i = 0; i < count;i++)
    {
      I2C_START();        //模拟I2C读数据
      I2C_TxByte((read_addr - 1));
      I2C_GetACK();

      I2C_TxByte(send_data[i]);
      I2C_GetACK();

      I2C_START();
      I2C_TxByte(read_addr);
      I2C_GetACK();

      I2C_RECV_DATA[i] = I2C_RxByte();
      I2C_SetNAk();
      I2C_STOP();
      Delay5us();
    }
}

//************单读一个字节数据****************

unsigned char SINGLE_read(unsigned char device_addr,char data)
{
    int i = 0;
		 char value;
		 char send_data = data;
    unsigned char read_addr = device_addr;

    I2C_Set_scl_high();
    for(i = NUM;i > 0;i--);
    I2C_Set_sda_high();
    for(i = NUM;i > 0;i--);

    I2C_START();
    I2C_TxByte((read_addr - 1));
    I2C_GetACK();

    I2C_TxByte(send_data);
    I2C_GetACK();

    I2C_START();
    I2C_TxByte(read_addr);
    I2C_GetACK();

    value = I2C_RxByte();
    I2C_SetNAk();
    I2C_STOP();
    Delay5us();
    return value;
}

//**********************************************************************
//GPIO口初始化
//**********************************************************************
void I2C0GPIOBEnable(void) //配置I2C0模块的IO引脚，
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);   //使能  GPIO portB
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);   //使能  GPIO portE

	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_5|GPIO_PIN_4);//使能PB4、PB5作为输出口
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_5);//使能PE5作为输出口
	GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,GPIO_PIN_4);//PB4口置高，即ADXL345L的CS引脚
	GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,GPIO_PIN_5);//PE5口置高，即ADXL345L的ALT ADDRESS引脚
}
