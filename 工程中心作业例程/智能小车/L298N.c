#include "reg52.h"
#include "L298N.h"

sbit IN1 = P1^0; // ����
sbit IN2 = P1^1; // ����
sbit IN3 = P1^2; // ����
sbit IN4 = P1^3; // ����
sbit ENA = P1^4; //ʹ��A
sbit ENB = P1^5; //ʹ��B

sbit out0 = P2^0; // �ұ߱��ϣ����ϰ�����������0
sbit out1 = P2^1; // ��߱��ϣ����ϰ�����������0

static unsigned char speed = 100; //�����ٶȳ�ʼֵ


void stop() // ͣ
{
	IN1=0;
	IN2=0;
	IN3=0;
	IN4=0;
}
 
void forward() // ��ǰ
{
	IN1=1;
	IN2=0;
	IN3=1;
	IN4=0;	  
}
 
void back() // ���
{
	IN1=0;
	IN2=1;
	IN3=0;
	IN4=1;	
}
 
void left() // ����ֻ�����ֶ�
{
	IN1=1;
	IN2=0;
	IN3=1;
	IN4=1;
}
 
void avoid_left()//����������ͬʱת��
{
	IN1=1;
	IN2=0;
	IN3=0;
	IN4=1;
}
 
void right() // ���ң�ֻ�����ֶ�
{
	IN1=1;
	IN2=1;
	IN3=1;
	IN4=0;
}
 
void avoid_right()//����������ͬʱת��
{
	IN1=0;
	IN2=1;
	IN3=1;
	IN4=0;
}
void increase(void)//����
{
	speed += 5; // ÿ������5
	if(speed >= 100) // ������100
	{
		speed = 100;
	}
}
	
void reduce(void)//����
{
	speed -= 5; // ÿ�μ���5
	if(speed <= 0) // ������0
	{
		speed = 0;
	}
}
 
void avoid(void)//����ģʽ
{
	if(out0 == 0) // �ұ����ϰ����ת
	{
		avoid_left();
	}
	if(out1 == 0) // ������ϰ����ת
	{
		avoid_right();
	}
	if(out0 == 1 && out1 == 1) // û���ϰ��ǰ��
	{
		 forward();
	}
	if(out0 == 0 && out1 == 0) // ���ϰ��ֹͣ
	{
		 stop();
	}
}