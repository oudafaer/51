#include <reg52.h>
#include "delay.h"

sbit LSA=P2^4;
sbit LSB=P2^5;
sbit LSC=P2^6;
sbit LSD=P2^7;

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

void smg(u8 x,y)
{
	u8 a[17]={~0x3f,~0x06,~0x5b,~0x4f,~0x66,~0x6d,~0x7d,~0x07,~0x7f,~0x6f,~0x77,~0x7c,~0x39,~0x5e,~0x79,~0x71};
	switch(x)
	{
		case 1:LSA=0;LSB=1;LSC=1;LSD=1;break;
		case 2:LSA=1;LSB=0;LSC=1;LSD=1;break;
		case 3:LSA=1;LSB=1;LSC=0;LSD=1;break;
		case 4:LSA=1;LSB=1;LSC=1;LSD=0;break;
	}
	P0=a[y];
	delay(100);
	P0=0xff;
}