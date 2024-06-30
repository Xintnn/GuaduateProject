///AT+NSOCL=0
#include "BC28.h"
#include "stm32f4xx.h"
#include "string.h"
#include "delay.h"
#include "usart.h"
#include "includes.h"
#include "stdint.h"

#define BUFLEN 255
static char *strx,*extstrx;
static char atstr[BUFLEN];
static int err;    					//ȫ�ֱ���



////////////////////////ֻҪ�޸���Ҫ��/////////////////////////////////
#define PRODUCEKEY     "k1a1aUY0t1t"             //2��?��KEY
#define DEVICENAME          "UNIproject"      //
#define DEVICESECRET    "cb415e439b472f43170b240e3a50f2a8"  //


void Clear_Buffer(void)//��մ���2����
{ 
  delay_ms(300);
  memset(g_usart2_rx_buf, 0, BUFLEN);
  g_usart2_rx_end=0;
  g_usart2_rx_cnt=0;
  
}

int BC28_Init(void)
{
    int errcount = 0;
    err = 0;    //�ж�ģ�鿨�Ƿ��������Ҫ
    printf("start init BC28\r\n");
		//Clear_Buffer();	
    usart_send_str(USART2,"ATE1\r\n");
    delay_ms(3000);
    printf("%s\n",g_usart2_rx_buf);      //��ӡ�յ��Ĵ�����Ϣ
    printf("get back BC28\r\n");
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"OK");//����OK
    while(strx==NULL)
    {
        printf("\r\n��Ƭ���������ӵ�ģ��...\r\n");
       // //Clear_Buffer();	
       usart_send_str(USART2,"ATE1\r\n");
        delay_ms(300);
        strx=strstr((const char*)g_usart2_rx_buf,(const char*)"OK");//����OK
    }
    
   usart_send_str(USART2,"AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
    delay_ms(300);
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"ERROR");//ֻҪ�������� �����ͳɹ�
    if(strx==NULL)
    {
        printf("�ҵĿ����� : %s \r\n",&g_usart2_rx_buf[8]);
        ////Clear_Buffer();	
        delay_ms(300);
    }
    else
    {
        err = 1;
        printf("������ : %s \r\n",g_usart2_rx_buf);
        ////Clear_Buffer();
        delay_ms(300);
    }

   usart_send_str(USART2,"AT+CGSN=1\r\n");//�������磬PDP
    delay_ms(300);
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"OK");//��OK
    ////Clear_Buffer();	
    if(strx)
    {
        //Clear_Buffer();	
        printf("GET IMEI OK\r\n");
        delay_ms(300);
    }
		
    usart_send_str(USART2,"AT+CGATT?\r\n");//��ѯ����״̬
    delay_ms(300);
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+CGATT:1");//��1 ��������ɹ� ��ȡ��IP��ַ��
    //Clear_Buffer();	
    errcount = 0;
    while(strx==NULL)
    {
        errcount++;
        ////Clear_Buffer();	
        usart_send_str(USART2,"AT+CGATT?\r\n");//��ȡ����״̬
        delay_ms(300);
        strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+CGATT:1");//����1,����ע���ɹ�
        if(errcount>100)     //��ֹ��ѭ��
        {
            err=1;
            errcount = 0;
            break;
        }
    }


   usart_send_str(USART2,"AT+NBAND?\r\n"); //�������ֵ
    Delay(300);
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"OK");//����OK
    if(strx)
    {
        printf("========BAND========= \r\n %s \r\n",g_usart2_rx_buf);
        ////Clear_Buffer();
        Delay(300);
    }
		//��ȡ�ź�����
   usart_send_str(USART2,"AT+CSQ\r\n");//�鿴��ȡCSQֵ
    Delay(300);
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+CSQ");//����CSQ
    if(strx)
    {
        printf("�ź�����:%s\r\n",g_usart2_rx_buf);
        ////Clear_Buffer();
        Delay(300);
    }
		//��ȡע��״̬
   usart_send_str(USART2,"AT+CEREG?\r\n");
    Delay(300);
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+CEREG:0,1");//����ע��״̬
    extstrx=strstr((const char*)g_usart2_rx_buf,(const char*)"+CEREG:1,1");//����ע��״̬
    Clear_Buffer();	
    
    errcount = 0;
    printf("success");
    while(strx==NULL&&extstrx==NULL)//��������ֵ��û��
    {
        errcount++;
      printf("error");
        ////Clear_Buffer();
        usart_send_str(USART2,"AT+CEREG?\r\n");//�ж���Ӫ��
        Delay(300);
        strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+CEREG:0,1");//����ע��״̬
        extstrx=strstr((const char*)g_usart2_rx_buf,(const char*)"+CEREG:1,1");//����ע��״̬
        if(errcount>100)     //��ֹ��ѭ��
        {
            err=1;
            errcount = 0;
            break;
        }
    }
    return err;
}

void BC28_PDPACT(void)//�������Ϊ���ӷ�������׼��
{
    int errcount = 0;
    usart_send_str(USART2,"AT+CGPADDR\r\n");//�����
    Delay(300);
    ////Clear_Buffer();
   usart_send_str(USART2,"ATI\r\n");//��ȡ�汾
    Delay(300);
   // //Clear_Buffer();
    usart_send_str(USART2,"AT+CGATT?\r\n");//�����
    Delay(300);
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+CGATT:1");//ע����������Ϣ
    ////Clear_Buffer();	
    while(strx==NULL)
    {
        errcount++;
        ////Clear_Buffer();
        usart_send_str(USART2,"AT+CGATT?\r\n");//�����
        Delay(300);
        strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+CGATT:1");//һ��Ҫ�ն�����
        if(errcount>100)     //��ֹ��ѭ��
        {
            errcount = 0;
            break;
        }
    }
   Clear_Buffer();
}


void BC28_RegALIYUNIOT(void)//ƽ̨ע��
{
    usart_send_str(USART2,"AT+QMTCLOSE=0\r\n");//Disconnect a client from MQTT server
    delay_ms(300);
   // //Clear_Buffer();

    usart_send_str(USART2,"AT+QMTDISC=0\r\n");//ɾ�����
    delay_ms(300);
   // //Clear_Buffer();    
	
	 usart_send_str(USART2,"AT+QIDNSCFG=\"8.8.8.8\",\"8.8.4.4\"\r\n");//ɾ�����
    delay_ms(300);
  printf("DNS SUCCESS");
   // //Clear_Buffer();

    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTCFG=\"ALIAUTH\",0,\"%s\",\"%s\",\"%s\"\r\n",PRODUCEKEY,DEVICENAME,DEVICESECRET);
    usart_send_str(USART2,atstr);			//���Ͱ��������ò���
    delay_ms(300);  					//�ȴ�300ms����OK
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"OK");//��OK
    while(strx==NULL)
    {
        strx=strstr((const char*)g_usart2_rx_buf,(const char*)"OK");//��OK
    }
    //Clear_Buffer();

   usart_send_str(USART2,"AT+QMTOPEN=0,\"iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\n");//��¼������ƽ̨ ��Ҫ�ȴ�5����
    delay_ms(300);
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+QMTOPEN: 0,0");//��+QMTOPEN: 0,0
    while(strx==NULL)
    {
        strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+QMTOPEN: 0,0");//��+QMTOPEN: 0,0
    }
   // Clear_Buffer();

		//�����豸�����ӵ��豸
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTCONN=0,\"%s\"\r\n",DEVICENAME);
    printf("atstr = %s \r\n",atstr);
    usart_send_str(USART2,atstr);		//�������ӵ�������
    delay_ms(300);
    printf("%s\n",g_usart2_rx_buf);
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+QMTCONN: 0,0,0");//��+QMTCONN: 0,0,0
    while(strx==NULL)
    {
        strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+QMTCONN: 0,0,0");//��+QMTCONN: 0,0,0
    }
    //Clear_Buffer();
/*
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTSUB=0,1,\"%s\",0 \r\n",SUBSCRIBE);
    printf("atstr = %s \r\n",atstr);
    Uart2_SendStr(atstr);//����
    delay_ms(300);
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+QMTSUB: 0,1,0,1");//��OK
    while(strx==NULL)
    {
        strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+QMTSUB: 0,1,0,1");//��OK
    }
    //Clear_Buffer();
*/		
}

void BC28_ALYIOTSenddata(u8 *len,u8 *data,u8 *data1,u8 *data2,u8 *data3,int *data4)//�Ϸ����ݣ��Ϸ������ݸ���Ӧ�Ĳ���й�ϵ���û���Ҫע����Ȼ���Ӧ���ݼ���
{
    int errcount = 0;
    memset(atstr,0,BUFLEN); //������������
    //AT+QMTPUB=0,0,0,0,"/sys/a18dtRetCT0/BC26TEST/thing/event/property/post"
    sprintf(atstr,"AT+QMTPUB=0,0,0,0,\"/sys/%s/%s/thing/event/property/post\"\n",PRODUCEKEY,DEVICENAME);
  //printf("AT+QMTPUB=0,0,0,0,\"/sys/%s/%s/thing/event/property/post\"\r\n",PRODUCEKEY,DEVICENAME);
  
    usart_send_str(USART2,atstr); 
  //printf("%s\n",g_usart2_rx_buf); 
    delay_ms(300);
	
		strx=strstr((const char*)g_usart2_rx_buf,(const char*)">");//��SEND OK
    while(strx==NULL)
    {
        errcount++;
        delay_ms(30);
        strx=strstr((const char*)g_usart2_rx_buf,(const char*)">");//����OK
//        if(errcount>200)     //��ֹ��ѭ��
//        {
//            GPIO_SetBits(GPIOD,GPIO_Pin_6);		//ģ������
//            Delay(500);
//            GPIO_ResetBits(GPIOD,GPIO_Pin_6);
//            Delay(300);
//            NVIC_SystemReset();	//û�д���TCP SOCKET������ϵͳ�ȵ�����������
//        }
    }	
	
	
    memset(atstr,0,BUFLEN); //��������
    sprintf(atstr,"{params:{Temperature:%s,Humidity:%s,GasConcentration:%s,DetectDistance:%s,beep:%d}}\r\n",data,data1,data2,data3,data4);
    printf(" %s \r\n",atstr);
    usart_send_str(USART2,atstr);
    delay_ms(30);
   usart_send_hexbytes(0x1a);   //��������
    
    delay_ms(30);
    Clear_Buffer();
}


