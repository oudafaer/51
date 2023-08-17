#ifndef __OLED_H_
#define __OLED_H_
 
//ÑÓÊ±
void delay_ma(unsigned int ms);
//oled¿ØÖÆº¯Êý
void Delay_50ms(unsigned int Del_50ms);
void Delay_1ms(unsigned int Del_1ms);
void OLED_Set_Pos(unsigned char x,unsigned char y);
void OLED_Open(void);
void OLED_Off(void);
void OLED_Clear(void);
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char Char,unsigned char Char_Size);
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned long num,unsigned char len,unsigned char size2);
void OLED_ShowString(unsigned char x,unsigned char y,unsigned char *chr,unsigned char Char_Size);
void OLED_ShowCHinese(unsigned char x,unsigned char y,unsigned char no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void OLED_Init(void);
void OLED_WR_Byte(unsigned dat,unsigned cmd);


#endif