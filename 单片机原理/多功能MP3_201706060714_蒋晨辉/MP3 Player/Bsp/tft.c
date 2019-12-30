#include "tft.h"

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/epi.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/rom.h"
#include "grlib/grlib.h"
#include "inc/tm4c1294ncpdt.h"
#include "driverlib/fpu.h"
#include "tft.h"

#include "EPIinit.h"
#include "IQmath/IQmathLib.h"
#include "usart.h"
#include "uartstdio.h"
#include "font.h"


static volatile unsigned long g_ulTickCount;
extern uint32_t SysClock;
static void WriteDataGPIO(uint16_t ui16Data);
static void WriteCommandGPIO(uint16_t ui16Data);
static tSeries g_cSeries[240];
pfnWriteData WriteData = WriteDataGPIO;
pfnWriteCommand WriteCommand = WriteCommandGPIO;


void
SysTickHandler(void)
{

    g_ulTickCount++;
    if(g_ulTickCount >= 320)
        g_ulTickCount = 0;
}


static void
WriteDataGPIO(uint16_t ui16Data)
{
    //
    // Write the most significant byte of the data to the bus.
    //
    TFTLCD_ADDR = 0;

    HWREG(LCD_RS_PIN_REG) = 0xFF;
    //SysCtlDelay(1);
    //
    // Pull CS Low.
    //
    HWREG(LCD_CS_PIN_REG) = 0;
    //
    // Assert the write enable signal.
    // Delay for at least 60nS (at 120 Mhz) to meet Display timing.
    //
    HWREG(LCD_WR_PIN_REG) = 0;
    //
    // Write the least significant byte of the data to the bus.
    //
    TFTLCD_ADDR = ui16Data;

    //SysCtlDelay(3);
    //
    // Deassert the write enable signal.
    //
    HWREG(LCD_WR_PIN_REG) = 0xFF;

    //
    // Deassert the chip select pin.
    //
    HWREG(LCD_CS_PIN_REG) = 0xFF;
    //SysCtlDelay(3);
}

static void
WriteCommandGPIO(uint16_t ui16Data)
{
    //
    // Write the most significant byte of the data to the bus. This is always
    // 0 since commands are no more than 8 bits currently.
    //

    //
    // Pull RS Low.
    //
    HWREG(LCD_RS_PIN_REG) = 0;
    SysCtlDelay(1);
    //
    // Pull CS Low.
    //
    HWREG(LCD_CS_PIN_REG) = 0;
    //
    // Pull RD High.
    //

    // Assert the write enable signal.
    // Delay for at least 60nS (at 120 Mhz) to meet Display timing.
    //
    HWREG(LCD_WR_PIN_REG) = 0;

    //
    // Write the least significant byte of the data to the bus.
    //
    TFTLCD_ADDR = ui16Data;

    SysCtlDelay(3);
    //
    // Deassert the write enable signals.  Make sure we add padding here
    // too since some compilers inline this function.
    //
    HWREG(LCD_WR_PIN_REG) = 0xFF;

    HWREG(LCD_CS_PIN_REG) = 0xFF;
    SysCtlDelay(3);
}

void
InitGPIOLCDInterface(uint32_t ui32ClockMS)
{
    //
    // Configure the pins that connect to the LCD as GPIO outputs.
    //
    SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOB);//
    SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOC);//
    SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOE);//
    SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOF);//
    SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOK);//
    SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOM);//
    SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPION);//
    SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOP);//
    SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOL);//

    GPIOPinTypeGPIOOutput(LCD_CS_BASE, LCD_CS_PIN);
    GPIOPinTypeGPIOOutput(LCD_RESET_BASE, LCD_RESET_PIN);
    GPIOPinTypeGPIOOutput(LCD_RS_BASE, LCD_RS_PIN);
    GPIOPinTypeGPIOOutput(LCD_IM0_BASE, LCD_IM0_PIN);
    GPIOPinTypeGPIOOutput(LCD_WR_BASE, LCD_WR_PIN);
    GPIOPinTypeGPIOOutput(LCD_RD_BASE, LCD_RD_PIN);

    //
    // Set the LCD control pins to their default values.
    //
    GPIOPinWrite(LCD_CS_BASE, LCD_CS_PIN, LCD_CS_PIN);
    GPIOPinWrite(LCD_RS_BASE, LCD_RS_PIN, 0x00);
    GPIOPinWrite(LCD_RD_BASE, LCD_RD_PIN, LCD_RD_PIN);
    GPIOPinWrite(LCD_WR_BASE, LCD_WR_PIN, LCD_WR_PIN);
    GPIOPinWrite(LCD_CS_BASE, LCD_CS_PIN, 0);
    GPIOPinWrite(LCD_IM0_BASE, LCD_IM0_PIN, 0x00);
    //
    // Delay for 1ms.
    //
    SysCtlDelay(ui32ClockMS);

    //
    // Deassert the LCD reset signal.
    //
    GPIOPinWrite(LCD_RESET_BASE, LCD_RESET_PIN, LCD_RESET_PIN);

    //
    // Delay for 1ms while the LCD comes out of reset.
    //
    SysCtlDelay(ui32ClockMS);
}


void TFTLCD_EnterSleep()
{

    WriteCommand(0x07F0);
    WriteData(0x5420);
    WriteCommand(0x07DE);
    WriteData(0x0C00);
    WriteCommand(0x07F0);
    WriteData(0x0000);
    WriteCommand(0x0007);
    WriteData(0x0000);
    SysCtlDelay(200000);
    WriteCommand(0x0100);
    WriteData(0x0002);
    SysCtlDelay(200000);
}

void TFTLCD_ExitSleep()
{

    WriteCommand(0x07F0);
    WriteData(0x5420);
    WriteCommand(0x07DE);
    WriteData(0x0400);
    WriteCommand(0x07F0);
    WriteData(0x0000);
    WriteCommand(0x0100);
    WriteData(0x11B0);
    SysCtlDelay(200000);
    WriteCommand(0x0007);
    WriteData(0x0173);
    SysCtlDelay(200000);

}

void

TFT_Init()
{

    EPIGPIOinit();
    uint32_t ui32ClockMS;

    uint32_t ui32SysClockSpeed = SysClock;

    ui32ClockMS = ui32SysClockSpeed / (3 * 1000);

    SysCtlPeripheralEnable(LCD_RS_PERIPH);
    SysCtlPeripheralEnable(LCD_RD_PERIPH);
    SysCtlPeripheralEnable(LCD_WR_PERIPH);
    SysCtlPeripheralEnable(LCD_CS_PERIPH);
    SysCtlPeripheralEnable(LCD_RESET_PERIPH);
    SysCtlPeripheralEnable(LCD_IM0_PERIPH);

    {
        InitGPIOLCDInterface(ui32ClockMS);
    }

    HWREG(LCD_RESET_PIN_REG) = 0xFF;
    SysCtlDelay(200000);   //Under 120MHz system clock, SysCtlDelay(1000) is about 25us, SysCtlDelay(1) for about 25ns.
    HWREG(LCD_RESET_PIN_REG) = 0;
    SysCtlDelay(400000);
    HWREG(LCD_RESET_PIN_REG) = 0xFF;
    SysCtlDelay(4000000);
    // sleep
    //    TFTLCD_EnterSleep();
    //======== Power ON sequence============//
    WriteCommand(0x0606);
    WriteData(0x0000);//Set the order of receiving data when using i80 interface.1st to 2nd
    SysCtlDelay(4000000);
    WriteCommand(0x0007);
    WriteData(0x0001);//Display Control 1
    SysCtlDelay(200000);
    WriteCommand(0x0110);
    WriteData(0x0001);
    SysCtlDelay(200000);
    WriteCommand(0x0100);
    WriteData(0x17B0);
    WriteCommand(0x0101);
    WriteData(0x0147);
    WriteCommand(0x0102);
    WriteData(0x019D);
    WriteCommand(0x0103);
    WriteData(0x3600);
    WriteCommand(0x0281);
    WriteData(0x0010);
    SysCtlDelay(200000);
    WriteCommand(0x0102);
    WriteData(0x01BD);
    SysCtlDelay(200000);
//==========================================//
    WriteCommand(0x0000);
    WriteData(0x0000);
    WriteCommand(0x0001);
    WriteData(0x0100);

    WriteCommand(0x0002);
    WriteData(0x0100);
    WriteCommand(0x0003);
    WriteData(0x00A0);

    WriteCommand(0x0006);
    WriteData(0x0000);
    WriteCommand(0x0008);
    WriteData(0x0202);//Display Control 2

    WriteCommand(0x0009);
    WriteData(0x0001);//Display Control 3
    WriteCommand(0x000B);
    WriteData(0x0000);//Sam test

    WriteCommand(0x000C);
    WriteData(0x0000);//External
//		Display Interface Control 1
    //RM = 0,System interface/VSYNC interface
    WriteCommand(0x000F);
    WriteData(0x0000);//External Display Interface Control 2
    WriteCommand(0x0007);
    WriteData(0x0001);

    //Panel Interface Control 1
    WriteCommand(0x0010);
    WriteData(0x0013);//Set the clock cycle per line  19

    WriteCommand(0x0011);
    WriteData(0x0202);// NOWI [2:0]: Set the adjacent gate driver output non-overlap

    WriteCommand(0x0012);
    WriteData(0x0300);
    WriteCommand(0x0020);
    WriteData(0x021E);
    WriteCommand(0x0021);
    WriteData(0x0202);

    WriteCommand(0x0022);
    WriteData(0x0100);
    WriteCommand(0x0090);
    WriteData(0x8000);//Set the position of the frame marker. 0 < FMP < BP + NL + FP
    WriteCommand(0x0092);
    WriteData(0x8000);
    WriteCommand(0x0100);
    WriteData(0x16B0);
    SysCtlDelay(200000);
    WriteCommand(0x0101);
    WriteData(0x0147);
    WriteCommand(0x0102);
    WriteData(0x01BE);
    SysCtlDelay(200000);
    WriteCommand(0x0103);
    WriteData(0x1600);
    SysCtlDelay(200000);
    WriteCommand(0x0107);
    WriteData(0x0000);
    SysCtlDelay(200000);
    WriteCommand(0x0110);
    WriteData(0x0001);
    SysCtlDelay(200000);
    WriteCommand(0x0210);
    WriteData(0x0000);
    WriteCommand(0x0211);
    WriteData(0x00EF);
    WriteCommand(0x0212);
    WriteData(0x0000);
    WriteCommand(0x0213);
    WriteData(0x018F);
    WriteCommand(0x0200);
    WriteData(0x0000);//tb

    WriteCommand(0x0201);
    WriteData(0x0000);//tb
    WriteCommand(0x0280);
    WriteData(0x0000);
    WriteCommand(0x0281);
    WriteData(0x0007);

    WriteCommand(0x0282);
    WriteData(0x0000);
    SysCtlDelay(200000);
    WriteCommand(0x0300);
    WriteData(0x0002);
    WriteCommand(0x0301);
    WriteData(0x081D);
    WriteCommand(0x0302);
    WriteData(0x1621);
    WriteCommand(0x0303);
    WriteData(0x3D12);
    WriteCommand(0x0304);
    WriteData(0x3C0B);
    WriteCommand(0x0305);
    WriteData(0x1004);
    WriteCommand(0x0306);
    WriteData(0x0A06);
    WriteCommand(0x0307);
    WriteData(0x0612);
    WriteCommand(0x0308);
    WriteData(0x0105);
    WriteCommand(0x0309);
    WriteData(0x0004);
    WriteCommand(0x030A);
    WriteData(0x0F05);
    WriteCommand(0x030B);
    WriteData(0x0F00);
    WriteCommand(0x030C);
    WriteData(0x000F);
    WriteCommand(0x030D);
    WriteData(0x050F);
    WriteCommand(0x030E);
    WriteData(0x0106);
    WriteCommand(0x030F);
    WriteData(0x0406);

    WriteCommand(0x0400);
    WriteData(0x3500);
    WriteCommand(0x0401);
    WriteData(0x0001);
    WriteCommand(0x0404);
    WriteData(0x0000);
    WriteCommand(0x0500);
    WriteData(0x0000);
    WriteCommand(0x0501);
    WriteData(0x0000);
    WriteCommand(0x0502);
    WriteData(0x0000);
    WriteCommand(0x0503);
    WriteData(0x0000);
    WriteCommand(0x0504);
    WriteData(0x0000);
    WriteCommand(0x0505);
    WriteData(0x0000);
    WriteCommand(0x0600);
    WriteData(0x0000);
    WriteCommand(0x0606);
    WriteData(0x0000);
    WriteCommand(0x06F0);
    WriteData(0x0000);
    WriteCommand(0x07F0);
    WriteData(0x5420);

    WriteCommand(0x07F2);
    WriteData(0x00DF);
    WriteCommand(0x07F3);
    WriteData(0x288A);
    WriteCommand(0x07F4);
    WriteData(0x0022);
    WriteCommand(0x07F5);
    WriteData(0x0041);
    WriteCommand(0x07F0);
    WriteData(0x0000);
    WriteCommand(0x0007);
    WriteData(0x0173);
    SysCtlDelay(200000);
    WriteCommand(0x0007);
    WriteData(0x0061);
    SysCtlDelay(200000);
    WriteCommand(0x0007);
    WriteData(0x0173);
    SysCtlDelay(2000000);
    WriteCommand(0x0202);


    TFT_CLEAR(0);

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);
}

void TFT_REFRESH_GRAM(uint16_t ui16Data)
{
    WriteData(ui16Data);
}

//================================================================================================
//	实现功能：	设置坐标
//  输入参数：  x0，y0 起始坐标
//         x1，y1 结束坐标
//================================================================================================
void TFT_SetPos(uint32_t x0, uint32_t x1, uint32_t y0, uint32_t y1)
{
    WriteCommand(0x0210);
    WriteData(x0);
    WriteCommand(0x0211);
    WriteData(x1);
    WriteCommand(0x0212);
    WriteData(y0);
    WriteCommand(0x0213);
    WriteData(y1);
    WriteCommand(0x0200);
    WriteData(x0);
    WriteCommand(0x0201);
    WriteData(y0);
    WriteCommand(0x0202);
}

//================================================================================================
//	实现功能：	画点
//  输入参数：  x，y 需要画点坐标
//         color 点的颜色
//================================================================================================
void TFT_DrawPoint(unsigned int x, unsigned int y, unsigned int color)
{
    TFT_SetPos(x, x, y, y);     //设置待画点坐标
    WriteData(color);         //在指定点写入颜色数据
}


//================================================================================================
//	实现功能：	显示Ascii字符
//  输入参数：  x 横坐标
//              y 纵坐标
//		        c 需要显示的字符
//		        fColor 字符颜色
//		        bColor 字符背景颜色
//================================================================================================
void TFT_ShowChar(uint32_t x, uint32_t y, uint32_t chr, uint32_t fColor, uint32_t bColor)
{
    uint32_t i, j;
    uint8_t m;

    chr = chr - ' '; //得到偏移后的值
    TFT_SetPos(x, x + 8 - 1, y, y + 16 - 1);         //设置字符显示位置
    for(i = 0; i < 16; i++) {                        //循环写入16字节，一个字符为16字节
        m = asc2_1608[chr][i]; //提取c字符的第i个字节
        for(j = 0; j < 8; j++) {                  //循环写入8位，一个字节为8位
            if((m & 0x80) == 0x80) {              //判断最高位是否为1
                WriteData(fColor);           //最高位为1，写入字符颜色
            }
            else {
                WriteData(bColor);           //最高位为0，写入背景颜色
                ;
            }
            m <<= 1;                              //左移1位，准备写下一位
        }
    }
}



//显示字符串
//x,y:起点坐标
//*p:字符串起始地址
//用16字体
#define MAX_CHAR_POSX 230
#define MAX_CHAR_POSY 390
void TFT_ShowString(uint32_t x, uint32_t y, char *p, uint32_t fColor, uint32_t bColor)
{

    while(*p != '\0')
    {
        if(x > MAX_CHAR_POSX) {
            x = 0;
            y += 16;
        }
        if(y > MAX_CHAR_POSY) {
            y = x = 0;
            TFT_CLEAR(bColor);
        }
        TFT_ShowChar(x, y, *p, fColor, bColor);
        x += 8;
        p++;
    }
}

//================================================================================================
//	实现功能：	TFT清屏
//	输入参数：	bColor 清屏所使用的背景色
//================================================================================================
void TFT_CLEAR(uint32_t bColor)
{
    uint32_t i, j;
    TFT_SetPos(0, 240, 0, 400); //400x240
    for (i = 0; i < 400; i++)
    {
        for (j = 0; j < 240; j++)
            WriteData(bColor);
    }
}

//================================================================================================
//	实现功能：	用指定颜色填充指定区域
//	输入参数：	x0:横坐标起点,y0:纵坐标起点,x1:横坐标终点,y1:纵坐标终点,color:指定颜色
//================================================================================================
void TFT_FillBlock(uint32_t x0, uint32_t x1, uint32_t y0, uint32_t y1, uint32_t Color)
{
    uint32_t i, j;
    TFT_SetPos(x0, x1, y0, y1);
    for (i = y0; i < y1; i++)
    {
        for (j = x0; j < x1; j++)
            WriteData(Color);
    }


}

//================================================================================================
//  实现功能：显示一个变量数据 i32Data  -9999<=i32Data<=9999
//  输入参数：变量数字i32Data，    x,y:起点坐标       ,color:指定颜色
//================================================================================================
void TFT_ShowData(uint32_t x, uint32_t y, int32_t i32Data, uint32_t fColor, uint32_t bColor)
{
    char Data[6];
    if(i32Data > 9999 || i32Data < -9999)
    {
        TFT_ShowString(x, y, "Error:Out of range!", RED, WHITE);
        return;
    }
    if(i32Data < 0)
    {
        Data[0] = '-';
        i32Data = ~i32Data + 1;
    }
    else
    {
        Data[0] = ' ';
    }
    Data[1] = i32Data / 1000;
    Data[2] = (i32Data - Data[1] * 1000) / 100;
    Data[3] = (i32Data - Data[1] * 1000 - Data[2] * 100) / 10;
    Data[4] = (i32Data - Data[1] * 1000 - Data[2] * 100 - Data[3] * 10);
    Data[1] += '0';
    Data[2] += '0';
    Data[3] += '0';
    Data[4] += '0';
    Data[5] = '\0';
    TFT_ShowString(x, y, Data, fColor, bColor);
    ;
}


//================================================================================================
//	实现功能：	画一条水平线
//	输入参数：	x0:横坐标起点,x1:横坐标终点,y:纵坐标,color:指定颜色
//================================================================================================
void TFT_DrawHorizontalLine(uint32_t x0, uint32_t x1, uint32_t y, uint32_t color)
{
    uint32_t i = 0, PointNum = 0;
    PointNum = x1 - x0;
    for(i = 0; i < PointNum; i++)
    {
        TFT_DrawPoint(x0 + i, y, color);
    }

}


//================================================================================================
//	实现功能：	画一条垂直线
//	输入参数：	y0:纵坐标起点,y1:纵坐标终点,x:横坐标,color:指定颜色
//================================================================================================
void TFT_DrawVerticalLine(uint32_t y0, uint32_t y1, uint32_t x, uint32_t color)
{
    uint32_t i = 0, PointNum = 0;
    PointNum = y1 - y0;
    for(i = 0; i < PointNum; i++)
    {
        TFT_DrawPoint(x, y0 + i, color);
    }
}


//================================================================================================
//	实现功能：	画一个⚪
//	输入参数：	x,y:圆心坐标，radius:半径，color：填充颜色
//================================================================================================
void TFT_DrawCircle(uint32_t x, uint32_t y, uint8_t radius, uint32_t color)
{
    int a, b, num;
    a = 0;
    b = radius;
    while(2 * b * b >= radius * radius)
    {
        TFT_DrawPoint(x + a, y - b, color);
        TFT_DrawPoint(x - a, y - b, color);
        TFT_DrawPoint(x - a, y + b, color);
        TFT_DrawPoint(x + a, y + b, color);

        TFT_DrawPoint(x + b, y + a, color);
        TFT_DrawPoint(x + b, y - a, color);
        TFT_DrawPoint(x - b, y - a, color);
        TFT_DrawPoint(x - b, y + a, color);

        a++;
        num = (a * a + b * b) - radius * radius;
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}

void TFT_DrawRectangle(uint32_t x,uint32_t y,uint32_t width,uint32_t height,uint32_t color)
{
	TFT_DrawLine(x,y,x+width,y,color);
	TFT_DrawLine(x+width,y,x+width,y+height,color);
	TFT_DrawLine(x+width,y+height,x,y+height,color);
	TFT_DrawLine(x,y+height,x,y,color);
	TFT_FillBlock(x,x+width,y,y+height+1,color);
}


//显示一条直线
void TFT_DrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color)
{
    int dx, dy, e;
    dx = x2 - x1;
    dy = y2 - y1;
    if (dx >= 0)
    {
        if (dy >= 0) // dy>=0
        {
            if (dx >= dy) // 1/8 octant
            {
                e = dy - dx / 2;
                while (x1 <= x2)
                {
                    TFT_DrawPoint(x1, y1, color);
                    if (e > 0)
                    {
                        y1 += 1;
                        e -= dx;
                    }
                    x1 += 1;
                    e += dy;
                }
            }
            else // 2/8 octant
            {
                e = dx - dy / 2;
                while (y1 <= y2)
                {
                    TFT_DrawPoint(x1, y1, color);
                    if (e > 0)
                    {
                        x1 += 1;
                        e -= dy;
                    }
                    y1 += 1;
                    e += dx;
                }
            }
        }
        else // dy<0
        {
            dy = -dy;     // dy=abs(dy)
            if (dx >= dy) // 8/8 octant
            {
                e = dy - dx / 2;
                while (x1 <= x2)
                {
                    TFT_DrawPoint(x1, y1, color);
                    if (e > 0)
                    {
                        y1 -= 1;
                        e -= dx;
                    }
                    x1 += 1;
                    e += dy;
                }
            }
            else // 7/8 octant
            {
                e = dx - dy / 2;
                while (y1 >= y2)
                {
                    TFT_DrawPoint(x1, y1, color);
                    if (e > 0)
                    {
                        x1 += 1;
                        e -= dy;
                    }
                    y1 -= 1;
                    e += dx;
                }
            }
        }
    }
    else //dx<0
    {
        dx = -dx;    //dx=abs(dx)
        if (dy >= 0) // dy>=0
        {
            if (dx >= dy) // 4/8 octant
            {
                e = dy - dx / 2;
                while (x1 >= x2)
                {
                    TFT_DrawPoint(x1, y1, color);
                    if (e > 0)
                    {
                        y1 += 1;
                        e -= dx;
                    }
                    x1 -= 1;
                    e += dy;
                }
            }
            else // 3/8 octant
            {
                e = dx - dy / 2;
                while (y1 <= y2)
                {
                    TFT_DrawPoint(x1, y1, color);
                    if (e > 0)
                    {
                        x1 -= 1;
                        e -= dy;
                    }
                    y1 += 1;
                    e += dx;
                }
            }
        }
        else // dy<0
        {
            dy = -dy;     // dy=abs(dy)
            if (dx >= dy) // 5/8 octant
            {
                e = dy - dx / 2;
                while (x1 >= x2)
                {
                    TFT_DrawPoint(x1, y1, color);
                    if (e > 0)
                    {
                        y1 -= 1;
                        e -= dx;
                    }
                    x1 -= 1;
                    e += dy;
                }
            }
            else // 6/8 octant
            {
                e = dx - dy / 2;
                while (y1 >= y2)
                {
                    TFT_DrawPoint(x1, y1, color);
                    if (e > 0)
                    {
                        x1 -= 1;
                        e -= dy;
                    }
                    y1 -= 1;
                    e += dx;
                }
            }
        }
    }
}


//显示一副图片
void TFT_ShowPicture(uint32_t x,uint32_t y, uint32_t width,uint32_t height, uint8_t* gImage)
{

	TFT_SetPos(x,x+width-1,y,y+height-1);
	for(int i=0;i<height*width;i++)
	{
		WriteData((gImage[2*i]|0x0000)<<8|gImage[2*i+1]);
	}
	

}
void TFT_ShowByte(uint32_t x, uint32_t y, uint8_t  byte, uint32_t color)
{
    for(int i = 0; i < 8; i++)
    {
        if(byte & (0x80 >> i))
            TFT_DrawPoint(x + i, y, color);
    }
}

void TFT_ShowName(uint32_t x, uint32_t y, uint8_t  name, uint32_t color)
{
    for(int i = 0; i < 2; i++)
    {
        uint8_t * temp = kaiguan[2 * name + i];
        for(int j = 0; j < 8; j++)
        {
            TFT_ShowByte(x, y + 8 * i + j, temp[2 * j], color);
            TFT_ShowByte(x + 8, y + 8 * i + j, temp[2 * j + 1], color);
        }
    }
}


void TFT_ShowWheel(void)
{
	TFT_DrawCircle(120,300,100,WHITE);
	TFT_DrawCircle(120,300,40,WHITE);
	//音量加
	TFT_ShowName(92,220,8,WHITE);
	TFT_ShowName(110,220,9,WHITE);
	TFT_ShowName(128,220,10,WHITE);
	//音量减
	TFT_ShowName(92,360,8,WHITE);
	TFT_ShowName(110,360,9,WHITE);
	TFT_ShowName(128,360,11,WHITE);
	//上一首
	TFT_ShowName(26,290,4,WHITE);
	TFT_ShowName(44,290,6,WHITE);
	TFT_ShowName(62,290,7,WHITE);
	//下一首
	TFT_ShowName(164,290,5,WHITE);
	TFT_ShowName(182,290,6,WHITE);
	TFT_ShowName(200,290,7,WHITE);
	//播放/暂停
	TFT_ShowName(84,290,0,WHITE);
	TFT_ShowName(102,290,1,WHITE);
	TFT_ShowChar(118,290,'/',WHITE,BLACK);
	TFT_ShowName(126,290,2,WHITE);
	TFT_ShowName(144,290,3,WHITE);
	TFT_DrawLine(148,271,190,229,WHITE);
	TFT_DrawLine(92,271,50,229,WHITE);
	TFT_DrawLine(148,329,190,371,WHITE);
	TFT_DrawLine(92,329,50,371,WHITE);
}

void num2char(char *str, double number, uint8_t g, uint8_t l)
{
    uint8_t i;
    int temp = number/1;
		static char table[]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    double t2 = 0.0;
    for (i = 1; i<=g; i++)
    {
        if (temp==0)
            str[g-i] = table[0];
        else
            str[g-i] = table[temp%10];
        temp = temp/10;
    }
    *(str+g) = '.';
    temp = 0;
    t2 = number;
    for(i=1; i<=l; i++)
    {
        temp = t2*10;
        str[g+i] = table[temp%10];
        t2 = t2*10;
    }
    *(str+g+l+1) = '\0';
}

