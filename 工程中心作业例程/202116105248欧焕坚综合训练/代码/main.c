#include "reg52.h"
#include "IR.h"
#include "DS18B20.h"
#include "AT24C02.h"
#include "BMP.h"
#include "oled.h"

float T;
unsigned int Temp[4];
unsigned char Address,Command;
unsigned State;
unsigned StateT;

void main()
{
	OLED_Init();
	OLED_Clear();
	IR_Init();
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
			if(Command==IR_EQ)
			{
			AT24C02_WriteByte(0,Temp[0]%256);
			Delay_1ms(5);
			AT24C02_WriteByte(1,(Temp[1]%1000)%256);
			Delay_1ms(5) ;				
			}
			if(Command==IR_MODE)
			{
				OLED_Clear();
				StateT++;
				StateT%=2;
			}
		}
		if(State==0)
		{
			OLED_Clear();
		}
		if(State==1 && StateT==0)
		{
			OLED_ShowCHinese(0,0,0);
			OLED_ShowCHinese(18,0,1);
			OLED_ShowCHinese(36,0,2);
			OLED_ShowCHinese(54,0,3);				
				
			OLED_ShowCHinese(0,3,9);
			OLED_ShowCHinese(18,3,6);
			OLED_ShowChar(36,3,':',16);
			OLED_ShowNum(54,3,T,2,16);
			OLED_ShowChar(70,3,'.',16);
			OLED_ShowNum(80,3,(unsigned long)(T*10000)%10000,3,16);

			Temp[2]=AT24C02_ReadByte(0);
			Temp[3]=AT24C02_ReadByte(1);
	       	OLED_ShowCHinese(0,6,4);
			OLED_ShowCHinese(18,6,5);
			OLED_ShowChar(36,6,':',16);
			OLED_ShowNum(54,6,Temp[2],2,16);
			OLED_ShowChar(70,6,'.',16);
			OLED_ShowNum(80,6,Temp[3],3,16);
		}
		if(State==1 && StateT==1)
		{
			OLED_ShowCHinese(0,0,7);
			OLED_ShowCHinese(18,0,8);
			OLED_ShowCHinese(36,0,1);
			
				
			OLED_ShowCHinese(0,3,9);
			OLED_ShowCHinese(18,3,6);
			OLED_ShowChar(36,3,':',16);
			OLED_ShowNum(54,3,((T*1.8)+32),2,16);
			OLED_ShowChar(70,3,'.',16);
			OLED_ShowNum(80,3,((unsigned long)(T*10000)%10000*1.8),3,16);

			Temp[2]=AT24C02_ReadByte(0);
			Temp[3]=AT24C02_ReadByte(1);
	       	OLED_ShowCHinese(0,6,4);
			OLED_ShowCHinese(18,6,5);
			OLED_ShowChar(36,6,':',16);
			OLED_ShowNum(54,6,((Temp[2]*1.8)+32),2,16);
			OLED_ShowChar(70,6,'.',16);
			OLED_ShowNum(80,6,(Temp[3]*1.8),3,16);
		}
	}
}