#include "reg52.h"

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

u8 Date=0;

//函数声明
void UartInit(void);		//串口初始化配置

/*串口初始化配置*/
void UartInit(void)		//9600bps@11.0592MHz
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xFD;		//设定定时初值
	TH1 = 0xFD;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	EA=1;
	ES=1;
}


/*主函数*/
void main()
{
    UartInit();
    while(1)
	{
		if(Date==0x01)
		{
			P1=0x00;
		}
		if(Date==0x02)
		{
			P1==0x02;
		}
	}
}

/*中断函数*/
void Uart() interrupt 4
{
    Date=SBUF;        //取出接受到的数据
    RI=0;			  //清除接受中断标志位
	while(!TI);
	TI=0;
    //收到的数据是date
}
