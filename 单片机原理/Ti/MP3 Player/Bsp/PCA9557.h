#ifndef PCA9557_H
#define PCA9557_H


#define bool _Bool
#define I2C0_MASTER_BASE 0x40020000
#define I2C0_SLAVE_BASE 0x40020000

//U21控制4个米字管和特殊管脚的亮灭
#define I2C0_ADDR_TUBE_SEL	      0x18  //00110000
//U22控制米字管7~14管脚对应的码段
#define I2C0_ADDR_TUBE_SEG_LOW    0x19  //00110010
//U23控制米字管15~18管脚对应的码段
#define I2C0_ADDR_TUBE_SEG_HIGH  0x1a //00110100
//U24控制LED光柱

//PCA9557内部寄存器，也称子地址
#define PCA9557_REG_INPUT	 0x00
#define PCA9557_REG_OUTPUT	 0x01
#define PCA9557_REG_PolInver 0x02
#define PCA9557_REG_CONFIG	 0x03



static const char tubeCodeTable[16][2]=
{   //  SegmLow, SegHigh
    {	0x10,	0x3E	},		// 	    	0
    {	0x00,	0x18	},		// 	      1
    {	0x70,	0x2C	},		//      	2
    {	0x70,	0x26	},		//       	3
    {	0x60,	0x32	},		//      	4
    {	0x70,	0x16	},		//     		5
    {	0x70,	0x1E	},		//     		6
    {	0x00,	0x26	},		//     		7
    {	0x70,	0x3E	},		//     		8
    {	0x70,	0x36	},	  //        9
    {	0x60,	0x3E	},	  //     		A
    {	0x70,	0x3E	},		//     		B
    {	0x10,	0x1C	},		//     		C
    {	0x10,	0x3E	},		//     		D
    {	0x70,	0x1C	},		//     		E
    {	0x60,	0x1C	},		//     		F

};




bool I2C0Send(unsigned char slaveAddr, char *data, unsigned char size);
void I2C0TubeSelSet(char data);
void I2C0TubeLowSet(char data);
void I2C0TubeHighSet(char data);
void PCA9557_Init(void);
void PCA9557_Test(void);
void PCA9557_Show_Voltage(char * s);
void setnumber(int index,char value);
#endif
