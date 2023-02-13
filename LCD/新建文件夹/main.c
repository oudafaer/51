#include "reg52.h"
#include "LCD1602.h"
 
void main()
{
	LcdInit();
	LCD_ShowChar(1,1,'A');
	LCD_ShowString(1,3,"Hello");
	LCD_ShowNum(1,9,66,2);
	LCD_ShowSignedNum(1,12,-88,2);
	LCD_ShowHexNum(2,1,0xA5,2);
	ShowBinNum(2,4,0xa5,8);
	LCD_ShowChar(2,14,'C');
	LCD_ShowString(1,3,"welcome to China");
	while(1)
	{
		LcdWriteCom(0x18);
		Lcd1602_Delay1ms(50);
	}
}