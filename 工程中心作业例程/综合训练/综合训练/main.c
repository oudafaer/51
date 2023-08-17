#include "reg52.h"
#include "IR.h"
#include "LCD1602.h"
#include "DS18B20.h"
#include "AT24C02.h"
#include "Delay.h"

float T;
unsigned int Temp[4];
unsigned char Address,Command;
unsigned State;
void main()
{
	IR_Init();
	LCD_Init();
	while(1)
	{
		Temp[0]=T;
		Temp[1]=T*100.0;
		if(IR_GetDataFlag() |IR_GetRepeatFlag())
		{
			Address=IR_GetAddress();
			Command=IR_GetCommand();
			if(Command==IR_0)
			{
				State=0;
			}
			if(Command==IR_1)
			{
				State=1;
			}
			if(Command==IR_2)
			{
				State=2;
			}
			if(Command==IR_3)
			{
				State=3;
			}			
		}
		if(State==0)
		{
			LCD_WriteCommand(0x01);//光标复位，清屏
		}		
		if(State==1)
		{
			DS18B20_ConvertT();
			T=DS18B20_ReadT();
			LCD_ShowNum(2,2,T,3);		//显示温度整数部分
			LCD_ShowChar(2,5,'.');		//显示小数点
			LCD_ShowNum(2,6,(unsigned long)(T*10000)%10000,4);//显示温度小数部分				
		}
		if(State==2)
		{
			AT24C02_WriteByte(0,Temp[0]%256);
			delay(5);
			AT24C02_WriteByte(1,(Temp[1]%100)%256);
			delay(5);		
		}		
		if(State==3)
		{
			Temp[2]=AT24C02_ReadByte(0);
			Temp[3]=AT24C02_ReadByte(1);
			LCD_ShowNum(2,2,Temp[2],3);		//显示温度整数部分
			LCD_ShowChar(2,5,'.');		//显示小数点
			LCD_ShowNum(2,6,Temp[3],2);//显示温度小数部分	
		}

	}
}
