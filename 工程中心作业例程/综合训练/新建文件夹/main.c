#include "reg52.h"
#include "IR.h"
#include "DS18B20.h"
#include "AT24C02.h"
#include "BMP.h"
#include "oled.h"

float T;
unsigned int Temp[4];
unsigned char Address,Command;
unsigned mod=0;
unsigned State;
unsigned StateT;

void main()
{
	IR_Init();
	OLED_Init();
	OLED_Clear();
	while(1)
	{
		T=DS18B20_ReadT();
		DS18B20_ConvertT();
		Temp[0]=T;
		Temp[1]=T*100.0;
		if(IR_GetDataFlag())
		{
			Address=IR_GetAddress();
			Command=IR_GetCommand();
			if(Command==IR_POWER)
			{
				State++;
				State%=2;
			}
			if(State==0)
			{
				OLED_Clear();
				break;
			}
			if(State==1)
			{
				if(Command==IR_MODE)
				{
					mod++;
					mod%=2;
				}
				if(mod==0)
				{
					OLED_DrawBMP(0,0,128,8,BMP1);
				}
				if(mod==1)
				{
					OLED_Clear();
					OLED_ShowCHinese(0,0,0);
					OLED_ShowCHinese(18,0,1);
					OLED_ShowCHinese(36,0,2);
					OLED_ShowCHinese(54,0,3);
					OLED_ShowCHinese(72,0,4);
					OLED_ShowCHinese(90,0,5);
					

					OLED_ShowCHinese(0,3,6);
					OLED_ShowCHinese(18,3,7);
					OLED_ShowCHinese(36,3,8);
					OLED_ShowCHinese(54,3,9);
					OLED_ShowChar(72,3,':',16);
					OLED_ShowNum(90,3,1,1,16);
					

					OLED_ShowCHinese(0,6,10);
					OLED_ShowCHinese(18,6,11);
					OLED_ShowCHinese(36,6,12);
					OLED_ShowCHinese(54,6,13);
					OLED_ShowChar(72,6,':',16);
					OLED_ShowNum(90,6,2,1,16);
					if(Command==IR_1)
					{
						OLED_Clear();
						StateT=1;
					}
					if(Command==IR_2)
					{
						StateT=2;
						OLED_Clear();
					}
					if(Command==IR_EQ)
					{
						StateT=3;
					}
				}
			}			
		}
		if(StateT==1)
		{
			Temp[2]=AT24C02_ReadByte(0);
			Temp[3]=AT24C02_ReadByte(1);
			OLED_ShowCHinese(0,0,6);
			OLED_ShowCHinese(18,0,7);
			OLED_ShowCHinese(36,0,8);
			OLED_ShowCHinese(54,0,9);
			OLED_ShowChar(72,0,':',16);
			OLED_ShowNum(80,3,Temp[2],2,16);
			OLED_ShowChar(98,3,'.',16);
			OLED_ShowNum(108,3,Temp[3],2,16);
			
		}
		if(StateT==2)
		{
			OLED_ShowCHinese(0,0,10);
			OLED_ShowCHinese(18,0,11);
			OLED_ShowCHinese(36,0,12);
			OLED_ShowCHinese(54,0,13);
			OLED_ShowChar(72,0,':',16);
			OLED_ShowNum(80,3,T,2,16);
			OLED_ShowChar(98,3,'.',16);
			OLED_ShowNum(108,3,(unsigned long)(T*10000)%10000,2,16);
		}
		if(StateT==3)
		{
			AT24C02_WriteByte(0,Temp[0]%256);
			Delay_1ms(5);
			AT24C02_WriteByte(1,(Temp[1]%100)%256);
			Delay_1ms(5);
		}
	}
}
