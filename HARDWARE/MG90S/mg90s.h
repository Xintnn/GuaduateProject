#ifndef __MG90S_H
#define __MG90S_H
#include "sys.h"
#include "delay.h"
#include "math.h"
#include "stm32f4xx.h"
/*
 *	舵机驱动头文件
 */

void mg_init(void);
	void sg_angle(uint32_t angle);
#endif 