#include"iic.h"

int main()
{
	oled_init();//OLED初始化

//  delay_ms(3000);
//	oled_showstring(12,2,"Hello World!",32);//显示字符串
//	oled_showstring(10,4,"perseverance",32);
//	oled_shownum(48,6,9527,4,16);//显示数字
//	oled_wr_byte(0x2e,0);//关滚动
//	oled_wr_byte(0x29,0);//方向，29右，2a左
//	oled_wr_byte(0x00,0);//空字节
//  oled_wr_byte(0x04,0);//水平起始页
//	oled_wr_byte(0xcf,0);//水平滚动速度
//	oled_wr_byte(0x01,0);//水平结束页
//	oled_wr_byte(0x00,0);//每次垂直滚动位移
//	oled_wr_byte(0x2f,0);//开滚动
//	delay_ms(3000);
//	oled_clear();
	oled_wr_byte(0x2e,0);//关滚动
	oled_wr_byte(0x2a,0);//方向，29右滚动，2a左滚动
	oled_wr_byte(0x00,0);//空字节
    oled_wr_byte(0x00,0);//水平起始页
	oled_wr_byte(0x00,0);//水平滚动速度
	oled_wr_byte(0x07,0);//水平结束页
	oled_wr_byte(0x00,0);//每次垂直滚动位移
	oled_wr_byte(0xff,0);//虚拟字节
	oled_showchinese(0,0,0);//显示汉字
	oled_showchinese(16,0,1);
	oled_showchinese(32,0,2);
	oled_showchinese(48,0,3);
	oled_showchinese(64,0,4);
	oled_showstring(0,3,"Hello World!",32);//显示字符串
	oled_showstring(0,4,"perseverance",32);
	oled_showstring(0,6,"9527",16);
	delay_ms(5);
	oled_wr_byte(0x2f,0);//开滚动


  while (1){	;
	}
}