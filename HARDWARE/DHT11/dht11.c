#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"

void dht11_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	//使能端口G的硬件时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		
	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;//6号引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;//2MHz的控制响应速度，引脚翻转的响应速度设置，速度越高，功耗越高
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;//开漏输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  //上下拉电阻的不使能
	GPIO_Init(GPIOA,&GPIO_InitStructure);	

	
	//只要是输出模式，会有初始的电平状态，看时序图
	PAout(6)=1;
}


int32_t dht11_read(uint8_t *buf)
{
	int32_t t=0;
	int32_t i=0,j=0;
	uint8_t d=0;
	uint8_t *p=buf;
	uint8_t check_sum=0;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;//6号引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;//2MHz的控制响应速度，引脚翻转的响应速度设置，速度越高，功耗越高
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;//开漏输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  //上下拉电阻的不使能
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	
	
	PAout(6)=0;
	delay_ms(18);
	PAout(6)=1;
	delay_us(30);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;//9号引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;//输入模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;//2MHz的控制响应速度，引脚翻转的响应速度设置，速度越高，功耗越高
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  //上下拉电阻的不使能
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	
	
	//等待DHT11的低电平响应信号
	t=0;
	while(PAin(6))
	{
		t++;
		delay_us(1);
		if(t>=1000000)
			return -1;
	
	}
	
	//测量DHT11的低电平响应信号持续时间
	t=0;
	while(PAin(6)==0)
	{
		delay_us(1);
		t++;
		if(t>=100)
			return -2;
	
	}
	
	//测量DHT11的高电平响应信号持续时间
	t=0;
	while(PAin(6))
	{
		delay_us(1);
		t++;
		if(t>=100)
			return -3;
	}
	
	for(j=0; j<5; j++)
	{
		//因为当前是MSB，最高有效位优先传输
		d=0;
		for(i=7; i>=0; i--)
		{
			//等待前置50us低电平持续完毕
			t=0;
			while(PAin(6)==0)
			{
				delay_us(1);
				t++;
				if(t>=100)
					return -4;
			
			}
			
			//小延时
			delay_us(40);
			
			if(PAin(6))
			{
				d|=1<<i;
				
				//等待剩下的高电平持续完毕
				t=0;
				while(PAin(6))
				{
					delay_us(1);
					t++;
					if(t>=100)
						return -5;
				
				}			
				
			}

			
		
		
		}

		p[j]=d;

	}
	
	//进行数据校验
	check_sum=(p[0]+p[1]+p[2]+p[3])	& 0xFF;
	if(check_sum == p[4])
		return 0;
		
	return -6;	
}
