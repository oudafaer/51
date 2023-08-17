#include <reg52.h> 
#define uint unsigned int						  //对数据类型进行声明定义
#define uchar unsigned char
//定义LED灯端口
sbit led1 = P1^0;
sbit led2 = P1^1;
sbit led3 = P1^2;
sbit led4 = P1^3;

//读取按键
uchar key_scanf()
{
   static int Key_on_off = 0 ;//按键自锁变量
   uchar  num , temp ;
   num = P2 ;            //将P2的值存在变量num中
   num &= 0xf0 ; 	//将低四位清0
   if(num != 0xf0)
   {
   	  if(Key_on_off == 0)
	  {
	  	 Key_on_off = 1 ;
		 switch(num)
		 {
		 	//返回按键的编码
		 	case 0xe0 : temp = 1 ; break ; 
			case 0xd0 : temp = 2 ; break ; 
			case 0xb0 : temp = 3 ; break ; 
			case 0x70 : temp = 4 ; break ; 
		 }
	  }   
   }
   else
   	 Key_on_off = 0 ;
	return temp  ;
}
//主函数
void main()
{
	int Key_value;
	led1=0;
	led2=0;
	led3=0;
	led4=0;
	while(1)
	{	
		Key_value=key_scanf();
		 switch(Key_value)
		 {
		 	//返回按键的编码
		 	case 1 : led1 = ~led1 ; break ; 
			case 2 : led2 = ~led2 ; break ; 
			case 3 : led3 = ~led3 ; break ; 
			case 4 : led4 = ~led4 ; break ; 
		 }
	}
}