/*************************************************************************************
*
实验现象：下载程序后"继电器模块"的继电器吸合，DK1指示灯亮，可使用万用表测量NO端及COM端是否导通
接线说明： 单片机-->继电器模块(具体接线图可见开发攻略对应实验的“实验现象”章节)
		   P14-->J15	
注意事项：										
										  
**************************************************************************************
*/

#include "reg52.h"			 //此文件中定义了单片机的一些特殊功能寄存器

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

sbit relay=P1^4;	   


/*******************************************************************************
* 函 数 名       : main
* 函数功能		 : 主函数
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void main()
{	
	relay=0;
	while(1);
}
