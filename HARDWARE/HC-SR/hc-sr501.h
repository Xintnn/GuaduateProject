#ifndef __HCSR501_H
#define __HCSR501_H
#include "sys.h"
void HC_SR501_Init(void);
int HC_SR501_Read(void);
void HC_SR501_Application(void);
u16 Get_Adc2_Average(u8 ch,u8 times);
u16 Get_Adc2(u8 ch);
#endif