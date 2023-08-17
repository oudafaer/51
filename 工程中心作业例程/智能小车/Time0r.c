#include "reg52.h"
 
/**
   * @brief  ��ʱ��0��1����@11.0592MHz
   * @param  ��
   * @retval ��
   */
void Timer0Init(void)		//1����@11.0592MHz
{
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	TL0 = 0x66;		//���ö�ʱ��ֵ
	TH0 = 0xFC;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;		//��ʱ������
	EA = 1;			//�ܿ���
}
/*ģ��
	void time_control() interrupt 1
	{
		static unsigned int counst;//�ֲ���������̬��������ֹÿ�ν����жϣ�����counst��Ϊ0
		TL0 = 0x66;		//���ö�ʱ��ֵ
		TH0 = 0xFC;		//���ö�ʱ��ֵ
		counst++;
		if (counst == 1000)
		{
			counst = 0;
			
		}
	
	}
*/
