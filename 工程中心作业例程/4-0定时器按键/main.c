#include "reg52.h"
#include <intrins.h>
#include "key.h"
#include "timer0.h"
#include "delay.h"
#include "smg.h"
 
sbit k1=P3^4;
sbit k2=P3^5;
sbit k3=P3^6;
sbit k4=P3^7;

unsigned char keynum,ledmode,shudu,zt,temp=5,pmode,kg=0xff;

void Timer1() interrupt 1
{
    static unsigned int T1count;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	T1count++;
		if(k3==0)
	{
		delay(1000);
		while(k3==0);
		delay(1000);
	}
	if(k4==0)
	{
		delay(1000);
		while(k4==0);
		delay(1000);
		pmode=ledmode;
		ledmode=temp;
		temp=pmode;
		P1=kg;
		kg=zt;
	}
	if(T1count>=(500-shudu*200))
	{
		T1count=0;
		if(ledmode==0)
		{
			P1=_crol_(P1,1);
		}
		if(ledmode==1)
		{
			P1=_cror_(P1,1);
		}
		if(ledmode==2)
		{
			P1=P1<<1;
		}
		if(ledmode==3)
		{
			P1=P1>>1;
		}
	}
}
void main()
{
	P1=0xfe;
	Timer0Init();
	while(1)
	{
		smg(1,ledmode);
		keynum=key();
		if(keynum==1)
		{
			if(keynum==1)
			{
				ledmode++;
				if(ledmode>=4)
				{
					ledmode=0;
				}
			}
		}
		if(keynum==2)
		{
			if(keynum==2)
			{
				shudu++;
				if(shudu>=3)shudu=0;
			}
		}
        if(P1==0x00)
		{
			P1=0xfe;
		}
	}		
}