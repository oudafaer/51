#include <STC89C5xRC.H>
#include "intrins.h" 
unsigned int shuju;
void delay(unsigned int i)
{
	while(i--);
}
void UartInit(void)		//4800bps@12.000MHz
{
	PCON |= 0x80;		//使能波特率倍速位SMOD
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xF3;		//设定定时初值
	TH1 = 0xF3;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	EA=1;
	ES=1;
}

void UNRT_sendbyte(unsigned char byte)
{
	SBUF=byte;
	while(TI==0);
	TI=0;
}

void UNRT_accept() interrupt 4
{
	if(RI==1)
	{
		shuju=SBUF;
		RI=0;
	}
}
void main()
{
	UartInit();
	while(1)
	{
		delay(100000);
		UNRT_sendbyte(shuju);
		delay(100000);
	}
	
}
