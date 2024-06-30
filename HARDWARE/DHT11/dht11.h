#ifndef __DHT11_H
#define __DHT11_H 
#include "sys.h"
void dht11_init(void);//初始化DHT11
int32_t dht11_read(uint8_t *buf);//读取温湿度
u8 dht11_check(void);//检测是否存在DHT11

void DHT11_Rst(void);//复位DHT11    
#endif