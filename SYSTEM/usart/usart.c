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
		
	//等待数据发送成功
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
	//使能端口A硬件时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	//使能串口1硬件时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	
	//配置PA9、PA10为复用功能引脚
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//将PA9、PA10连接到USART1的硬件
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	
	//配置USART1的相关参数：波特率、数据位、校验位
	USART_InitStructure.USART_BaudRate = baud;//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//允许串口发送和接收数据
	USART_Init(USART1, &USART_InitStructure);
	
	
	//使能串口接收到数据触发中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//使能串口1工作
	USART_Cmd(USART1,ENABLE);
}

void usart2_init(uint32_t baud)
{
	//使能端口A硬件时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
	//使能串口A硬件时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	//配置PA2、PA3为复用功能引脚
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	//将PA2、PA3连接到USART2的硬件
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
	
	
	//配置USART1的相关参数：波特率、数据位、校验位
	USART_InitStructure.USART_BaudRate = baud;//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//允许串口发送和接收数据
	USART_Init(USART2, &USART_InitStructure);
	
	
	//使能串口接收到数据触发中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//使能串口2工作
	USART_Cmd(USART2,ENABLE);
	

}



//void usart3_init(uint32_t baud)
//{
//	//使能端口B硬件时钟
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
//	
//	//使能串口3硬件时钟
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
//	
//	//配置PB10、PB11为复用功能引脚
//	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10|GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	
//	//将PB10、PB10连接到USART3的硬件
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
//	
//	
//	//配置USART1的相关参数：波特率、数据位、校验位
//	USART_InitStructure.USART_BaudRate = baud;//波特率
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据位
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1位停止位
//	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件流控制
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//允许串口发送和接收数据
//	USART_Init(USART3, &USART_InitStructure);
//	
//	
//	//使能串口接收到数据触发中断
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	//使能串口3工作
//	USART_Cmd(USART3,ENABLE);
//}



void usart_send_str(USART_TypeDef* USARTx,char *str)
{
	char *p = str;
	
	while(*p!='\0')
	{
		USART_SendData(USARTx,*p);
		
		p++;
	
		//等待数据发送成功
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
		
		//等待数据发送成功
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);
		USART_ClearFlag(USARTx,USART_FLAG_TXE);
	}
}
void usart_send_hexbytes(char SendBuf)
{
	while((USART2->SR&0X40)==0)
    {
    }//等待发送完成
    USART2->DR = (u8) SendBuf;
    while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
		USART_ClearFlag(USART2,USART_FLAG_TXE);
}




void USART1_IRQHandler(void)
{
	uint8_t d=0;
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif
	//检测是否接收到数据
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
		//清空标志位，可以响应新的中断请求
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
  #ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();  											 
#endif
}



void USART2_IRQHandler(void)
{
	uint8_t d=0;
	#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif
	//检测是否接收到数据
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
		//清空标志位，可以响应新的中断请求
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    #ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
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
//	//检测是否接收到数据
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
//		//清空标志位，可以响应新的中断请求
//		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//	}
//}
