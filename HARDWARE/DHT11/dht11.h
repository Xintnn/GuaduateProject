#ifndef __DHT11_H
#define __DHT11_H 
#include "sys.h"
void dht11_init(void);//��ʼ��DHT11
int32_t dht11_read(uint8_t *buf);//��ȡ��ʪ��
u8 dht11_check(void);//����Ƿ����DHT11

void DHT11_Rst(void);//��λDHT11    
#endif