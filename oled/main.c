#include"iic.h"

int main()
{
	oled_init();//OLED��ʼ��

//  delay_ms(3000);
//	oled_showstring(12,2,"Hello World!",32);//��ʾ�ַ���
//	oled_showstring(10,4,"perseverance",32);
//	oled_shownum(48,6,9527,4,16);//��ʾ����
//	oled_wr_byte(0x2e,0);//�ع���
//	oled_wr_byte(0x29,0);//����29�ң�2a��
//	oled_wr_byte(0x00,0);//���ֽ�
//  oled_wr_byte(0x04,0);//ˮƽ��ʼҳ
//	oled_wr_byte(0xcf,0);//ˮƽ�����ٶ�
//	oled_wr_byte(0x01,0);//ˮƽ����ҳ
//	oled_wr_byte(0x00,0);//ÿ�δ�ֱ����λ��
//	oled_wr_byte(0x2f,0);//������
//	delay_ms(3000);
//	oled_clear();
	oled_wr_byte(0x2e,0);//�ع���
	oled_wr_byte(0x2a,0);//����29�ҹ�����2a�����
	oled_wr_byte(0x00,0);//���ֽ�
    oled_wr_byte(0x00,0);//ˮƽ��ʼҳ
	oled_wr_byte(0x00,0);//ˮƽ�����ٶ�
	oled_wr_byte(0x07,0);//ˮƽ����ҳ
	oled_wr_byte(0x00,0);//ÿ�δ�ֱ����λ��
	oled_wr_byte(0xff,0);//�����ֽ�
	oled_showchinese(0,0,0);//��ʾ����
	oled_showchinese(16,0,1);
	oled_showchinese(32,0,2);
	oled_showchinese(48,0,3);
	oled_showchinese(64,0,4);
	oled_showstring(0,3,"Hello World!",32);//��ʾ�ַ���
	oled_showstring(0,4,"perseverance",32);
	oled_showstring(0,6,"9527",16);
	delay_ms(5);
	oled_wr_byte(0x2f,0);//������


  while (1){	;
	}
}