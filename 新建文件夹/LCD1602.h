#ifndef __LCD1602_H_
#define __LCD1602_H_

void LcdWriteCom(unsigned char com); //写指令
void Lcd1602_Delay1ms(unsigned int c); //延时
void LcdInit();			//初始化
void LCD_ShowChar(unsigned char Line,unsigned char Column,unsigned char Char);
void LCD_ShowString(unsigned char Line,unsigned char Column,unsigned char String[]);
void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
void LCD_ShowSignedNum(unsigned char Line,unsigned char Column,int nunber,unsigned char Length);
void LCD_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
void ShowBinNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);

#endif