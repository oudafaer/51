#include "reg52.h"
#include "LCD1602.h"
#include "DS1302.h"

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;


 

void main()
{
	LcdInit();
	DS1302_Init();
	DS1302_SetTime();
	LCD_ShowString(1,1,"  _  _  ");
	LCD_ShowString(2,1,"  :  :  ");
	while(1)
	{
		DS1302_ReadTime();
		LCD_ShowNum(1,1,DS1302_Time[0],2);
		LCD_ShowNum(1,4,DS1302_Time[1],2);
		LCD_ShowNum(1,7,DS1302_Time[2],2);
		LCD_ShowNum(2,1,DS1302_Time[3],2);
		LCD_ShowNum(2,4,DS1302_Time[4],2);
		LCD_ShowNum(2,7,DS1302_Time[5],2);
	}
}