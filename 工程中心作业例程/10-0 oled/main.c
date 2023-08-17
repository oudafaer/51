#include "reg52.h"
#include "oled.h"

void main()
{
	OLED_Init();
	OLED_Clear();
	OLED_ShowCHinese(0,0,0);//温
	OLED_ShowCHinese(18,0,1);//度
	OLED_ShowCHinese(36,0,2);//检
	OLED_ShowCHinese(54,0,3);//测
	OLED_ShowCHinese(72,0,4);//实
	OLED_ShowCHinese(92,0,5);//验
	while(1)
	{
		OLED_ShowCHinese(0,0,0);//温
		OLED_ShowCHinese(18,0,1);//度
		OLED_ShowCHinese(36,0,2);//检
		OLED_ShowCHinese(54,0,3);//测
		OLED_ShowCHinese(72,0,4);//实
		OLED_ShowCHinese(92,0,5);//验
		
		OLED_ShowCHinese(0,3,6);//储
		OLED_ShowCHinese(18,3,7);//存
		OLED_ShowCHinese(36,3,8);//温
		OLED_ShowCHinese(54,3,9);//度
		
		OLED_ShowCHinese(0,6,10);//实
		OLED_ShowCHinese(18,6,11);//时
		OLED_ShowCHinese(36,6,12);//温
		OLED_ShowCHinese(54,6,13);//度

		
	}
}

