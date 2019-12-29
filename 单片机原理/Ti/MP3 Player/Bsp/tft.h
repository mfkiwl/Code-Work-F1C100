#ifndef TFT_H
#define TFT_H

#include <stdint.h>
#include <stdbool.h>

#define WHITE         	 0xFFFF//白色
#define BLACK         	 0x0000//黑色
#define RED           	 0x001F//红色
#define BLUE         	   0xF800//蓝色
#define GREEN         	 0x07E0//绿色
#define GBLUE			       0X07FF//?
#define PURPLE			     0XF01F //紫色
#define MAGENTA       	 0xF81F//品红色
#define CYAN          	 0x7FFF//蓝绿色
#define YELLOW        	 0xFFE0//黄色
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			     0XC618 //浅灰色(PANNEL)
#define LGRAYBLUE        0XA651 //浅灰蓝色
#define LBBLUE           0X2B12 //浅棕蓝色



#define LCD_CS_PERIPH              SYSCTL_PERIPH_GPIOE
#define LCD_CS_BASE                GPIO_PORTE_BASE
#define LCD_CS_PIN                 GPIO_PIN_1
#define LCD_CS_PIN_REG             0x40024008

#define LCD_RESET_PERIPH           SYSCTL_PERIPH_GPIOF
#define LCD_RESET_BASE             GPIO_PORTF_BASE
#define LCD_RESET_PIN              GPIO_PIN_4
#define LCD_RESET_PIN_REG          0x40025040

#define LCD_RS_PERIPH              SYSCTL_PERIPH_GPIOP
#define LCD_RS_BASE                GPIO_PORTP_BASE
#define LCD_RS_PIN                 GPIO_PIN_5
#define LCD_RS_PIN_REG             0x40065080

#define LCD_IM0_PERIPH             SYSCTL_PERIPH_GPIOL
#define LCD_IM0_BASE               GPIO_PORTL_BASE
#define LCD_IM0_PIN                GPIO_PIN_5
#define LCD_IM0_PIN_REG            0x40062080

#define LCD_WR_PERIPH              SYSCTL_PERIPH_GPIOE
#define LCD_WR_BASE                GPIO_PORTE_BASE
#define LCD_WR_PIN                 GPIO_PIN_2
#define LCD_WR_PIN_REG             0x40024010

#define LCD_RD_PERIPH              SYSCTL_PERIPH_GPIOE
#define LCD_RD_BASE                GPIO_PORTE_BASE
#define LCD_RD_PIN                 GPIO_PIN_0
#define LCD_RD_PIN_REG             0x40024004

#define TFTLCD_ADDR	  *(unsigned short *)0xA0000000

#define LCD_VERTICAL_MAX 400
#define LCD_HORIZONTAL_MAX 240

#ifndef M_PI
#define M_PI                    3.14159265358979323846F
#endif

#define TICKS_PER_SECOND 40
#define FSECONDS_PER_TICK (1.0F / (float)TICKS_PER_SECOND)

#define SERIES_LENGTH 240


typedef struct _Series
{
	uint32_t xAxis;
	int32_t data;
}tSeries;

typedef void (*pfnWriteData)(uint16_t ui16Data);
typedef void (*pfnWriteCommand)(uint16_t ui8Data);
extern void InitGPIOLCDInterface(uint32_t ui32ClockMS);
extern void TFT_Init(void);
extern void TFT_REFRESH_GRAM(uint16_t ui16Data);
extern void TFT_SET_COM(uint16_t ui16Data);
extern void TFT_DrawPoint(unsigned int x,unsigned int y,unsigned int color);
extern void TFT_ShowString(uint32_t x,uint32_t y,char *p,uint32_t fColor, uint32_t bColor);
extern void TFT_CLEAR(uint32_t bColor);
extern void TFT_EnterSleep(void);
extern void TFT_ExitSleep(void);
extern void TFT_FillBlock(uint32_t x0, uint32_t x1, uint32_t y0, uint32_t y1, uint32_t Color);
extern void TFT_ShowData(uint32_t x,uint32_t y,int32_t i32Data,uint32_t fColor, uint32_t bColor);
extern void TFT_DrawHorizontalLine(uint32_t x0,uint32_t x1,uint32_t y,uint32_t color);
extern void TFT_DrawVerticalLine(uint32_t y0,uint32_t y1,uint32_t x,uint32_t color);
extern void TFT_ShowPicture(uint32_t x,uint32_t y, uint32_t width,uint32_t height, uint8_t* gImage);
void TFT_DrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color);
void TFT_DrawCircle(uint32_t x, uint32_t y, uint8_t radius, uint32_t color);
void TFT_DrawRectangle(uint32_t x,uint32_t y,uint32_t width,uint32_t height,uint32_t color);
void TFT_ShowName(uint32_t x, uint32_t y, uint8_t  name, uint32_t color);
void TFT_ShowWheel(void);
void TFT_Test(void);
void TFT_SetPos(uint32_t x0, uint32_t x1, uint32_t y0, uint32_t y1);
extern pfnWriteData WriteData;
void num2char(char *str, double number, uint8_t g, uint8_t l);
#endif
