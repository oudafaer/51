/**************************************************************************************
实验现象：下载程序后，步进电机旋转
		  
接线说明： (具体接线图可见开发攻略对应实验的“实验现象”章节)
		   1，单片机-->四线双极性步进电机模块
		   		P10-->IA
				P11-->IB
				P12-->IC
				P13-->ID
		   2，四线双极性步进电机模块输出-->步进电机
		   		OA-->A+
				OB-->A-
				OC-->B+
				OD-->B-	
	
注意事项：																				
  
***************************************************************************************/

#include "reg52.h"			 //此文件中定义了单片机的一些特殊功能寄存器
#include<intrins.h>		//因为要用到左右移函数，所以加入这个头文件

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

sbit MOTOA = P1^0;
sbit MOTOB = P1^1;
sbit MOTOC = P1^2;
sbit MOTOD = P1^3;	

#define SPEED 200 	 //修改此值可改变电机旋转速度，不能过大或过小

/*******************************************************************************
* 函 数 名         : delay
* 函数功能		   : 延时函数，i=1时，大约延时10us
*******************************************************************************/
void delay(u16 i)
{
	while(i--);	
}


/*******************************************************************************
* 函 数 名       : main
* 函数功能		 : 主函数
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void main()
{	
	
	P1=0X00;
	while(1)
	{	
		MOTOA = 1;
		MOTOB = 0;
		MOTOC = 1;
		MOTOD = 1;
		delay(SPEED);

		MOTOA = 1;
		MOTOB = 1;
		MOTOC = 1;
		MOTOD = 0;
		delay(SPEED);

		MOTOA = 0;
		MOTOB = 1;
		MOTOC = 1;
		MOTOD = 1;
		delay(SPEED);

		MOTOA = 1;
		MOTOB = 1;
		MOTOC = 0;
		MOTOD = 1;
		delay(SPEED);
													
	}
}
