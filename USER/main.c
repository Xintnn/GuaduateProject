#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "includes.h"
#include "beep.h"
#include "key.h"
#include "ultrasonic.h"
#include "dht11.h"
#include "mq.h"
#include "bc28.h"
#include "hc-sr501.h"
#include "mg90s.h"
#include "oled.h"

#define BUFLEN 48
static uint8_t buf[5];
static int32_t distance=100;
static double mq;
static char senddata[BUFLEN];
static char sendbeep[8];
static char sendmq[BUFLEN];
static char senddis[BUFLEN];
static char sendhumi[BUFLEN];
static char humistr[BUFLEN];
static char tempstr[BUFLEN];
static char mqstr[BUFLEN];
static char disstr[BUFLEN];
static char beepstr[8];
static int flag_open=0;
static int beep_flag=0;
static int warning=0;
static int t_angle=0;
struct os_stk_data StackBytes;
// STM32F407������ UCOSʵ��1
//UCOSII ��ֲ
//STM32F4���� 
//�Ա����̣�http://mcudev.taobao.com	

//START ����
//�����������ȼ�
#define START_TASK_PRIO			15  ///��ʼ��������ȼ�Ϊ���
//���������ջ��С
#define START_STK_SIZE			128
//���������ջ
__align(8) OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata);

//LED0����
//�����������ȼ�
#define LED_TASK_PRIO			14
//���������ջ��С
#define LED_STK_SIZE				64
//�����ջ
__align(8) OS_STK LED_TASK_STK[LED_STK_SIZE];
//������
void led_task(void *pdata);

//LED1����
//�����������ȼ�
#define ULTRASONIC_TASK_PRIO			11
//���������ջ��С
#define ULTRASONIC_STK_SIZE				256
//�����ջ
__align(8) OS_STK ULTRASONIC_TASK_STK[ULTRASONIC_STK_SIZE];
//������
void ultrasonic_task(void *pdata);

//�����������
#define DHT11_TASK_PRIO			10
//���������ջ��С
#define DHT11_STK_SIZE			256
//�����ջ
//���������ʹ��printf����ӡ�������ݵĻ�һ��Ҫ8�ֽڶ���
__align(8) OS_STK DHT11_TASK_STK[DHT11_STK_SIZE]; 
//������
void dht11_task(void *pdata);

//�����������
#define MQ_TASK_PRIO			9
//���������ջ��С
#define MQ_STK_SIZE			374
//�����ջ
//���������ʹ��printf����ӡ�������ݵĻ�һ��Ҫ8�ֽڶ���
__align(8) OS_STK MQ_TASK_STK[MQ_STK_SIZE]; 
//������
void mq_task(void *pdata);

//�����������
#define BC28_TASK_PRIO			6
//���������ջ��С
#define BC28_STK_SIZE			256
//�����ջ
//���������ʹ��printf����ӡ�������ݵĻ�һ��Ҫ8�ֽڶ���
__align(8) OS_STK BC28_TASK_STK[BC28_STK_SIZE]; 
//������
void bc28_task(void *pdata);

#define OPENLID_TASK_PRIO			5
//���������ջ��С
#define OPENLID_STK_SIZE			256
//�����ջ
//���������ʹ��printf����ӡ�������ݵĻ�һ��Ҫ8�ֽڶ���
__align(8) OS_STK OPENLID_TASK_STK[OPENLID_STK_SIZE]; 
//������
void openlid_task(void *pdata);

#define OLED_TASK_PRIO		12	
//���������ջ��С
#define OLED_STK_SIZE			1024  
//�����ջ
//���������ʹ��printf����ӡ�������ݵĻ�һ��Ҫ8�ֽڶ���
__align(8) OS_STK OLED_TASK_STK[OLED_STK_SIZE]; 
//������
void oled_task(void *pdata);

#define BEEP_TASK_PRIO		13	
//���������ջ��С
#define BEEP_STK_SIZE			128  
//�����ջ
//���������ʹ��printf����ӡ�������ݵĻ�һ��Ҫ8�ֽڶ���
__align(8) OS_STK BEEP_TASK_STK[BEEP_STK_SIZE]; 
//������
void beep_task(void *pdata);


int main(void)
{
	delay_init(168);       //��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //�жϷ�������
	usart1_init(115200);
  usart2_init(9600);
    //���ڲ���������
 // LED_Init();
 mg_init();
  sg_angle(0);
  beep_Init();
  
  MQ135_Init();
  OLED_Init();			//��ʼ��OLED  
	 OLED_Clear();
  
    name_ch();
	  //LED��ʼ��
	sr04_init();
  dht11_init();
  HC_SR501_Init();
  BC28_Init();
  	OLED_ShowCHinese(18,2,14);
   BC28_PDPACT();
  	OLED_ShowCHinese(36,2,14);
   BC28_RegALIYUNIOT();
   	OLED_ShowCHinese(54,2,14);
    OLED_Clear();
    ok_ch();
    beep_flag=beep_on();
    delay_ms(500);
     beep_flag=beep_off();
    delay_ms(2500);
	OSInit();  //UCOS��ʼ��
	OSTaskCreate(start_task,(void*)0,(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO); //������ʼ����
	OSStart(); //��ʼ����
}

//��ʼ����
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata=pdata;
	OSStatInit();  //����ͳ������
	
	OS_ENTER_CRITICAL();  //�����ٽ���(�ر��ж�)
	//OSTaskCreate(beep_task,(void*)0,(OS_STK*)&BEEP_TASK_STK[BEEP_STK_SIZE-1],BEEP_TASK_PRIO);//����LED0����
  OSTaskCreate(oled_task,(void*)0,(OS_STK*)&OLED_TASK_STK[OLED_STK_SIZE-1],OLED_TASK_PRIO);
	OSTaskCreate(ultrasonic_task,(void*)0,(OS_STK*)&ULTRASONIC_TASK_STK[ULTRASONIC_STK_SIZE-1],ULTRASONIC_TASK_PRIO);//����LED1����
	OSTaskCreate(dht11_task,(void*)0,(OS_STK*)&DHT11_TASK_STK[DHT11_STK_SIZE-1],DHT11_TASK_PRIO);//���������������
  OSTaskCreate(mq_task,(void*)0,(OS_STK*)&MQ_TASK_STK[MQ_STK_SIZE-1],MQ_TASK_PRIO);//���������������
  OSTaskCreate(bc28_task,(void*)0,(OS_STK*)&BC28_TASK_STK[BC28_STK_SIZE-1],BC28_TASK_PRIO);//���������������
  OSTaskCreate(openlid_task,(void*)0,(OS_STK*)&OPENLID_TASK_STK[OPENLID_STK_SIZE-1],OPENLID_TASK_PRIO);
  
	OSTaskSuspend(START_TASK_PRIO);//����ʼ����
	OS_EXIT_CRITICAL();  //�˳��ٽ���(���ж�)
}
 

//LED0����
void led_task(void *pdata)
{
  
	while(1)
	{
    LED1=0;
		delay_ms(500);
		LED1=1;
		delay_ms(500);
    };
		
}


//����������
void ultrasonic_task(void *pdata)
{
	
  while(1){
    distance = sr04_get_distance();
//    dht11_read(buf);
//    mq=GetMQ135Out();
    delay_ms(800);
};
 }

//DHT11��������
void dht11_task(void *pdata)
{

  while(1){
    dht11_read(buf);
    delay_ms(2000);
    if(buf[2]>50){
      t_angle=1;
    }else{
      t_angle=0;
    }
    if(t_angle==1){
      sg_angle(45);
    }
  };
}
void mq_task(void *pdata){
  
  while(1){
    mq=GetMQ135Out();
    delay_ms(1000);
    if(mq>20)warning++;
    if(warning==10){
      beep_on();
      delay_ms(1000);
      beep_off();
      warning=0;
    }
  }

    

}
void bc28_task(void *pdata){
  
    while(1){
  delay_ms(5000);
    memset(tempstr,0,BUFLEN);
    memset(senddata,0,BUFLEN);
    memset(humistr,0,BUFLEN);
    memset(sendhumi,0,BUFLEN);
    memset(mqstr,0,BUFLEN);
    memset(sendmq,0,BUFLEN);
    memset(disstr,0,BUFLEN);
    memset(senddis,0,BUFLEN);
    memset(sendbeep,0,8);
    memset(beepstr,0,8);
    sprintf(tempstr,"%d.%d",buf[2],buf[3]);
    strcat(senddata,tempstr);
    sprintf(humistr,"%d.%d",buf[0],buf[1]);
    strcat(sendhumi,humistr);
    sprintf(mqstr,"%f",mq);
    strcat(sendmq,mqstr);
    sprintf(disstr,"%d",distance);
    strcat(senddis,disstr);
    
    BC28_ALYIOTSenddata("8",(u8 *)senddata,(u8 *) sendhumi,(u8 *)sendmq,(u8 *)senddis,(int *)beep_flag);
      

    };

}

void openlid_task(void *pdata){
  while(1){
    delay_ms(2500);
    HC_SR501_Application();
    
  }

}

void oled_task(void *pdata){
  delay_ms(500);
  OLED_Clear();
  OLED_ShowChar(8,0,84,16);
  OLED_ShowChar(16,0,58,16);
  OLED_ShowChar(8,2,72,16);
  OLED_ShowChar(16,2,58,16);
  
  OLED_ShowCHinese(0,4,19);
  OLED_ShowChar(16,4,58,16);
  OLED_ShowCHinese(0,6,18);
  OLED_ShowChar(16,6,58,16);
  while(1){
    int n,i;
    for(i=0;i<4;i=i+2)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		for(n=24;n<32;n++)OLED_WR_Byte(0,OLED_DATA); 
	} 
  for(i=4;i<8;i=i+2)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		for(n=23;n<48;n++)OLED_WR_Byte(0,OLED_DATA); 
	} 
    delay_ms(1000);
    OLED_ShowNum(24,0,buf[2],2,16);
    OLED_ShowNum(24,2,buf[0],2,16);
    if(mq<10.0){
      OLED_ShowCHinese(24,4,22);
      OLED_ShowCHinese(40,4,23);
    }
    else {
      OLED_ShowCHinese(24,4,20);
      OLED_ShowCHinese(40,4,21);
    }
    if(distance<20||distance>5000){
      OLED_ShowCHinese(24,6,15);
    }
    else OLED_ShowCHinese(24,6,17);
    OLED_ShowCHinese(40,6,16);
    delay_ms(2000);
  }

}

void beep_task(void *pdata){
  while(1){
    
  if(!beep_flag){
    beep_on();
  }else{
    beep_off();
  }
  
  delay_ms(800);
  }

}