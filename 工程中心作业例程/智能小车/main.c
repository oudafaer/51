#include "reg52.h"
#include "Delay.h"
#include "Timer0.h"
#include "nixie.h"
#include "uart.h"
#include "L298N.h"

sbit ENA = P1^4; //ʹ��A
sbit ENB = P1^5; //ʹ��B

unsigned char PWM; //PWM��
unsigned char date;//���뻺����
extern  unsigned char speed = 100; //�����ٶȳ�ʼֵ


int main(void)	
{
	UsartConfiguration(); // ���ڳ�ʼ��
	Timer0Init();//��ʱ��0��ʼ��
	while(1)
	{
		//������ܿ��Կ�����ǰ���ٶ��Ƕ���
		Nixie(1, speed/100);
		delay(5);
		Nixie(2, (speed/10)%10);
		delay(5);
		Nixie(3, speed%10);
		delay(5);
		//��������ģʽ������ģʽ���������������
	}
}

//�����ж�
void Time_Int () interrupt 4
{
	if(RI == 1) // RIΪ1ʱ�����0
	{
		RI = 0; // ������ܱ�־
		date = SBUF; // �������ݻ�����date��
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
 //��ʱ��0
void time_control() interrupt 1
{
	TL0 = 0x66;		//���ö�ʱ��ֵ
	TH0 = 0xFC;		//���ö�ʱ��ֵ
	PWM++;
	if (PWM == 100)
	{
		PWM = 0;
	}
	if(PWM <= speed)//����PWM�����ʹ��
	{
		ENA = 1;
		ENB = 1;
	}
	if(PWM > speed)//С��PWM����ر�ʹ��
	{
		ENA = 0;
		ENB = 0;
	}
}
 