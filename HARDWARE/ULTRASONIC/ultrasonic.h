#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H	 
#include "sys.h"
void sr04_init(void);
uint32_t sr04_get_distance(void);
#endif