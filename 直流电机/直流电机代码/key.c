#include <reg52.h>
#include "delay.h"

sbit k1=P3^4;
sbit k2=P3^5;
sbit k3=P3^6;
sbit k4=P3^7;
unsigned char Key()
{
	unsigned char keynumber=0;
	if(k1==0)
	{
		delay(1000);
		while(k1==0);
		delay(1000);
		keynumber=1;
	}
	if(k2==0)
	{
		delay(1000);
		while(k2==0);
		delay(1000);
		keynumber=2;
	}
	if(k3==0)
	{
		delay(1000);
		while(k3==0);
		delay(1000);
		keynumber=3;
	}
	if(k4==0)
	{
		delay(1000);
		while(k4==0);
		delay(1000);
		keynumber=4;
	}
	return keynumber;
}