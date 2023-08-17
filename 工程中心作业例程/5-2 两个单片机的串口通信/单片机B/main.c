#include "reg52.h"  //定义头文件

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

//位声明
sbit led0=P1^0;
sbit led1=P1^1;
sbit led2=P1^2;
sbit led3=P1^3;

//定义全局变量
u16 i;

//函数声明
void Uart_Init();

//串口参数初始化设置函数
void Uart_Init(void)
{
    SCON=0x50;                  //串口工作方式1:10位异步收发器，1位起始位，8位数据位，1位停止位;REN=1:允许接收 
//    PCON=0x00;4800 9600                //SMOD=0:波特率不加倍 
    TMOD=0x20;                  //定时器T1工作方式2，8位自动重装载定时器/计数器，用于UART波特率 
    TH1=0xFD;                   //定时器初值设置0xFD，单片机晶振11.0592MHz，对应波特率9600
    TL1=0x00;                   //无需设置该位，保持为0x00
    TR1=1;	                    //开启定时器T1，允许T1计数
    SM0=0;//设置串口工作方式
    SM1=1;
    REN=1;	
	ES=1;                       //串口中断允许位置1，开启串口中断
    EA=1;                       //总中断允许位，开启总中断
}
//主函数
void main()
{
	Uart_Init();
	while(1);
}
//中断服务函数
void uart() interrupt 4
{
	u8 receiveData;
	i=SBUF;                   
	 switch(i)
	{
		case 0:	//收到“0”
			led0=0;
			break;
		
		case 1:	//收到“1”
			led1=0;
			break;
		
		case 2:	//收到“2”
			led2=0;
			break;
		
		case 3:	//收到“3”
			led3=0;
			break;
		default:
			P2=0XFF;
			break;
	}
	receiveData=SBUF;        //出去接收到的数据
	RI=0;                    //清除接收中断标志位
}
