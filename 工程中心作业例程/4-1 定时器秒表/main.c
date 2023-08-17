#include <reg52.h>
#include "delay.h"
#include "smg.h"

sbit k1 = P3^4;	 //按键K1
sbit k2 = P3^5;  //按键K2

unsigned char ms,s,m; //毫秒，秒，分

void Timer0Init()
{
	TMOD|=0X01;//选择为定时器0模式，工作方式1。

	TH0=0Xd8;	//给定时器赋初值，定时10ms
	TL0=0Xf0;	
	ET0=1;//打开定时器0中断允许
	EA=1;//打开总中断
	TR0=1;//打开定时器			
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
	if(k1 == 0)		  //检测按键K1是否按下
	{	
		delay(1000);   //消抖
		if(k1 == 0)	 //再次判断按键是否按下
		{
			if(TR0 == 1)
				TR0 = 0; //关闭定时器			
			else
				TR0 = 1; //打开定时器			
		}
		while(!k1);	 //检测按键是否松开
	}	
	else if(k2 == 0)
	{
		delay(1000);   //消抖
		if(k2 == 0)	 //再次判断按键是否按下
		{
			TR0 = 0;//关闭定时器	
			ms = 0;
			s = 0;
			m = 0;
		}
		while(!k2);	 //检测按键是否松开
	}
}


void main()
{	
	Timer0Init();  //定时器0初始化
	while(1)
	{
		datapros();	
		Key_Process();
	}		
}


void Timer0() interrupt 1
{
	TH0=0Xd8;	//给定时器赋初值，定时10ms
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
