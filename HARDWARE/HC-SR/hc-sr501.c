#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
#include "mg90s.h"
#include "hc-sr501.h"
static int flag_open=0;

void HC_SR501_Init(void) {
GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); //使能ADC1时钟

  //先初始化ADC1通道5 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA5 通道5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);	//复位结束	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
  ADC_Init(ADC2, &ADC_InitStructure);//ADC初始化
	
 
	ADC_Cmd(ADC2, ENABLE);//开启AD转换器	
}
 u16 Get_Adc2(u8 ch)   
{
	  	//设置指定ADC的规则组通道，一个序列，采样时间
		ADC_RegularChannelConfig(ADC2, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
  
	ADC_SoftwareStartConv(ADC2);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC2);	//返回最近一次ADC1规则组的转换结果
}
u16 Get_Adc2_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc2(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 
int HC_SR501_Read(void) {
  uint16_t adcvalue = 0;
    adcvalue=Get_Adc2_Average(13,2);

  //printf("adc value:%d\n",(int)adcvalue/1000);
    return (int)adcvalue/1000;
    
}
 
void HC_SR501_Application(void) {
  int time=0; 
  int flag=HC_SR501_Read();
//  if(HC_SR501_Read()){
//  while(HC_SR501_Read()){
//    delay_ms(1000);
//      time++;
//    if(time>=2){
//      sg_angle(90);
//      
//      flag=1;
//      delay_ms(5000);
//      while(flag){
//      if(HC_SR501_Read()==0)
//      { delay_ms(1000);
//        if(HC_SR501_Read()==0){
//        sg_angle(0);
//        printf("close the door!");
//          flag=0;
//          
//  }
//}
//  break;
//      }
//    
//  }
//  }
//}
printf("open=%d",flag_open);
if(flag>=3){
  delay_ms(1000);
  if(flag_open==0){
  flag=HC_SR501_Read();
  if(flag>=3)flag_open=1;
  }
}else{

  if(flag_open==0){
    delay_ms(150);
    return;
  }
  
  delay_ms(2500);
  flag=HC_SR501_Read();
   if(flag_open==1&&flag<3){
    flag_open=0;
  }
  
}
switch(flag_open){
  case 0:sg_angle(0);
  delay_ms(2500);
  break;
  case 1:sg_angle(90);
  delay_ms(5000);
  break;

}
delay_ms(500);
}


