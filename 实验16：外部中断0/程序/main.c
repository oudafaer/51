/**************************************************************************************
实验现象：下载程序后，操作K3按键使D1状态取反
		  
接线说明： (具体接线图可见开发攻略对应实验的“实验现象”章节)
		   1，单片机-->LED&交通灯模块
		   		P20-->D1
		   2，单片机-->独立按键模块
		   		P32-->K3	
	
注意事项：																				

  
***************************************************************************************/

#include "reg52.h"			 //此文件中定义了单片机的一些特殊功能寄存器

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

sbit k3=P3^2;  //定义按键K3
sbit led=P1^0;	 //定义P20口是led

/****************************************** *************************************
* 函 数 名         : delay
* 函数功能		   : 延时函数，i=1时，大约延时10us
*******************************************************************************/
void delay(u16 i)
{
	while(i--);	
}

/*******************************************************************************
* 函 数 名         : Int1Init()
* 函数功能		   : 设置外部中断1
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Int0Init()
{
	//设置INT0
	IT0=1;//跳变沿出发方式（下降沿）
	EX0=1;//打开INT0的中断允许。	
	EA=1;//打开总中断	
}

/*******************************************************************************
* 函 数 名       : main
* 函数功能		 : 主函数
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void main()
{	
	Int0Init();  //	设置外部中断0
	while(1);		
}

/*******************************************************************************
* 函 数 名         : Int0()	interrupt 0
* 函数功能		   : 外部中断0的中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void Int0()	interrupt 0		//外部中断0的中断函数
{
	delay(1000);	 //延时消抖
	if(k3==0)
	{
		led=~led;
	}
}
