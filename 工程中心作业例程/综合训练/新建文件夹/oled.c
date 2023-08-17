#include "oled.h"
#include "oledfont.h"
#include "reg52.h"

sbit OLED_SCL=P1^0;//时钟 D0SCLK?
sbit OLED_SDIN=P1^1;//D1（MOSI） 数据

void delay_ma(unsigned int ms)
{
	unsigned int a;
	while(ms)
	{
		a=1800;
		while(a--);
		ms--;
	}
	return;
}

//iic start
void IIC_Start()
{
	OLED_SCL=1;
	OLED_SDIN=1;
	OLED_SDIN=0;
	OLED_SCL=0;
	
}

void IIC_Stop()
{
	OLED_SCL=1;
	OLED_SDIN=0;
	OLED_SDIN=1;
}

void IIC_Wait_Ack()
{
	OLED_SCL=1;
	OLED_SCL=0;
}
//iic write byte
void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	OLED_SCL=0;
	for(i=0;i<8;i++)
	{
		m=da;
		m=m&0x80;
		if(m==0x80)
		{
			OLED_SDIN=1;
		}
		else OLED_SDIN=0;
		da=da<<1;
		OLED_SCL=1;
		OLED_SCL=0;
	}
}
//iic write command
void Write_IIC_Command(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78);
	IIC_Wait_Ack();
	Write_IIC_Byte(0x00);
	IIC_Wait_Ack();
	Write_IIC_Byte(IIC_Command);
	IIC_Wait_Ack();
	IIC_Stop();
}
//iic weite Data
void Write_IIC_Data(unsigned char IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(0x78);
	IIC_Wait_Ack();
	Write_IIC_Byte(0x40);
	IIC_Wait_Ack();
	Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();
	IIC_Stop();
}
//写命令或数据
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
	{
		Write_IIC_Data(dat);
	}
	else
	{
		Write_IIC_Command(dat);
	}
}
// 插入图片
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);		//page0-page1
		OLED_WR_Byte(0x00,0);		//low column start address
		OLED_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)
			{
				OLED_WR_Byte(fill_Data,1);
			}
	}
}
//延时函数
void Delay_50ms(unsigned int Del_50ms)
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
		for(m=6245;m>0;m--);
}
void Delay_1ms(unsigned int Del_1ms)
{
	unsigned char j;
	while(Del_1ms--)
	{	
		for(j=0;j<123;j++);
	}
}
//坐标设置
void OLED_Set_Pos(unsigned char x,unsigned char y)
{
	OLED_WR_Byte(0xb0+y,0);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,0);
	OLED_WR_Byte((x&0x0f),0);
}
//oled显示
void OLED_Open(void)
{
	OLED_WR_Byte(0X8D,0); 
	OLED_WR_Byte(0X14,0);  
	OLED_WR_Byte(0XAF,0);  
}

void OLED_Off(void)
{
	OLED_WR_Byte(0X8D,0);  
	OLED_WR_Byte(0X10,0); 
	OLED_WR_Byte(0XAF,0);  
}

//清屏函数  
void OLED_Clear(void)  
{  
	unsigned char i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,0);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,0);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,0);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
		{
			OLED_WR_Byte(0,1); 
		} 
	} //更新显示
}
//指定位置显示字符
//x:0~27
//y:0~63
//Size:选择字体 16/12
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>128-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],1);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],1);
			}
			else {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],1);
				
			}
}
//m*n函数
unsigned long oled_pow(unsigned char m,unsigned char n)
{
	unsigned long result=1;
	while(n--)result*=m;
	return result;
}
//显示两个数字
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned long num,unsigned char len,unsigned char size2)
{         	
	unsigned char t,temp;
	unsigned char enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//显示一个字符号串
void OLED_ShowString(unsigned char x,unsigned char y,unsigned char *chr,unsigned char Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//显示汉字
void OLED_ShowCHinese(unsigned char x,unsigned char y,unsigned char no)
{      			    
	unsigned char t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],1);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],1);
				adder+=1;
      }					
}
//显示图片
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],1);	    	
	    }
	}
} 

//初始化SSD1306					    
void OLED_Init(void)
{
	OLED_WR_Byte(0xAE,0);//--关闭显示
	OLED_WR_Byte(0x00,0);
	OLED_WR_Byte(0x10,0);
	OLED_WR_Byte(0x40,0);
	OLED_WR_Byte(0xB0,0);
	OLED_WR_Byte(0x81,0); // 对比度设置
	OLED_WR_Byte(0xff,0);//--128   
	OLED_WR_Byte(0xA1,0);
	OLED_WR_Byte(0xA6,0);
	OLED_WR_Byte(0xA8,0);
	OLED_WR_Byte(0x3F,0);
	OLED_WR_Byte(0xC8,0);
	OLED_WR_Byte(0xD3,0);
	OLED_WR_Byte(0x00,0);
	
	OLED_WR_Byte(0xD5,0);
	OLED_WR_Byte(0x80,0);
	
	OLED_WR_Byte(0xD8,0);
	OLED_WR_Byte(0x05,0);
	
	OLED_WR_Byte(0xD9,0);
	OLED_WR_Byte(0xF1,0);
	
	OLED_WR_Byte(0xDA,0);
	OLED_WR_Byte(0x12,0);
	
	OLED_WR_Byte(0xDB,0);
	OLED_WR_Byte(0x30,0);
	
	OLED_WR_Byte(0x8D,0);
	OLED_WR_Byte(0x14,0);
	
	OLED_WR_Byte(0xAF,0);
} 
