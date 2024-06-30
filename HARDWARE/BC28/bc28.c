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
static int err;    					//全局变量



////////////////////////只要修改三要素/////////////////////////////////
#define PRODUCEKEY     "k1a1aUY0t1t"             //2ú?·KEY
#define DEVICENAME          "UNIproject"      //
#define DEVICESECRET    "cb415e439b472f43170b240e3a50f2a8"  //


void Clear_Buffer(void)//清空串口2缓存
{ 
  delay_ms(300);
  memset(g_usart2_rx_buf, 0, BUFLEN);
  g_usart2_rx_end=0;
  g_usart2_rx_cnt=0;
  
}

int BC28_Init(void)
{
    int errcount = 0;
    err = 0;    //判断模块卡是否就绪最重要
    printf("start init BC28\r\n");
		//Clear_Buffer();	
    usart_send_str(USART2,"ATE1\r\n");
    delay_ms(3000);
    printf("%s\n",g_usart2_rx_buf);      //打印收到的串口信息
    printf("get back BC28\r\n");
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"OK");//返回OK
    while(strx==NULL)
    {
        printf("\r\n单片机正在连接到模块...\r\n");
       // //Clear_Buffer();	
       usart_send_str(USART2,"ATE1\r\n");
        delay_ms(300);
        strx=strstr((const char*)g_usart2_rx_buf,(const char*)"OK");//返回OK
    }
    
   usart_send_str(USART2,"AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
    delay_ms(300);
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"ERROR");//只要卡不错误 基本就成功
    if(strx==NULL)
    {
        printf("我的卡号是 : %s \r\n",&g_usart2_rx_buf[8]);
        ////Clear_Buffer();	
        delay_ms(300);
    }
    else
    {
        err = 1;
        printf("卡错误 : %s \r\n",g_usart2_rx_buf);
        ////Clear_Buffer();
        delay_ms(300);
    }

   usart_send_str(USART2,"AT+CGSN=1\r\n");//激活网络，PDP
    delay_ms(300);
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"OK");//返OK
    ////Clear_Buffer();	
    if(strx)
    {
        //Clear_Buffer();	
        printf("GET IMEI OK\r\n");
        delay_ms(300);
    }
		
    usart_send_str(USART2,"AT+CGATT?\r\n");//查询激活状态
    delay_ms(300);
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+CGATT:1");//返1 表明激活成功 获取到IP地址了
    //Clear_Buffer();	
    errcount = 0;
    while(strx==NULL)
    {
        errcount++;
        ////Clear_Buffer();	
        usart_send_str(USART2,"AT+CGATT?\r\n");//获取激活状态
        delay_ms(300);
        strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+CGATT:1");//返回1,表明注网成功
        if(errcount>100)     //防止死循环
        {
            err=1;
            errcount = 0;
            break;
        }
    }


   usart_send_str(USART2,"AT+NBAND?\r\n"); //允许错误值
    Delay(300);
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"OK");//返回OK
    if(strx)
    {
        printf("========BAND========= \r\n %s \r\n",g_usart2_rx_buf);
        ////Clear_Buffer();
        Delay(300);
    }
		//获取信号质量
   usart_send_str(USART2,"AT+CSQ\r\n");//查看获取CSQ值
    Delay(300);
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+CSQ");//返回CSQ
    if(strx)
    {
        printf("信号质量:%s\r\n",g_usart2_rx_buf);
        ////Clear_Buffer();
        Delay(300);
    }
		//获取注册状态
   usart_send_str(USART2,"AT+CEREG?\r\n");
    Delay(300);
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+CEREG:0,1");//返回注册状态
    extstrx=strstr((const char*)g_usart2_rx_buf,(const char*)"+CEREG:1,1");//返回注册状态
    Clear_Buffer();	
    
    errcount = 0;
    printf("success");
    while(strx==NULL&&extstrx==NULL)//两个返回值都没有
    {
        errcount++;
      printf("error");
        ////Clear_Buffer();
        usart_send_str(USART2,"AT+CEREG?\r\n");//判断运营商
        Delay(300);
        strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+CEREG:0,1");//返回注册状态
        extstrx=strstr((const char*)g_usart2_rx_buf,(const char*)"+CEREG:1,1");//返回注册状态
        if(errcount>100)     //防止死循环
        {
            err=1;
            errcount = 0;
            break;
        }
    }
    return err;
}

void BC28_PDPACT(void)//激活场景，为连接服务器做准备
{
    int errcount = 0;
    usart_send_str(USART2,"AT+CGPADDR\r\n");//激活场景
    Delay(300);
    ////Clear_Buffer();
   usart_send_str(USART2,"ATI\r\n");//获取版本
    Delay(300);
   // //Clear_Buffer();
    usart_send_str(USART2,"AT+CGATT?\r\n");//激活场景
    Delay(300);
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+CGATT:1");//注册上网络信息
    ////Clear_Buffer();	
    while(strx==NULL)
    {
        errcount++;
        ////Clear_Buffer();
        usart_send_str(USART2,"AT+CGATT?\r\n");//激活场景
        Delay(300);
        strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+CGATT:1");//一定要终端入网
        if(errcount>100)     //防止死循环
        {
            errcount = 0;
            break;
        }
    }
   Clear_Buffer();
}


void BC28_RegALIYUNIOT(void)//平台注册
{
    usart_send_str(USART2,"AT+QMTCLOSE=0\r\n");//Disconnect a client from MQTT server
    delay_ms(300);
   // //Clear_Buffer();

    usart_send_str(USART2,"AT+QMTDISC=0\r\n");//删除句柄
    delay_ms(300);
   // //Clear_Buffer();    
	
	 usart_send_str(USART2,"AT+QIDNSCFG=\"8.8.8.8\",\"8.8.4.4\"\r\n");//删除句柄
    delay_ms(300);
  printf("DNS SUCCESS");
   // //Clear_Buffer();

    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTCFG=\"ALIAUTH\",0,\"%s\",\"%s\",\"%s\"\r\n",PRODUCEKEY,DEVICENAME,DEVICESECRET);
    usart_send_str(USART2,atstr);			//发送阿里云配置参数
    delay_ms(300);  					//等待300ms反馈OK
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"OK");//返OK
    while(strx==NULL)
    {
        strx=strstr((const char*)g_usart2_rx_buf,(const char*)"OK");//返OK
    }
    //Clear_Buffer();

   usart_send_str(USART2,"AT+QMTOPEN=0,\"iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\n");//登录阿里云平台 需要等待5分钟
    delay_ms(300);
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+QMTOPEN: 0,0");//返+QMTOPEN: 0,0
    while(strx==NULL)
    {
        strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+QMTOPEN: 0,0");//返+QMTOPEN: 0,0
    }
   // Clear_Buffer();

		//链接设备，连接到设备
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTCONN=0,\"%s\"\r\n",DEVICENAME);
    printf("atstr = %s \r\n",atstr);
    usart_send_str(USART2,atstr);		//发送链接到阿里云
    delay_ms(300);
    printf("%s\n",g_usart2_rx_buf);
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+QMTCONN: 0,0,0");//返+QMTCONN: 0,0,0
    while(strx==NULL)
    {
        strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+QMTCONN: 0,0,0");//返+QMTCONN: 0,0,0
    }
    //Clear_Buffer();
/*
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTSUB=0,1,\"%s\",0 \r\n",SUBSCRIBE);
    printf("atstr = %s \r\n",atstr);
    Uart2_SendStr(atstr);//订阅
    delay_ms(300);
    strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+QMTSUB: 0,1,0,1");//返OK
    while(strx==NULL)
    {
        strx=strstr((const char*)g_usart2_rx_buf,(const char*)"+QMTSUB: 0,1,0,1");//返OK
    }
    //Clear_Buffer();
*/		
}

void BC28_ALYIOTSenddata(u8 *len,u8 *data,u8 *data1,u8 *data2,u8 *data3,int *data4)//上发数据，上发的数据跟对应的插件有关系，用户需要注意插件然后对应数据即可
{
    int errcount = 0;
    memset(atstr,0,BUFLEN); //发送数据命令
    //AT+QMTPUB=0,0,0,0,"/sys/a18dtRetCT0/BC26TEST/thing/event/property/post"
    sprintf(atstr,"AT+QMTPUB=0,0,0,0,\"/sys/%s/%s/thing/event/property/post\"\n",PRODUCEKEY,DEVICENAME);
  //printf("AT+QMTPUB=0,0,0,0,\"/sys/%s/%s/thing/event/property/post\"\r\n",PRODUCEKEY,DEVICENAME);
  
    usart_send_str(USART2,atstr); 
  //printf("%s\n",g_usart2_rx_buf); 
    delay_ms(300);
	
		strx=strstr((const char*)g_usart2_rx_buf,(const char*)">");//返SEND OK
    while(strx==NULL)
    {
        errcount++;
        delay_ms(30);
        strx=strstr((const char*)g_usart2_rx_buf,(const char*)">");//返回OK
//        if(errcount>200)     //防止死循环
//        {
//            GPIO_SetBits(GPIOD,GPIO_Pin_6);		//模块重启
//            Delay(500);
//            GPIO_ResetBits(GPIOD,GPIO_Pin_6);
//            Delay(300);
//            NVIC_SystemReset();	//没有创建TCP SOCKET就重启系统等到服务器就绪
//        }
    }	
	
	
    memset(atstr,0,BUFLEN); //发送数据
    sprintf(atstr,"{params:{Temperature:%s,Humidity:%s,GasConcentration:%s,DetectDistance:%s,beep:%d}}\r\n",data,data1,data2,data3,data4);
    printf(" %s \r\n",atstr);
    usart_send_str(USART2,atstr);
    delay_ms(30);
   usart_send_hexbytes(0x1a);   //发送数据
    
    delay_ms(30);
    Clear_Buffer();
}


