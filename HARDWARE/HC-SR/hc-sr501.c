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
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOAʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); //ʹ��ADC1ʱ��

  //�ȳ�ʼ��ADC1ͨ��5 IO��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA5 ͨ��5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);	//��λ����	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
  ADC_Init(ADC2, &ADC_InitStructure);//ADC��ʼ��
	
 
	ADC_Cmd(ADC2, ENABLE);//����ADת����	
}
 u16 Get_Adc2(u8 ch)   
{
	  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
		ADC_RegularChannelConfig(ADC2, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
  
	ADC_SoftwareStartConv(ADC2);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC2);	//�������һ��ADC1�������ת�����
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


