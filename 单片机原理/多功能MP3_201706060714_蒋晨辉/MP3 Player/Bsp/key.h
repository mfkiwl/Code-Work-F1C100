/*
 * key.h
 *
 *  Created on: 2019Äê10ÔÂ15ÈÕ
 *      Author: Chenhui
 */

#ifndef KEY_H_
#define KEY_H_

void KEY_Init(void);
void KEY_Test(void);
int KEY_Read(void);
void KEY_Int_Init(void);
void PortPIntHandler(void);
void PortNIntHandler(void);
void PortDIntHandler(void);
#endif /* KEY_H_ */
