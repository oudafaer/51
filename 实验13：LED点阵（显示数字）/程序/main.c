/**************************************************************************************
实验现象：下载程序后，LED点阵显示数字0
		  
接线说明： (具体接线图可见开发攻略对应实验的“实验现象”章节)
		   1，单片机-->74HC595模块
		   		P34-->SE
		   		P35-->RC
				P36-->SC
		   2，74HC595(A)模块输出-->LED双色点阵模块
		   		J27-->J6
		   3，74HC595(B)模块输出-->LED双色点阵模块
		   		J32-->J34或J28(双色)	
	
注意事项：																				

  
***************************************************************************************/

#include "reg51.h"			 //此文件中定义了单片机的一些特殊功能寄存器
#include "intrins.h"

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

//--定义使用的IO口--//
sbit SRCLK=P3^6;
sbit RCLK=P3^5;
sbit SER=P3^4;

//数字0取模数据
u8 ledduan[]={0x00,0x00,0x3E,0x41,0x41,0x41,0x3E,0x00};	//类似于动态数码管的段选

//心型图像取模数据
//u8 ledduan[]={0x1C,0x3E,0x7E,0xFC,0xFC,0x7E,0x3E,0x1C};	//类似于动态数码管的段选
u8 ledwei[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};	//类似于动态数码管的位选


/*******************************************************************************
* 函 数 名         : delay
* 函数功能		   : 延时函数，i=1时，大约延时10us
*******************************************************************************/
void delay(u16 i)
{
	while(i--);	
}

/*******************************************************************************
* 函 数 名         : Hc595SendByte(u8 dat1,u8 dat2)
* 函数功能		   : 通过595发送2个字节的数据
* 输    入         : dat1：第2个595输出数值
*                  * dat2: 第1个595输出数值
* 输    出         : 无
*******************************************************************************/
void Hc595SendByte(u8 dat1,u8 dat2)
{
	u8 a;

	SRCLK = 1;
	RCLK = 1;

	for(a=0;a<8;a++)		 //发送8位数
	{
		SER = dat1 >> 7;		 //从最高位开始发送
		dat1 <<= 1;

		SRCLK = 0;			 //发送时序
		_nop_();
		_nop_();
		SRCLK = 1;	
	}

	for(a=0;a<8;a++)		 //发送8位数
	{
		SER = dat2 >> 7;		 //从最高位开始发送
		dat2 <<= 1;

		SRCLK = 0;			 //发送时序
		_nop_();
		_nop_();
		SRCLK = 1;	
	}

	RCLK = 0;
	_nop_();
	_nop_();
	RCLK = 1;
}


/*******************************************************************************
* 函 数 名       : main
* 函数功能		 : 主函数
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void main()
{	
	u8 i=0;

	while(1)
	{
	   for(i=0;i<8;i++)
	   {
	   		Hc595SendByte(ledwei[i],ledduan[i]);
			delay(10);			
	   }
	}		
}
