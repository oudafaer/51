#include <reg52.h>
#include "delay.h"
#include "smg.h"

sbit k1 = P3^4;	 //����K1
sbit k2 = P3^5;  //����K2

unsigned char ms,s,m; //���룬�룬��

void Timer0Init()
{
	TMOD|=0X01;//ѡ��Ϊ��ʱ��0ģʽ��������ʽ1��

	TH0=0Xd8;	//����ʱ������ֵ����ʱ10ms
	TL0=0Xf0;	
	ET0=1;//�򿪶�ʱ��0�ж�����
	EA=1;//�����ж�
	TR0=1;//�򿪶�ʱ��			
}
void datapros()
{
	smg(1,m/10);
	smg1(2,m%10);
	smg(3,s/10);
	smg(4,s%10);

}

void Key_Process(void)
{
	if(k1 == 0)		  //��ⰴ��K1�Ƿ���
	{	
		delay(1000);   //����
		if(k1 == 0)	 //�ٴ��жϰ����Ƿ���
		{
			if(TR0 == 1)
				TR0 = 0; //�رն�ʱ��			
			else
				TR0 = 1; //�򿪶�ʱ��			
		}
		while(!k1);	 //��ⰴ���Ƿ��ɿ�
	}	
	else if(k2 == 0)
	{
		delay(1000);   //����
		if(k2 == 0)	 //�ٴ��жϰ����Ƿ���
		{
			TR0 = 0;//�رն�ʱ��	
			ms = 0;
			s = 0;
			m = 0;
		}
		while(!k2);	 //��ⰴ���Ƿ��ɿ�
	}
}


void main()
{	
	Timer0Init();  //��ʱ��0��ʼ��
	while(1)
	{
		datapros();	
		Key_Process();
	}		
}


void Timer0() interrupt 1
{
	TH0=0Xd8;	//����ʱ������ֵ����ʱ10ms
	TL0=0Xf0;
	ms++;
	if(ms>=100)  //1s
	{
		ms=0;
		s++;
		if(s>=60)
		{
			s=0;
			m++;
			if(m>=60)
			{
				m=0;
			}
		}	
	}	
}
