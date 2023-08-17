#include "reg52.h"
#include <stdio.h>

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

sbit LSA=P2^4;
sbit LSB=P2^5;
sbit LSC=P2^6;
sbit LSD=P2^7;

sbit CLOCK=P2^0;
sbit START=P2^1;
sbit EOC=P2^2;
sbit OE=P2^3;
u16 a;
u8 adc_number;
u8 xianshi[4];

void delay(unsigned int i)
{
	while(i--);	
}
void smg(u8 x,y)
{
	u8 b[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
	switch(x)
	{
		case 1:LSA=0;LSB=1;LSC=1;LSD=1;break;
		case 2:LSA=1;LSB=0;LSC=1;LSD=1;break;
		case 3:LSA=1;LSB=1;LSC=0;LSD=1;break;
		case 4:LSA=1;LSB=1;LSC=1;LSD=0;break;
	}
	P0=b[y];
	delay(100);
}
void TimeInitial()
{ 
	TMOD=0x10;
	TH1=(65536-200)/256;
	TL1=(65536-200)%256;
	EA=1;
	ET1=1;
	TR1=1;
 }

void datapros()
{
	START=0;
	OE=0;
	START=1;
	START=0;
	while(EOC==0);
	OE=1;
	adc_number=P1;
	OE=0;
	a=adc_number;
	xianshi[1]=a/1000+32;
	xianshi[2]=a%1000/100;
	xianshi[3]=a%100/10;
	xianshi[4]=a%10;
	
	smg(1,xianshi[1]);
	smg(2,xianshi[2]);
	smg(3,xianshi[3]);
	smg(4,xianshi[4]);
}

void t1(void) interrupt 3 
{
  TH1=(65536-200)/256;
  TL1=(65536-200)%256;
  CLOCK=~CLOCK;
}
void main()
{
	TimeInitial();
	datapros();
	while(1)
	{
		datapros();
	}
}
