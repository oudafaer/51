#include <reg52.h> 
#define uint unsigned int						  //���������ͽ�����������
#define uchar unsigned char
//����LED�ƶ˿�
sbit led1 = P1^0;
sbit led2 = P1^1;
sbit led3 = P1^2;
sbit led4 = P1^3;

//��ȡ����
uchar key_scanf()
{
   static int Key_on_off = 0 ;//������������
   uchar  num , temp ;
   num = P2 ;            //��P2��ֵ���ڱ���num��
   num &= 0xf0 ; 	//������λ��0
   if(num != 0xf0)
   {
   	  if(Key_on_off == 0)
	  {
	  	 Key_on_off = 1 ;
		 switch(num)
		 {
		 	//���ذ����ı���
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
//������
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
		 	//���ذ����ı���
		 	case 1 : led1 = ~led1 ; break ; 
			case 2 : led2 = ~led2 ; break ; 
			case 3 : led3 = ~led3 ; break ; 
			case 4 : led4 = ~led4 ; break ; 
		 }
	}
}