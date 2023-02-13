#include<reg51.h>
#include<picture.h>

sbit CS0  = P0^0;		//片选
sbit RST  = P0^1;		//复位
sbit RS   = P2^7; 		//数据命令选择端
sbit SCL  = P2^6;		//SPI时钟端
sbit SDA  = P2^5;		//SPI数据端


void Delay10ms(unsigned int c)   //误差 0us
{
    unsigned char a,b;
    for(;c>0;c--)
        for(b=38;b>0;b--)
            for(a=130;a>0;a--);
}


void  SendDataSPI(unsigned char dat)
{  
   unsigned char i;
   
   for(i=0; i<8; i++)			
   {  
      if( (dat&0x80)!=0 ) SDA = 1;
        else SDA = 0;

      dat <<= 1;

	  SCL = 0;
      SCL = 1;			
   }
}

void WriteComm(unsigned int i)
{

    CS0 = 0;
	RS  = 0;

	SendDataSPI(i);

	CS0 = 1;

}
void WriteData(unsigned int i)
{
    CS0 = 0;
	RS  = 1;

	SendDataSPI(i);

	CS0 = 1;
}

void LCD_Init(void)
{
	unsigned int i;
	//CS0=0;
	
	RST=1;  
	for(i = 0; i < 1000; i++);
	
	RST=0;
	for(i = 0; i < 1000; i++);

	RST=1;
	for(i = 0; i < 1000; i++);

    WriteComm(0xe2);		  //软件复位
	WriteComm(0xc8);		  //0xA0段方向选择正常方向（0xA1为反方向
	WriteComm(0xa0);		  //0xC8普通方向选择选择反向，0xC0为正常方向
	WriteComm(0x2f);
	WriteComm(0x26);
	WriteComm(0x81);		  //背景光对比度
	WriteComm(0x10);
	WriteComm(0xaf);		  //开启显示

}

void Clear()
{
	unsigned char i,j;
	
	for(i=0; i < 9; i++)
	{
		WriteComm(0xb0+i);
		WriteComm(0x10);
		WriteComm(0x00);
		
		for(j = 0; j < 96; j++)
		{
			WriteData(0x00);	
		}	
	}		
}

void Displine(void)			  //行扫屏
  {
   unsigned int i,k;

	for(i = 0 ; i <6; i++)
	{
    	WriteComm(0x40);  //set scroll line 	设置滚动一行
    	WriteComm(0xb0+i);//set page address		设置页地址
    	WriteComm(0x10); //column  msb		列最高位
    	WriteComm(0x00); //column  lsb		列最低位
    	for(k=0;k<132;k++)
    	{
	 	    WriteData(0XFf);
			  Delay10ms(1);
    	}

		for(k=0;k<132;k++)
    	{
	 	    WriteData(0X00);
			Delay10ms(1);
    	}
    }
	Clear();
}

void Clear_Lcd()
{
	unsigned char i,j;

	for(i = 0; i < 8; i++)
	{
		WriteComm(0XB0+i);
		WriteComm(0x10);
		WriteComm(0x04);
		for(j = 0; j < 96; j++)
		{
			WriteData(0xff);
			Delay10ms(1);								
		}	
	}

	for(i = 0; i < 8; i++)
	{
		WriteComm(0XB0+i);
		WriteComm(0x10);
		WriteComm(0x04);
		for(j = 0; j < 96; j++)
		{
			WriteData(0x00);
			Delay10ms(1);								
		}	
	}

}

void Dispgraphic(unsigned char *p)
    {
	   unsigned char i,j;
	
		   for(i=0;i<6;i++)
		   {
			    WriteComm(0x40);
			    WriteComm(0xb0|i);
			    WriteComm(0x10);
			    WriteComm(0x00);
			    for(j=0;j<128;j++)
				    {
				     WriteData(p[i*128+j]);
			         }
           }	 

}

void main()
{
	LCD_Init();
	Clear();
	while(1)
	{
		Dispgraphic(pic2);
		//DispLine();
		//Clear_Lcd();
	}	
}