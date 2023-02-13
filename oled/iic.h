/*������������stc15f104wϵ���Լ�89c51/c52ϵ�е�Ƭ��
*/
#include<reg52.h>
#include<intrins.h>
#include"oled.h"
typedef bit bool;

sbit sda=P1^1;//SDA�ӿ�
sbit scl=P1^0;//SCL�ӿ�
void delay_ms(uint time)//������ʱ
{
	uint x,y;
	for(y=0;y<time;y++)
		for(x=0;x<111;x++);
}
void delay_us()//5΢�����ʱ
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}
void iic_start()//��ʼiic
{
	scl=1;
	sda=1;
	delay_us();
	sda=0;
	delay_us();
	scl=0;
}
void iic_stop()//ֹͣiic
{
	scl=0; 
	sda=0;
	scl=1;
	delay_us();
	sda=1;
	delay_us();
}
bit checkack(void)//iic��Ӧ
{
	bit ack;
	scl=0;
	sda=1;
	delay_us();
	scl=1;
	delay_us();
	ack=sda;
	delay_us();
	scl=0;
	delay_us();
	return ack;
}
void write_bit(bool b)//д��λ
{
	if (b)
		sda=1;
	else
		sda=0;
	delay_us();
	scl=1;
	delay_us();
	scl=0;
}
bool read_bit(void)//��ȡ��λ
{
	bool b;
	sda=1;
	delay_us();
	scl=1;
	delay_us();
	b=sda;
	scl=0;
	return b;
}
void write_byte(bool sendstart,bool sendstop,uchar byte)//д�ֽ�
{
	int i;
	
	if(sendstart)
	{
		iic_start();
	}
	for(i=0;i<8;i++)
	{
		if(byte & 0x80)
			sda=1;
		else
			sda=0;scl=1;scl=0;
		byte <<= 1;
	}
	sda=1;scl=1;scl=0;
	if(sendstop)
	{
		iic_stop();
	}
	}
uchar read_byte(bool nack, bool sendstop)//��ȡ�ֽ�
{
	uchar byte=0;int i;
	for(i=0;i<8;i++)
	{
		byte=(byte<<1)|read_bit();
	}
	write_bit(nack);
	if(sendstop)
		iic_stop();
	return byte;
}
void iic_writecommand(uint iic_cmd)//д����
{
	write_byte(1,0,0x78);
	
	write_byte(0,0,0x00);
	
	write_byte(0,0,iic_cmd);
	
	iic_stop();
}
void iic_writedata(uchar iic_data)//д����
{
	write_byte(1,0,0x78);
	
	write_byte(0,0,0x40);
	
	write_byte(0,0,iic_data);
	
	iic_stop();
}
void oled_wr_byte(unsigned dat,unsigned cmd)//oledд�ֽ�
{
	if(cmd)
	{
		iic_writedata(dat);
	}
	else
	{
		iic_writecommand(dat);
	}
}
void fill_picture(uchar fill_data)//���ͼ��
{
	uchar m,n;
	for(m=0;m<8;m++)
	{
		oled_wr_byte(0xb0+m,0);
		oled_wr_byte(0x01,0);
		oled_wr_byte(0x10,0);
		for(n=0;n<128;n++)
		{
			oled_wr_byte(fill_data,1);
		}
	}
}
void oled_set_pos(uchar x,uchar y)//���ù��λ��
{
	oled_wr_byte(0xb0+y,0);
	oled_wr_byte(((x&0xf0)>>4)|0x10,0);
	oled_wr_byte((x&0x0f),0);
}
void oled_display_on(void)//����ʾ
{
	oled_wr_byte(0x8d,0);
	oled_wr_byte(0x14,0);
	oled_wr_byte(0xaf,0);
}
void oled_display_off(void)//����ʾ
{
	oled_wr_byte(0x8d,0);
	oled_wr_byte(0x10,0);
	oled_wr_byte(0xae,0);
}
void oled_clear(void)//����
{
	uchar i,n;
	for(i=0;i<8;i++)
	{
		oled_wr_byte(0xb0+i,0);
		oled_wr_byte(0x00,0);
		oled_wr_byte(0x10,0);
		for(n=0;n<128;n++)oled_wr_byte(0,1);
	}
}
void oled_on(void)//������ʾ
{
	uchar i,n;
	for(i=0;i<8;i++)
	{
		oled_wr_byte(0xb0+i,0);
		oled_wr_byte(0x00,0);
		oled_wr_byte(0x10,0);
		for(n=0;n<128;n++)oled_wr_byte(1,1);
	}
}
void oled_showchar(uchar x,uchar y,uchar chr,uchar char_size)//��ʾһ���ַ���x:0-127,y:0-63,size�����壬Ĭ����16����
{
	uchar c=0,i=0;
	c=chr-' ';
	if(x>128-1){x=0;y=y+2;}
	if(char_size==16)
	{
		oled_set_pos(x,y);
		for(i=0;i<8;i++)
		oled_wr_byte(F8X16[c*16+i],1);
		oled_set_pos(x,y+1);
		for(i=0;i<8;i++)
		oled_wr_byte(F8X16[c*16+i+8],1);
	}
	else {
		oled_set_pos(x,y);
		for(i=0;i<6;i++)
		oled_wr_byte(F6x8[c][i],1);
	}
}
uint oled_pow(uchar m,uchar n)//�η�����
{
	uint result=1;
	while(n--)result*=m;
	return result;
}
void oled_shownum(uchar x,uchar y,uint num,uchar len,uchar size2)//x,y:����,num��ֵ,lenλ��,size2�����壬Ĭ����16����
{
	uchar t,temp;
	uchar enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
	if(enshow==0&&t<(len-1))
	{
		if(temp==0)
		{
			oled_showchar(x+(size2/2)*t,y,' ',size2);
			continue;
		}else enshow=1;
	}
	oled_showchar(x+(size2/2)*t,y,temp+'0',size2);
	}
}
void oled_showstring(uchar x,uchar y,uchar *chr,uchar char_size)//��ʾһ���ַ���
{
	uchar j=0;
	while(chr[j]!='\0')
	{
		oled_showchar(x,y,chr[j],char_size);
    x+=8;
		if(x>120){x=0;y+=2;}
		j++;
	}
}
void oled_showchinese(uchar x,uchar y,uchar no)//��ʾ����,xyΪ���꣬noΪ���
{
	uchar t,adder=0;
	oled_set_pos(x,y);
	for(t=0;t<16;t++)
	{
		oled_wr_byte(HzK[2*no][t],1);
		adder+=1;
	}
	oled_set_pos(x,y+1);
	for(t=0;t<16;t++)
	{
		oled_wr_byte(HzK[2*no+1][t],1);
		adder+=1;
	}
}
void oled_drawbmp(uchar x0,uchar y0,uchar x1,uchar y1,uchar bmp[])//��ͼ
{
	uint j=0;
	uchar x,y;
	if(y1%8==0)y=y1/8;
	else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		oled_set_pos(x0,y);
		for(x=x0;x<x1;x++)
		{
			oled_wr_byte(bmp[j++],1);
		}
	}
}
void oled_init(void)//��ʼ��ssd1306
{
	delay_ms(500);//��ʱ�������У�
	oled_wr_byte(0xAE,0);//�ر�oled���
	oled_wr_byte(0x00,0);//���õ�λ�е�ַ
	oled_wr_byte(0x10,0);//���ø�λ�е�ַ
	oled_wr_byte(0x40,0);//������ʼ�е�ַ����ӳ��RAM��ʾ��ʼ�У�0x00~0x3F��
	oled_wr_byte(0x81,0);//--���öԱȶȿ��ƼĴ���
	oled_wr_byte(0xCF,0);//����SEG�����������
	oled_wr_byte(0xA1,0);//���÷ֶ�/��ӳ��0xa0���ҷ��� 0xa1������
	oled_wr_byte(0xC8,0);//����COM/��ɨ�跽��0xc0���·��� 0xc8������
	oled_wr_byte(0xA6,0);//����������ʾ
	oled_wr_byte(0xA8,0);//���ø��ñȣ�1��64��
	oled_wr_byte(0x3F,0);//1/64����
	oled_wr_byte(0xD3,0);//������ʾƫ��ӳ��RAM��������0x00~0x3F��
	oled_wr_byte(0x00,0);//������
	oled_wr_byte(0xD5,0);//������ʾʱ�ӷ�Ƶ��/����Ƶ��
	oled_wr_byte(0x80,0);//���÷�Ƶ�ȣ���ʱ������Ϊ100֡/��
	oled_wr_byte(0xD8,0);
	oled_wr_byte(0x05,0);
	oled_wr_byte(0xD9,0);//����Ԥ���ʱ��
	oled_wr_byte(0xF1,0);//��Ԥ�������Ϊ15��ʱ�ӣ��ŵ�����Ϊ1��ʱ��
	oled_wr_byte(0xDA,0);//����com����Ӳ������
	oled_wr_byte(0x12,0);//
	oled_wr_byte(0xDB,0);//����vcomh
	oled_wr_byte(0x40,0);//����VCOMȡ��ѡ�񼶱�
	oled_wr_byte(0x20,0);//����ҳ��Ѱַģʽ��0x00/0x01/0x02��
	oled_wr_byte(0x02,0);//
	oled_wr_byte(0x8D,0);//���õ�ɱ�����/����
	oled_wr_byte(0x14,0);//���ã�0x10������
	oled_wr_byte(0xA4,0);//����������ʾ�򿪣�0xa4/0xa5��
	oled_wr_byte(0xA6,0);//���÷�ת��ʾ�򿪣�0xa6/a7��
	oled_wr_byte(0xAF,0);//��oled���
	fill_picture(0x00);//��ʼ����
	oled_set_pos(0,0);//��ʼ���
}