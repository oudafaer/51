#include "reg52.h"    //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;


sbit led1=P2^1;	   
sbit led2=P2^2;
sbit beep=P1^5;	
sbit STA=P1^0;

#define USART_MAX_RECV_LEN	40
u8 USART_RX_BUF[USART_MAX_RECV_LEN];
u16 USART_RX_STA=0; 

void delay(u16 i)
{
	while(i--);
}

void UsartInit()
{
	SCON=0X50;			//����Ϊ������ʽ1
	TMOD=0X20;			//���ü�����������ʽ2
	PCON=0X80;			//�����ʼӱ�
	TH1=0XFa;			//��������ʼֵ���ã�ע�Ⲩ������9600��
	TL1=0XFa;
	ES=1;						//�򿪽����ж�
	EA=1;						//�����ж�
	TR1=1;					//�򿪼�����
}

void USART_SendData(u8 datbuf[])
{
	u8 i=0;
	for(i=0;i<1;i++)
	{
		SBUF=datbuf[i];//�����յ������ݷ��뵽���ͼĴ���
		while(!TI);//�ȴ������������
		TI=0;//���������ɱ�־λ	
	}
}

void main()
{
	u8 i=0,j=0;
	STA=0;

	UsartInit();  //���ڳ�ʼ��
	while(1)
	{	
		if(USART_RX_BUF[0]==0x11)
		{
			led1=0;
			USART_SendData(USART_RX_BUF);
		}
		else if(USART_RX_BUF[0]==0x10)
		{
			led1=1;
			USART_SendData(USART_RX_BUF);
		}
		if(USART_RX_BUF[0]==0x21)
		{
			beep=0;
//			j=20;
//			while(j--)
//			{
//				beep=!beep;
//				delay(50);	
//			}
			USART_SendData(USART_RX_BUF);
		}
		else if(USART_RX_BUF[0]==0x20)
		{
			beep=1;
			USART_SendData(USART_RX_BUF);
		}
		i++;
		if(i%200==0)
		{
			led2=!led2;
		}
		delay(100);		
	}		
}

void Usart() interrupt 4
{
	u8 res;
	res=SBUF;
	USART_RX_BUF[0]=res;
	RI = 0;//��������жϱ�־λ							 
}
