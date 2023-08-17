#include "reg52.h"
#include "Delay.h"
#include "Timer0.h"
#include "nixie.h"
#include "uart.h"
#include "L298N.h"

sbit ENA = P1^4; //使能A
sbit ENB = P1^5; //使能B

unsigned char PWM; //PWM波
unsigned char date;//输入缓冲区
extern  unsigned char speed = 100; //设置速度初始值


int main(void)	
{
	UsartConfiguration(); // 串口初始化
	Timer0Init();//定时器0初始化
	while(1)
	{
		//在数码管可以看到当前的速度是多少
		Nixie(1, speed/100);
		delay(5);
		Nixie(2, (speed/10)%10);
		delay(5);
		Nixie(3, speed%10);
		delay(5);
		//启动避障模式，将此模式存放在主函数当中
	}
}

//串口中断
void Time_Int () interrupt 4
{
	if(RI == 1) // RI为1时软件置0
	{
		RI = 0; // 清除接受标志
		date = SBUF; // 接收数据缓存在date中
		switch (date)
		{
			case ('1'):
			{
				forward();
				break;
			}
			case ('2'):
			{
				back();
				break;
			}
			case ('3'):
			{
				avoid_left();
				break;
			}
			case ('4'):
			{
				avoid_right();
				break;
			}
			case ('0'):
			{
				stop();
				break;
			}
			case ('5'):
			{
				increase();
				break;
			}
			case ('6'):
			{
				reduce();
				break;
			}			
			case ('7'):
			{
				increase();
				break;
			}
			case ('8'):
			{
				reduce();
				break;
			}
			
		}
	}
}
 //定时器0
void time_control() interrupt 1
{
	TL0 = 0x66;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	PWM++;
	if (PWM == 100)
	{
		PWM = 0;
	}
	if(PWM <= speed)//大于PWM波则打开使能
	{
		ENA = 1;
		ENB = 1;
	}
	if(PWM > speed)//小于PWM波则关闭使能
	{
		ENA = 0;
		ENB = 0;
	}
}
 