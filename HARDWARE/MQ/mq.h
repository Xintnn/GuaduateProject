#ifndef __MQ_H
#define __MQ_H

#include "sys.h"

extern uint16_t ADCConvertedValue[10];

extern void MQ135_Init(void);
extern float GetMQ135Out(void);


#endif