#include "OLED_I2C.h"
#include "codetab.h"
#include "stdint.h"
#include "stdbool.h"
#include "inc/hw_i2c.h"
#include "driverlib/i2c.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"

bool I2C3Send(unsigned char slaveAddr, char *data, unsigned char size)
{
    unsigned char a;
    a=size;

    switch(a) {
    case 0:
        return false;
    case 1: {	//当前数据地址的单字节发送
        //设置控制字节：从机地址，写操作false（读为true）
        I2CMasterSlaveAddrSet (I2C3_BASE, slaveAddr, false);
        I2CMasterDataPut (I2C3_BASE, *data); //设置数据
        I2CMasterControl (I2C3_BASE,I2C_MASTER_CMD_SINGLE_SEND);  //启动”总线单字节发送”
        while(I2CMasterBusy(I2C3_BASE))  { } // wait for ACK
        if (I2CMasterErr(I2C3_BASE) != I2C_MASTER_ERR_NONE)
            return false;
        return true;
    }
    default: {   //非当前地址发送，参数data的第一个字节为子地址其余为数据
        //发送第一个字节（控制字节+子地址字节）
//		I2CMasterEnable(I2C3_BASE); //使能I2C0主机模式
        I2CMasterSlaveAddrSet (I2C3_BASE, slaveAddr, false);
        I2CMasterDataPut (I2C3_BASE,*(data++)); //设置子地址
        I2CMasterControl (I2C3_BASE,I2C_MASTER_CMD_BURST_SEND_START);  //启动”总线发送开始”
        while (I2CMasterBusy(I2C3_BASE))  { }  // wait for ACK
        if (I2CMasterErr(I2C3_BASE) != I2C_MASTER_ERR_NONE)
            return false;
        while (--size > 1) {  //发送中间字节（数据字节）
            I2CMasterDataPut (I2C3_BASE, *(data++)); //设数据
            I2CMasterControl (I2C3_BASE,I2C_MASTER_CMD_BURST_SEND_CONT);  //启动”总线发送继续”
            while (I2CMasterBusy(I2C3_BASE))  { }	 // wait for ACK
            if (I2CMasterErr(I2C3_BASE) != I2C_MASTER_ERR_NONE)
                return false;
        }
        //发送最后一个字节（数据字节）
        I2CMasterDataPut (I2C3_BASE, *data); //设置数据
        I2CMasterControl (I2C3_BASE,I2C_MASTER_CMD_BURST_SEND_FINISH);  //启动”总线发送结束”
        while (I2CMasterBusy(I2C3_BASE))  { }  //wait for ACK
        if (I2CMasterErr(I2C3_BASE) != I2C_MASTER_ERR_NONE)
        {
//				I2CMasterDisable(I2C3_BASE); //使能I2C0主机模式
//				while (I2CMasterBusBusy(I2C3_BASE))  { }  //wait for ACK
            return false;
        }
//			while (I2CMasterBusBusy(I2C3_BASE))  { }  //wait for ACK
//			I2CMasterDisable(I2C3_BASE); //使能I2C0主机模式
        return true;
    }//end of default
    }//end of switch
}


//**********************************************************************
void I2C3PinConfig(void) //配置I2C0模块的IO引脚，使其工作于开漏模式下
{   // Enable GPIO portB containing the I2C pins (PB2&PB3)
//    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
//                                       SYSCTL_OSC_MAIN |
//                                       SYSCTL_USE_PLL |
//                                       SYSCTL_CFG_VCO_480), 1000000);   //设置系统时间为1MHZ,不能太高，会出问题。
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
    GPIOPinTypeI2C(GPIO_PORTK_BASE, GPIO_PIN_4 | GPIO_PIN_5);
//	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinConfigure(GPIO_PK4_I2C3SCL);
    GPIOPinConfigure(GPIO_PK5_I2C3SDA);
    // Configure the PB2 and PB3 pins for I2C operation.
    GPIOPinTypeI2CSCL(GPIO_PORTK_BASE, GPIO_PIN_4);
    GPIOPinTypeI2C(GPIO_PORTK_BASE, GPIO_PIN_5);
}
void I2C3MasterInit(void) //初始化I2C0主机模块
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C3); //使能I2C0模块
    // 设I2C 主机模式, 使用系统时钟, 400kbps的快速模式（false普通模式）
    I2CMasterInitExpClk(I2C3_BASE,10000000, true);
    I2CMasterEnable(I2C3_BASE); //使能I2C0主机模式
}


void WriteCmd(unsigned char I2C_Command)//写命令
{
    char buffer[2] = {0x00,I2C_Command};
		I2C3Send(0x3c,buffer,2);
}

void WriteDat(unsigned char I2C_Data)//写数据
{
	  char buffer[2] = {0x40,I2C_Data};
		I2C3Send(0x3c,buffer,2);
}

void OLED_Init(void)
{
	I2C3PinConfig();
	I2C3MasterInit();
	SysCtlDelay(100*50000000/3000);
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //亮度调节 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
	OLED_CLS();
}

void OLED_SetPos(unsigned char x, unsigned char y) //设置起始点坐标
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

void OLED_Fill(unsigned char fill_Data)//全屏填充
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				WriteDat(fill_Data);
			}
	}
}

void OLED_CLS(void)//清屏
{
	OLED_Fill(0x00);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          : 
// Parameters     : none
// Description    : 将OLED从休眠中唤醒
//--------------------------------------------------------------
void OLED_ON(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X14);  //开启电荷泵
	WriteCmd(0XAF);  //OLED唤醒
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          : 
// Parameters     : none
// Description    : 让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X10);  //关闭电荷泵
	WriteCmd(0XAE);  //OLED休眠
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); ch[] -- 要显示的字符串; TextSize -- 字符大小(1:6*8 ; 2:8*16)
// Description    : 显示codetab.h中的ASCII字符,有6*8和8*16可选择
//--------------------------------------------------------------
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); N:汉字在codetab.h中的索引
// Description    : 显示codetab.h中的汉字,16*16点阵
//--------------------------------------------------------------
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int  adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
// Calls          : 
// Parameters     : x0,y0 -- 起始点坐标(x0:0~127, y0:0~7); x1,y1 -- 起点对角线(结束点)的坐标(x1:1~128,y1:1~8)
// Description    : 显示BMP位图
//--------------------------------------------------------------
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			WriteDat(BMP[j++]);
		}
	}
}
