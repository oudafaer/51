#include "IrReceive.h"

void irReceiveInit(void)//初始化
{
  TMOD=0x02;//定时器0工作方式2，TH0是重装值，TL0是初值
  TH0=0; //重载值
  TL0=0; //初始化值
  ET0=1;    //开中断
  TR0=1;    
	
	IT0 = 1;   //指定外部中断0下降沿触发，INT0 (P3.2)
	EX0 = 1;   //使能外部中断
	EA = 1;    //开总中断
}

void IrValueParse(u32 recv)//数据处理函数
{
	uchar temp1;
	uchar temp2;
	temp1=recv;
	temp2=recv>>8;
	IrValue.usercode=temp1;
	if(temp1!=(uchar)(~temp2))
	{
		IrValue.ok=0;
		return;
	}
	temp1=recv>>16;
	temp2=recv>>24;
	IrValue.cmd=temp1;
	if(temp1!=(uchar)(~temp2))
	{
		IrValue.ok=0;
		return;
	}
	IrValue.ok=1;
}

ir_value_t IrValue;

void timer0_isr(void) interrupt 1 //定时器0中断服务函数
{
	IrValue.irtime++;
}

void ir_ex0_interrupt() interrupt 0 //外部中断0 下降沿触发
{   
		static bit flag_start=0;
		static uchar IrCountBit=0;     //位计数器
		if(IrValue.irtime<60&&IrValue.irtime>=40) 
		{
			flag_start=1;引导码 9ms+4.5ms 数据开始接收
			IrCountBit=0;//开始数数
			IrValue.count_repeat=0;//重发码清零
		}
		if(flag_start)
		{
			IrValue.receive>>=1;
			if(IrValue.irtime>6&&IrValue.irtime<10) //逻辑1
			{
				IrValue.receive|=0x80000000;
			}
			else if(IrValue.irtime<513&&IrValue.irtime>=268)
			{
				IrValue.count_repeat++;
			}
			IrCountBit++;
			if(IrCountBit==33)	//接收完成
			{
				IrValueParse(IrValue.receive);	
			}
		}
		IrValue.irtime=0;//清空定时器变量
}
