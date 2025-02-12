#include "beep.h" 
#include "stm32f4xx.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F4工程模板-库函数版本
//淘宝店铺：http://mcudev.taobao.com								  
////////////////////////////////////////////////////////////////////////////////// 	 

//初始化PF8为输出口		    
//BEEP IO初始化
void beep_Init(void)
{   
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOF时钟
  
  //初始化蜂鸣器对应引脚GPIOF8
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//下拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	PBout(3)=1;
  
    //蜂鸣器对应引脚GPIOF8拉低， 
  GPIO_SetBits(GPIOB,GPIO_Pin_3);
}
int beep_on(void){
GPIO_ResetBits(GPIOB,GPIO_Pin_3);
  return 1;
}
int beep_off(void){
GPIO_SetBits(GPIOB,GPIO_Pin_3);
  return 0;
}




