#include "reg52.h"
#include "key.h"
#include "smg.h"
#include "Timer0.h"

sbit Motor=P1^0;

unsigned char Counter,Compare;
unsigned char KeyNum,Speed;

void main()
{
	Timer0Init();
	Compare=50;
	while(1)
	{
		KeyNum=Key();
		if(KeyNum==1)
		{
			Speed++;
			Speed%=4;
			if(Speed==0){Compare=0;}
			if(Speed==1){Compare=50;}
			if(Speed==2){Compare=70;}
			if(Speed==3){Compare=100;}
		}
		smg(1,Speed);
	}
}

void Timer0() interrupt 1
{
	TL0 = 0x9C;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	Counter++;
	Counter%=100;
	if(Counter<Compare)
	{
		Motor=1;
	}
	else
	{
		Motor=0;
	}
}
