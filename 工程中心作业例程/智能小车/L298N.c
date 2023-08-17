#include "reg52.h"
#include "L298N.h"

sbit IN1 = P1^0; // 左上
sbit IN2 = P1^1; // 左下
sbit IN3 = P1^2; // 右上
sbit IN4 = P1^3; // 右上
sbit ENA = P1^4; //使能A
sbit ENB = P1^5; //使能B

sbit out0 = P2^0; // 右边避障，有障碍物就亮，输出0
sbit out1 = P2^1; // 左边避障，有障碍物就亮，输出0

static unsigned char speed = 100; //设置速度初始值


void stop() // 停
{
	IN1=0;
	IN2=0;
	IN3=0;
	IN4=0;
}
 
void forward() // 向前
{
	IN1=1;
	IN2=0;
	IN3=1;
	IN4=0;	  
}
 
void back() // 向后
{
	IN1=0;
	IN2=1;
	IN3=0;
	IN4=1;	
}
 
void left() // 向左，只有左轮动
{
	IN1=1;
	IN2=0;
	IN3=1;
	IN4=1;
}
 
void avoid_left()//向左，左右伦同时转动
{
	IN1=1;
	IN2=0;
	IN3=0;
	IN4=1;
}
 
void right() // 向右，只有右轮动
{
	IN1=1;
	IN2=1;
	IN3=1;
	IN4=0;
}
 
void avoid_right()//向左，左右伦同时转动
{
	IN1=0;
	IN2=1;
	IN3=1;
	IN4=0;
}
void increase(void)//加速
{
	speed += 5; // 每次增加5
	if(speed >= 100) // 上限是100
	{
		speed = 100;
	}
}
	
void reduce(void)//减速
{
	speed -= 5; // 每次减少5
	if(speed <= 0) // 下限是0
	{
		speed = 0;
	}
}
 
void avoid(void)//避障模式
{
	if(out0 == 0) // 右边有障碍物，左转
	{
		avoid_left();
	}
	if(out1 == 0) // 左边有障碍物，右转
	{
		avoid_right();
	}
	if(out0 == 1 && out1 == 1) // 没有障碍物，前进
	{
		 forward();
	}
	if(out0 == 0 && out1 == 0) // 有障碍物，停止
	{
		 stop();
	}
}