#ifndef __BEEP_H
#define __BEEP_H	 
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F4����ģ��-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com								  
////////////////////////////////////////////////////////////////////////////////// 

//LED�˿ڶ���
#define beep1 PAout(9)	// ����������IO 

void beep_Init(void);
int  beep_on(void);
int beep_off(void);//��ʼ��		 				    
#endif


















