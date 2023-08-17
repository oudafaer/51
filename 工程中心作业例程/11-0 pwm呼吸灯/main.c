#include "reg52.h"

sbit LED=P1^0;

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

void Delay(unsigned int t)
{
	while(t--);
}

void main()
{
	unsigned char Time,i;
	while(1)
	{
		for(Time=0;Time<100;Time++)
		{
			for(i=0;i<20;i++)
			{
				LED=0;
				Delay(Time);
				LED=1;
				Delay(100-Time);
			}
			
		}
		for(Time=100;Time>0;Time-- )
		{
			for(i=0;i<20;i++)
			{
				LED=0;
				Delay(Time);
				LED=1;
				Delay(100-Time);
			}
			
		}		
	}
}