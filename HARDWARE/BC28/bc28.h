#include "usart.h"
#include <stm32f4xx.h>
#ifndef __BC28_H
#define __BC28_H
typedef struct
{
   uint8_t CSQ;    
   uint8_t Socketnum;   //���
   uint8_t reclen[10];   //��ȡ�����ݵĳ���
   uint8_t res;      
   uint8_t recdatalen[10];
   uint8_t recdata[100];
} BC28;
void Clear_Buffer(void);//��ջ���	
int BC28_Init(void);
void BC28_PDPACT(void);;
void BC28_RECData(void);
void BC28_RegALIYUNIOT(void);
void BC28_ALYIOTSenddata(u8 *len,u8 *data,u8 *data1,u8 *data2,u8 *data3,int *data4);

#endif