#include "accelerator.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "inc/hw_i2c.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "uartstdio.h"

#include "i2c.h"

void I2CSend(uint8_t salveAddr,uint8_t registerAddr,char*data,int num)
{
	char buffer[num+1];
	buffer[0]=(char)registerAddr;
	for(int i=0;i<num;i++)
	{
		buffer[i+1]=data[i];
	}
	i2c_write(num+1, salveAddr, buffer);
}

uint8_t I2CRead(uint8_t salveAddr,uint8_t registerAddr)
{
	uint8_t res;
	res =  SINGLE_read(salveAddr,registerAddr);	
	return res;
}

void ACCELERATOR_Init(void)
{
	
	
	char buffer;
	buffer = 0x0B;
	I2CSend(ACCELERATOR_W,ACCELERATOR_DATA_FORMAT,&buffer,1);
	
	buffer = 0x18;
	I2CSend(ACCELERATOR_W,ACCELERATOR_BW_RATE,&buffer,1);
	
	buffer = 0x08;
	I2CSend(ACCELERATOR_W,ACCELERATOR_POWER_CTL,&buffer,1);
	
	buffer = 0x00;
	I2CSend(ACCELERATOR_W,ACCELERATOR_INT_ENABLE,&buffer,1);
	
	buffer = 0xff;
	I2CSend(ACCELERATOR_W,ACCELERATOR_OFSX,&buffer,1);
	buffer = 0x01;
	I2CSend(ACCELERATOR_W,ACCELERATOR_OFSY,&buffer,1);
	buffer = 0x00;
	I2CSend(ACCELERATOR_W,ACCELERATOR_OFSZ,&buffer,1);
	

	uint8_t devid = I2CRead(ACCELERATOR_R,ACCELERATOR_DEVID);
	if(devid == 0xe5)
	{
		//UARTprintf("accelerator init success!\n");
	}
	
}

int ACCELERATOR_Read(char channel)
{
	int res;
	uint8_t register_value[2]={};
	switch(channel)
	{
		case('X'):
		{
			register_value[0] = I2CRead(ACCELERATOR_R,ACCELERATOR_X_L);
			register_value[1] = I2CRead(ACCELERATOR_R,ACCELERATOR_X_H);
			if(register_value[1]>16)
			{
				register_value[1]=0xFF-register_value[1];
				register_value[0]=0xFF-register_value[0];
				res = register_value[0]*4 + register_value[1]*1024;
				res = res*-1;
				break;
			}
			res = register_value[0]*4 + register_value[1]*1024;
			break;
		}
		
			case('Y'):
		{
			register_value[0] = I2CRead(ACCELERATOR_R,ACCELERATOR_Y_L);
			register_value[1] = I2CRead(ACCELERATOR_R,ACCELERATOR_Y_H);
			if(register_value[1]>16)
			{
				register_value[1]=0xFF-register_value[1];
				register_value[0]=0xFF-register_value[0];
				res = register_value[0]*4 + register_value[1]*1024;
				res = res*-1;
				break;
			}
			res = register_value[0]*4 + register_value[1]*1024;
			break;
		}
			case('Z'):
		{
			register_value[0] = I2CRead(ACCELERATOR_R,ACCELERATOR_Z_L);
			register_value[1] = I2CRead(ACCELERATOR_R,ACCELERATOR_Z_H);
			if(register_value[1]>16)
			{
				register_value[1]=0xFF-register_value[1];
				register_value[0]=0xFF-register_value[0];
				res = register_value[0]*4 + register_value[1]*1024;
				res = res*-1;
				break;
			}
			res = register_value[0]*4 + register_value[1]*1024;
			break;
		}
	}
	return res;
}

void ACCELERATOR_Test(void)
{
	int Gx,Gy,Gz;
	int sum=0;
	int f[10];
	while(1)
	{
		for(int i=0;i<10;i++)
		{
			f[i]=ACCELERATOR_Read('X');
		}
		int temp= 0; //中间变量
 
		//冒泡法排序实现从小到大排序
		for(int i=0;i<10;i++) //进行10次循环
		{
			for(int j=i+1;j<10;j++) //循环比较剩余的变量
			{
				if(f[i] > f[j]) //如果前面一个数比后面数大，交换两个数的值
				{
					temp = f[i];
					f[i] = f[j];
					f[j] = temp;
				}
			}
		}
	 
	for(int i=1;i<9;i++) //循环输出排序以后的结果
	{
		sum+=f[i];
	}

		Gx = sum/8;
		sum=0;
		
	
			for(int i=0;i<10;i++)
		{
			f[i]=ACCELERATOR_Read('Y');
		}
		temp= 0; //中间变量
 
		//冒泡法排序实现从小到大排序
		for(int i=0;i<10;i++) //进行10次循环
		{
			for(int j=i+1;j<10;j++) //循环比较剩余的变量
			{
				if(f[i] > f[j]) //如果前面一个数比后面数大，交换两个数的值
				{
					temp = f[i];
					f[i] = f[j];
					f[j] = temp;
				}
			}
		}
	 
	for(int i=1;i<9;i++) //循环输出排序以后的结果
	{
		sum+=f[i];
	}

		Gy = sum/8;
		sum=0;
		
				for(int i=0;i<10;i++)
		{
			f[i]=ACCELERATOR_Read('Z');
		}
		 temp= 0; //中间变量
 
		//冒泡法排序实现从小到大排序
		for(int i=0;i<10;i++) //进行10次循环
		{
			for(int j=i+1;j<10;j++) //循环比较剩余的变量
			{
				if(f[i] > f[j]) //如果前面一个数比后面数大，交换两个数的值
				{
					temp = f[i];
					f[i] = f[j];
					f[j] = temp;
				}
			}
		}
	 
	for(int i=1;i<9;i++) //循环输出排序以后的结果
	{
		sum+=f[i];
	}

		Gz = sum/8;
		sum=0;
		
		UARTprintf("X: %4dmg Y: %4dmg Z: %4dmg\n",Gx,Gy,Gz);
		SysCtlDelay(10000*SysCtlClockGet()/3000);
	}
}