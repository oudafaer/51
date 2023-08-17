#include "reg52.h"  //单片机头文件

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

//位定义
sbit k1=P3^4;
sbit k2=P3^5;
sbit k3=P3^6;
sbit k4=P3^7;

//函数声明
void Delay_ms(unsigned int xms);
void Uart_Init(void);
void Uart_Send_Byte(unsigned char c);
void keycontrol(void);

//主函数
void main()
{
	Uart_Init();
	while(1)
	{
		keycontrol();
	}
}
//延时函数
void Delay_ms(unsigned int xms)
{   
	unsigned char i,j;                        
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--);
}
//串口参数初始化设置函数
void Uart_Init(void)
{
    SCON=0x50;                  //串口工作方式1:10位异步收发器，1位起始位，8位数据位，1位停止位;REN=1:允许接收 
//    PCON=0x00;4800 9600                //SMOD=0:波特率不加倍 
    TMOD=0x20;                  //定时器T1工作方式2，8位自动重装载定时器/计数器，用于UART波特率 
    TH1=0xFD;                   //定时器初值设置0xFD，单片机晶振11.0592MHz，对应波特率9600
    TL1=0x00;                   //无需设置该位，保持为0x00
    TR1=1;	                    //开启定时器T1，允许T1计数 
	SM0=0;                      //设置串口工作方式
    SM1=1;
	ES=1;                       //串口中断允许位置1，开启串口中断
    EA=1;                       //总中断允许位，开启总中断
}
//串口发送一个字节函数
void Uart_Send_Byte(unsigned char c)
{
	SBUF = c;                     //将传入的数据赋值给发送缓冲器SBUF，将数据发送出去
	while(!TI);		                //等待数据发送完成，发送完成后发送中断标志位TI被硬件置1，跳出循环
	TI = 0;                       //发送中断标志位TI软件清零，为下次发送数据做准备
}
//按键控制
void keycontrol(void)
{
	unsigned char keynumber=0;
	if(k1==0)
	{
		Delay_ms(10);
		while(k1==0);
		Delay_ms(10);
		keynumber=1;
		Uart_Send_Byte(keynumber);
	}
		if(k2==0)
	{
		Delay_ms(10);
		while(k2==0);
		Delay_ms(10);
		keynumber=2;
		Uart_Send_Byte(keynumber);
	}
	if(k3==0)
	{
		Delay_ms(10);
		while(k3==0);
		Delay_ms(10);
		keynumber=3;
		Uart_Send_Byte(keynumber);
	}
	if(k4==0)
	{
		Delay_ms(10);
		while(k4==0);
		Delay_ms(10);
		keynumber=4;
		Uart_Send_Byte(keynumber);
	}
}