#ifndef __OLED_H_
#define __OLED_H_
 

//oled¿ØÖÆº¯Êý
void Delay_1ms(unsigned int Del_1ms);
void OLED_Clear(void);
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char Char,unsigned char Char_Size);
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned long num,unsigned char len,unsigned char size2);
void OLED_ShowCHinese(unsigned char x,unsigned char y,unsigned char no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void OLED_Init(void);

#endif