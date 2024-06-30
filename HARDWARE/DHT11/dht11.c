#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"

void dht11_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	//ʹ�ܶ˿�G��Ӳ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		
	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;//6������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;//2MHz�Ŀ�����Ӧ�ٶȣ����ŷ�ת����Ӧ�ٶ����ã��ٶ�Խ�ߣ�����Խ��
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;//��©���
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  //����������Ĳ�ʹ��
	GPIO_Init(GPIOA,&GPIO_InitStructure);	

	
	//ֻҪ�����ģʽ�����г�ʼ�ĵ�ƽ״̬����ʱ��ͼ
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
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;//6������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;//2MHz�Ŀ�����Ӧ�ٶȣ����ŷ�ת����Ӧ�ٶ����ã��ٶ�Խ�ߣ�����Խ��
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;//��©���
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  //����������Ĳ�ʹ��
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	
	
	PAout(6)=0;
	delay_ms(18);
	PAout(6)=1;
	delay_us(30);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;//9������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;//����ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;//2MHz�Ŀ�����Ӧ�ٶȣ����ŷ�ת����Ӧ�ٶ����ã��ٶ�Խ�ߣ�����Խ��
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  //����������Ĳ�ʹ��
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	
	
	//�ȴ�DHT11�ĵ͵�ƽ��Ӧ�ź�
	t=0;
	while(PAin(6))
	{
		t++;
		delay_us(1);
		if(t>=1000000)
			return -1;
	
	}
	
	//����DHT11�ĵ͵�ƽ��Ӧ�źų���ʱ��
	t=0;
	while(PAin(6)==0)
	{
		delay_us(1);
		t++;
		if(t>=100)
			return -2;
	
	}
	
	//����DHT11�ĸߵ�ƽ��Ӧ�źų���ʱ��
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
		//��Ϊ��ǰ��MSB�������Чλ���ȴ���
		d=0;
		for(i=7; i>=0; i--)
		{
			//�ȴ�ǰ��50us�͵�ƽ�������
			t=0;
			while(PAin(6)==0)
			{
				delay_us(1);
				t++;
				if(t>=100)
					return -4;
			
			}
			
			//С��ʱ
			delay_us(40);
			
			if(PAin(6))
			{
				d|=1<<i;
				
				//�ȴ�ʣ�µĸߵ�ƽ�������
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
	
	//��������У��
	check_sum=(p[0]+p[1]+p[2]+p[3])	& 0xFF;
	if(check_sum == p[4])
		return 0;
		
	return -6;	
}
