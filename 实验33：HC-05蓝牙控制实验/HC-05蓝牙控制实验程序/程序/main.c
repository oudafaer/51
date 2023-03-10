#include "reg52.h"    //此文件中定义了单片机的一些特殊功能寄存器

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;



#define USART_MAX_RECV_LEN	40
u8 USART_RX_BUF[USART_MAX_RECV_LEN];
u16 USART_RX_STA=0; 


void UsartInit()
{
	SCON=0X50;			//设置为工作方式1
	TMOD=0X20;			//设置计数器工作方式2
	PCON=0X80;			//波特率加倍
	TH1=0XFa;			//计数器初始值设置，注意波特率是9600的
	TL1=0XFa;
	ES=1;						//打开接收中断
	EA=1;						//打开总中断
	TR1=1;					//打开计数器
}

void USART_SendData(u8 datbuf[])
{
	u8 i=0;
	for(i=0;i<1;i++)
	{
		SBUF=datbuf[i];//将接收到的数据放入到发送寄存器
		while(!TI);//等待发送数据完成
		TI=0;//清除发送完成标志位	
	}
}

void main()
{
	UsartInit();  //串口初始化
	while(1)
	{	
		if(USART_RX_BUF[0]==0x01)
		{
			P1=0;
			USART_SendData(USART_RX_BUF);
		}
		else if(USART_RX_BUF[0]==0x02)
		{
			P1=1;
			USART_SendData(USART_RX_BUF);
		}		
	}
}

void Usart() interrupt 4
{
	u8 res;
	res=SBUF;
	USART_RX_BUF[0]=res;
	RI = 0;//清除接收中断标志位							 
}
