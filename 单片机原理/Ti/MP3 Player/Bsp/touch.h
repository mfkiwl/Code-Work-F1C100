/*
 * TOUCH_TSC2046.h
 *
 *  Created on: 2014Äê8ÔÂ28ÈÕ
 *      Author: Sam
 */

#ifndef TOUCH_TSC2046_H_
#define TOUCH_TSC2046_H_
#include <stdbool.h>
#include <stdint.h>




extern void TOUCH_Init(void);
extern void SoftSPI_PutData(uint8_t ui8Data);
extern uint16_t SoftSPI_GetData(void);
extern uint16_t touch_gety(void);
extern uint16_t touch_getx(void);
void Get_Point(void);
extern void TOUCH_PointAdjust(uint32_t *TouchXData, uint32_t *TouchYData);
void TOUCH_Test(void);
#endif /* TOUCH_TSC2046_H_ */
