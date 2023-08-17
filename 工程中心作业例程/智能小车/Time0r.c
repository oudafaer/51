#include "reg52.h"
 
/**
   * @brief  定时器0，1毫秒@11.0592MHz
   * @param  无
   * @retval 无
   */
void Timer0Init(void)		//1毫秒@11.0592MHz
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x66;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;		//定时器开关
	EA = 1;			//总开关
}
/*模板
	void time_control() interrupt 1
	{
		static unsigned int counst;//局部变量，静态变量，防止每次进入中断，都将counst置为0
		TL0 = 0x66;		//设置定时初值
		TH0 = 0xFC;		//设置定时初值
		counst++;
		if (counst == 1000)
		{
			counst = 0;
			
		}
	
	}
*/
