#include "reg52.h"
#include "oled.h"

void main()
{
	OLED_Init();
	OLED_Clear();
	OLED_ShowCHinese(0,0,0);//��
	OLED_ShowCHinese(18,0,1);//��
	OLED_ShowCHinese(36,0,2);//��
	OLED_ShowCHinese(54,0,3);//��
	OLED_ShowCHinese(72,0,4);//ʵ
	OLED_ShowCHinese(92,0,5);//��
	while(1)
	{
		OLED_ShowCHinese(0,0,0);//��
		OLED_ShowCHinese(18,0,1);//��
		OLED_ShowCHinese(36,0,2);//��
		OLED_ShowCHinese(54,0,3);//��
		OLED_ShowCHinese(72,0,4);//ʵ
		OLED_ShowCHinese(92,0,5);//��
		
		OLED_ShowCHinese(0,3,6);//��
		OLED_ShowCHinese(18,3,7);//��
		OLED_ShowCHinese(36,3,8);//��
		OLED_ShowCHinese(54,3,9);//��
		
		OLED_ShowCHinese(0,6,10);//ʵ
		OLED_ShowCHinese(18,6,11);//ʱ
		OLED_ShowCHinese(36,6,12);//��
		OLED_ShowCHinese(54,6,13);//��

		
	}
}

