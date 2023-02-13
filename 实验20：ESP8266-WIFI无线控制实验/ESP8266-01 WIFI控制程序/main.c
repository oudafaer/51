#include "public.h"			 
#include "temp.h"


sbit ESP8266_RST_Pin=P2^5;
sbit ESP8266_CH_PD_Pin=P2^6; 

sbit led1=P2^0;
sbit led2=P2^1;
sbit led3=P2^2;
sbit relay=P2^3;

#define RELOAD_COUNT 0xFA //宏定义波特率发生器的载入值 9600

#if 1
u8 RecBuf[50];
u8 recFlagOK=0;
#endif

u8 tempData[9];

void UART_Init()
{
	SCON=0X50;			//设置为工作方式1
	TMOD=0X20;			//设置计数器工作方式2
	PCON=0X80;			//波特率加倍
	TH1=RELOAD_COUNT;	//计数器初始值设置
	TL1=TH1;
	ES=0;				//关闭接收中断
	EA=1;				//打开总中断
	TR1=1;				//打开计数器
}

void UART_SendByte(u8 dat)
{
	ES=0; //关闭串口中断
	TI=0; //清发送完毕中断请求标志位
	SBUF=dat; //发送
	while(TI==0); //等待发送完毕
	TI=0; //清发送完毕中断请求标志位
	ES=1; //允许串口中断
}

void ESP8266_SendCmd(u8 *pbuf)
{
	while(*pbuf!='\0') //遇到空格跳出循环	
	{
		UART_SendByte(*pbuf);
		delay_10us(5);
		pbuf++;	
	}
	delay_10us(5);
	UART_SendByte('\r');//回车
	delay_10us(5);
	UART_SendByte('\n');//换行
	delay_ms(1000);
}

void ESP8266_SendData(u8 *pbuf)
{
	ESP8266_SendCmd("AT+CIPSEND=0,9");
	while(*pbuf!='\0') //遇到空格跳出循环	
	{
		UART_SendByte(*pbuf);
		delay_10us(5);
		pbuf++;	
	}
	delay_10us(5);
	UART_SendByte('\n');//换行	
	delay_ms(10);
}

//ESP8266-WIFI模块工作模式初始化
void ESP8266_ModeInit(void)
{
	ESP8266_RST_Pin=1;
	ESP8266_CH_PD_Pin=1;
	ESP8266_SendCmd("AT");
	ESP8266_SendCmd("AT+CWMODE=3");//设置路由器模式 1 staTIon模式 2 AP点 路由器模式 3 station+AP混合模式
	ESP8266_SendCmd("AT+CWSAP=\"PRECHIN\",\"prechin168\",11,0"); //设置WIFI热点名及密码
	ESP8266_SendCmd("AT+CIPAP=\"192.168.4.2\"");//重新启动wifi模块
	ESP8266_SendCmd("AT+RST");//重新启动wifi模块
	ESP8266_SendCmd("AT+CIPMUX=1");	//开启多连接模式，允许多个各客户端接入
	ESP8266_SendCmd("AT+CIPSERVER=1,8080");	//启动TCP/IP 端口为8080 实现基于网络控制	
}

//温度数据处理
#if 1
void TempDataPros(void)
{ 
	int tempdat=0;

	tempdat=Ds18b20ReadTemp()*10;
	if(tempdat<0)
	{
		tempData[0]='-';
	}		
	else
	{
		tempData[0]=' ';	
	}
			

	tempData[1]=tempdat / 10000+0x30;
	tempData[2]=tempdat % 10000 / 1000+0x30;
	tempData[3]=tempdat % 1000 / 100+0x30;
	tempData[4] ='.';
	tempData[5]=tempdat % 100 / 10+0x30;
	tempData[6]=tempdat % 10+0x30;
	tempData[7] ='C';
	tempData[8] ='\0';		
}	
#endif

void main()
{	
	u16 i=0;

	led1=led2=led3=1;
	relay=1;

	UART_Init(); //串口初始化
	ESP8266_ModeInit();
	ES=1; //允许串口中断
	while(1)
	{
		i++;
		if(i%10==0)
		{
			led3=!led3;
			TempDataPros();
			ESP8266_SendData(tempData);
		}
		delay_ms(10);
	}		
}

#if 1
void Uart() interrupt 4
{
	static u8 i=0;
	if(RI)
	{
		RI=0;
		RecBuf[i]=SBUF;	   //+IPD,0,1:F
		if(RecBuf[0]=='+')i++;
		else i=0;
		if(i==10)
		{
			i=0;
			if(RecBuf[0]=='+'&&RecBuf[1]=='I'&&RecBuf[2]=='P'&&RecBuf[3]=='D')
			{
				switch(RecBuf[9])
				{
					case 'F': led1=0;break;//点亮led1
					case 'B': led1=1;break;//关闭led1
					case 'L': relay=0;break;//继电器
					case 'R': relay=1;break;//
				}
			}
		}				
	}
						
}
#endif

