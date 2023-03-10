/**************************************************************************************
实验现象：下载程序后，按下独立按键可以点亮对应的led
		  	K1--D1
		  	K2--D2
			K3--D3
			... 
			K8--D8
接线说明： (具体接线图可见开发攻略对应实验的“实验现象”章节)
		   1，单片机-->LED&交通灯模块
		   		J22-->J19
		   2，单片机-->74HC165模块
		   		P15-->LD
		   		P36-->CLK
				P16-->QH
	           3，74HC165模块输出-->独立按键模块
		   		JP2-->JP1	
注意事项：																				  
***************************************************************************************/

#include "reg52.h"			 //此文件中定义了单片机的一些特殊功能寄存器
#include "intrins.h"

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

//--定义使用的IO口--//
#define GPIO_LED P0
sbit    IN_PL   = P1^6;    
sbit    IN_Data = P1^7;    //数据通过P1.7脚移进单片机内处理
sbit    SCK    = P3^6;
 



/*******************************************************************************
* 函 数 名         : Read74HC165
* 函数功能		   : 使用165读取一个字节数据
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

u8 Read74HC165(void)
{  
  u8 i;
  u8 indata;
		
   IN_PL = 0;
   _nop_();        //短暂延时 产生一定宽度的脉冲
   IN_PL = 1;	   //将外部信号全部读入锁存器中
   _nop_(); 
              
   indata=0;   //保存数据的变量清0  
   for(i=0; i<8; i++)
    { 
	  indata = indata<<1;	   //左移一位
	  SCK = 0;   //时钟置0	 
      _nop_();
	  indata |= IN_Data;
      SCK = 1;   //时钟置1	  
	} 
   
   return(indata);	 
}


/*******************************************************************************
* 函 数 名       : main
* 函数功能		 : 主函数
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void main()
{	
	u8 h165Value;

	GPIO_LED = 0xff;
	while(1)
	{
		h165Value = Read74HC165();
		if(h165Value != 0xFF)
		{
			GPIO_LED = h165Value;
		}	
	}			
}
