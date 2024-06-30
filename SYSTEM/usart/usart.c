#include <stm32f4xx.h>	
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>


static USART_InitTypeDef   		USART_InitStructure;
static GPIO_InitTypeDef 		GPIO_InitStructure;
static NVIC_InitTypeDef   		NVIC_InitStructure;


volatile uint8_t  g_usart1_rx_buf[512];
volatile uint32_t g_usart1_rx_cnt=0;
volatile uint32_t g_usart1_rx_end=0;


volatile uint8_t  g_usart2_rx_buf[512];
volatile uint32_t g_usart2_rx_cnt=0;
volatile uint32_t g_usart2_rx_end=0;

volatile uint8_t  g_usart3_rx_buf[512];
volatile uint32_t g_usart3_rx_cnt=0;
volatile uint32_t g_usart3_rx_end=0;

#pragma import(__use_no_semihosting_swi)

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) 
{
	USART_SendData(USART1,ch);
		
	//�ȴ����ݷ��ͳɹ�
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	USART_ClearFlag(USART1,USART_FLAG_TXE);

	return ch;
}

void _sys_exit(int return_code) {

}

void _ttywrch(int ch) 
{ 
	ch = ch; 
} 
void usart1_init(uint32_t baud)
{
	//ʹ�ܶ˿�AӲ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	//ʹ�ܴ���1Ӳ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	
	//����PA9��PA10Ϊ���ù�������
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//��PA9��PA10���ӵ�USART1��Ӳ��
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	
	//����USART1����ز����������ʡ�����λ��У��λ
	USART_InitStructure.USART_BaudRate = baud;//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�����ڷ��ͺͽ�������
	USART_Init(USART1, &USART_InitStructure);
	
	
	//ʹ�ܴ��ڽ��յ����ݴ����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//ʹ�ܴ���1����
	USART_Cmd(USART1,ENABLE);
}

void usart2_init(uint32_t baud)
{
	//ʹ�ܶ˿�AӲ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
	//ʹ�ܴ���AӲ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	//����PA2��PA3Ϊ���ù�������
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	//��PA2��PA3���ӵ�USART2��Ӳ��
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
	
	
	//����USART1����ز����������ʡ�����λ��У��λ
	USART_InitStructure.USART_BaudRate = baud;//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�����ڷ��ͺͽ�������
	USART_Init(USART2, &USART_InitStructure);
	
	
	//ʹ�ܴ��ڽ��յ����ݴ����ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//ʹ�ܴ���2����
	USART_Cmd(USART2,ENABLE);
	

}



//void usart3_init(uint32_t baud)
//{
//	//ʹ�ܶ˿�BӲ��ʱ��
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
//	
//	//ʹ�ܴ���3Ӳ��ʱ��
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
//	
//	//����PB10��PB11Ϊ���ù�������
//	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10|GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	
//	//��PB10��PB10���ӵ�USART3��Ӳ��
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
//	
//	
//	//����USART1����ز����������ʡ�����λ��У��λ
//	USART_InitStructure.USART_BaudRate = baud;//������
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ����λ
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1λֹͣλ
//	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ��������
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�����ڷ��ͺͽ�������
//	USART_Init(USART3, &USART_InitStructure);
//	
//	
//	//ʹ�ܴ��ڽ��յ����ݴ����ж�
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	//ʹ�ܴ���3����
//	USART_Cmd(USART3,ENABLE);
//}



void usart_send_str(USART_TypeDef* USARTx,char *str)
{
	char *p = str;
	
	while(*p!='\0')
	{
		USART_SendData(USARTx,*p);
		
		p++;
	
		//�ȴ����ݷ��ͳɹ�
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);
		USART_ClearFlag(USARTx,USART_FLAG_TXE);
	}
}


void usart_send_bytes(USART_TypeDef* USARTx,uint8_t *buf,uint32_t len)
{
	uint8_t *p = buf;
	
	while(len--)
	{
		USART_SendData(USARTx,*p);
		
		p++;
		
		//�ȴ����ݷ��ͳɹ�
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);
		USART_ClearFlag(USARTx,USART_FLAG_TXE);
	}
}
void usart_send_hexbytes(char SendBuf)
{
	while((USART2->SR&0X40)==0)
    {
    }//�ȴ��������
    USART2->DR = (u8) SendBuf;
    while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
		USART_ClearFlag(USART2,USART_FLAG_TXE);
}




void USART1_IRQHandler(void)
{
	uint8_t d=0;
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	//����Ƿ���յ�����
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		d=USART_ReceiveData(USART1);
		
		g_usart1_rx_buf[g_usart1_rx_cnt++]=d;
		
		if(g_usart1_rx_cnt >= sizeof g_usart1_rx_buf)
		{
			g_usart1_rx_end=1;
		}
#if 0		
		USART_SendData(USART1,d);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);	
#endif		
		//��ձ�־λ��������Ӧ�µ��ж�����
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
  #ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
}



void USART2_IRQHandler(void)
{
	uint8_t d=0;
	#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	//����Ƿ���յ�����
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		d=USART_ReceiveData(USART2);
		
		g_usart2_rx_buf[g_usart2_rx_cnt++]=d;
		
		if(g_usart2_rx_cnt >= sizeof g_usart2_rx_buf)
		{
			g_usart2_rx_end=1;
		}
#if 0		
		USART_SendData(USART2,d);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
#endif		
		//��ձ�־λ��������Ӧ�µ��ж�����
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    #ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
	}
}
/*
void USART1_IRQHandler(void)
{
   uint8_t value;
   if(USART_GetFlagStatus(USART1,USART_IT_RXNE) == SET)
    {   
value = USART_ReceiveData(USART1);


    }


    if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    { 
        
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);


    }


if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET)   
{               
USART_ReceiveData(USART1); 
USART_ClearFlag(USART1,USART_FLAG_ORE);              
    
}
}



*/

//void USART3_IRQHandler(void)
//{
//	uint8_t d=0;
//	
//	//����Ƿ���յ�����
//	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
//	{
//		d=USART_ReceiveData(USART3);
//		
//		g_usart3_rx_buf[g_usart3_rx_cnt++]=d;
//		
//		if(g_usart3_rx_cnt >= sizeof g_usart3_rx_buf)
//		{
//			g_usart3_rx_end=1;
//		}
//#if 0		
//		USART_SendData(USART1,d);
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
//#endif		
//		//��ձ�־λ��������Ӧ�µ��ж�����
//		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//	}
//}
