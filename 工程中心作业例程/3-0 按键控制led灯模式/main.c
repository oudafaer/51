#include "reg52.h"
#include <intrins.h>
#include "key.h"
#include "timer0.h"

unsigned char keynum,ledmode;

void Timer0() interrupt 1
{
    static unsigned int T0count;
	TR0=0;
	T0count++;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	if(T0count>=500)
	{
		T0count=0;
		if(ledmode==0)
		{
			P1=_crol_(P1,1);
		}
		if(ledmode==1)
		{
			P1=_cror_(P1,1);
		}	
	}
	TR0=1;
}

void main()
{
	P1=0xfe;
	Timer0Init();
	while(1)
	{
		keynum=key();
		if(keynum)
		{
			if(keynum==1)
			{
				ledmode++;
				if(ledmode>=2)
				{
					ledmode=0;
				}
			}
		}
		
		
	}		
}