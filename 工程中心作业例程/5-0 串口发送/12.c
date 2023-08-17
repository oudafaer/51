#include <reg52.h>

//��������
void Uart_Init(void);
void Uart_Send_Byte(unsigned char c);
void Uart_send_string(unsigned char *dat);
//unsigned char Uart_Receive_Byte();  �����ַ�������
void Uart_action(void);

//LEDλ���壬�˸�LED��Ӧ�˸�IO��
sbit led0=P1^0;
sbit led1=P1^1;
sbit led2=P1^2;
sbit led3=P1^3;
sbit led4=P1^4;
sbit led5=P1^5;
sbit led6=P1^6;
sbit led7=P1^7;

//��ʼ������ֹͣ���ձ�־λ��bit ��λ����
bit Start_Receiving=0,Stop_Receiving=0;

//�������ݻ������鶨�壬���ڴ�ŴӴ��ڽ��յ���ָ������
unsigned char Buffer[10];   // ������

//������
void main()
{
	//�˸�LED��ʼ������Ϊ�ߵ�ƽ��Ϩ��״̬��
	led0 = 1;
	led1 = 1;
	led2 = 1;
	led3 = 1;
	led4 = 1;
	led5 = 1;
	led6 = 1;
	led7 = 1;
	
	//������ز�����ʼ������
	Uart_Init();
	
	//��ѭ��
	while(1)
	{
		Uart_action();              //���д��ڽ������ݷ�������Ӧ�Ķ���
	}
}

//���ڲ�����ʼ�����ú���
void Uart_Init(void)
{
    SCON=0x50;                  //���ڹ�����ʽ1:10λ�첽�շ�����1λ��ʼλ��8λ����λ��1λֹͣλ;REN=1:������� 
//    PCON=0x00;4800 9600                //SMOD=0:�����ʲ��ӱ� 
    TMOD=0x20;                  //��ʱ��T1������ʽ2��8λ�Զ���װ�ض�ʱ��/������������UART������ 
    TH1=0xFD;                   //��ʱ����ֵ����0xFD����Ƭ������11.0592MHz����Ӧ������9600
    TL1=0x00;                   //�������ø�λ������Ϊ0x00
    TR1=1;	                    //������ʱ��T1������T1���� 
	ES=1;                       //�����ж�����λ��1�����������ж�
    EA=1;                       //���ж�����λ���������ж�
}

//���ڷ���һ���ֽں���
void Uart_Send_Byte(unsigned char c)
{
	SBUF = c;                     //����������ݸ�ֵ�����ͻ�����SBUF�������ݷ��ͳ�ȥ
	while(!TI);		                //�ȴ����ݷ�����ɣ�������ɺ����жϱ�־λTI��Ӳ����1������ѭ��
	TI = 0;                       //�����жϱ�־λTI������㣬Ϊ�´η���������׼��
}

//���ڷ����ַ�������
void Uart_send_string(unsigned char *dat)  
{
	while(*dat != '\0')           //�жϵ�ǰָ�������Ƿ�Ϊ'\0':�ַ���������־���������ִ��ѭ���ڲ��������������ѭ��
	{
		Uart_Send_Byte(*dat);       //����1���ֽ����ݣ�1���ַ���СΪ1���ֽڣ�8λ����
		dat++;                      //��ַ��1��ָ����һ���ַ�
	}
}

////���ڽ���һ���ֽں���
//unsigned char Uart_Receive_Byte()
//{	
//	unsigned char dat;
//	while(!RI);	                //�ȴ����ݽ�����ɣ�������ɺ�����жϱ�־λRI��Ӳ����1������ѭ��
//	RI = 0;                     //�����жϱ�־λRI������㣬Ϊ������һ���ֽ�������׼��
//	dat = SBUF;                 //��ȡ���ջ�������ֵ������ֵ������dat
//	return (dat);               //�������ض�ȡֵ
//}

//�����жϷ������
void Uart_interrupt(void) interrupt 4 		
{
  static char i=0;              //���徲̬����i�����´ν���˺���ʱ��i��ֵ���䣬��ʾ�������ݵ�i���ֽ�
	unsigned char a=0;            //����������ݻ������
    if(RI)                      //�жϽ����жϱ�־λRI�Ƿ�Ϊ1��Ϊ1�����������һ���ֽڣ��������δ�������
	{                             
		RI=0;                       //�����жϱ�־λRI������㣬Ϊ������һ���ֽ�������׼��
		a=SBUF;                     //��ȡ���ջ�������ֵ������ֵ������dat
		                            
		if(a==0xD1)                 //�ж��Ƿ���յ�ָ�ͷ��һ���ֽ�0xD1������ʼ������������ݣ����򲻿�ʼ����
		{Start_Receiving=1;}        //��ʼ�������ݱ�־λ��1�����������ǿ�ʼ��������״̬
		                            
		else if(a==0x0D)	          //�ж��Ƿ���յ�ָ��������һ���ֽڻس�0x0D������ֹͣ���գ������������
		{                           
			Stop_Receiving=1;         //�����������ݱ�־λ��1�����������ǽ�����������״̬
			Start_Receiving=0;        //��ʼ�������ݱ�־λ��0�����������Ѿ����ٽ�������
			i=0;                      //�����ֽ�����0��Ϊ������һ��ָ����׼��
		}                           
                                
		if(Start_Receiving==1)      //�жϿ�ʼ�������ݱ�־λ�Ƿ�Ϊ1��Ϊ1������ǿ�ʼ��������״̬������ʼ���д�����ݣ�����������ٽ��պʹ������
		{                           
			 Buffer[i]=a;             //�����յ������ݴ���ڽ��ջ�������Buffer��
			 i++;                     //�����ֽ�����1				
		}
		
   }
}

//���ڶ�������
void Uart_action(void)
{
	unsigned char reply[]="receive success��\n";
	
  if(Stop_Receiving)            //�жϽ����������ݱ�־λ�Ƿ�Ϊ1��Ϊ1������ǽ�����������״̬������ʼ���з������ݣ�����������ٽ��պʹ������
	{
		Uart_send_string(reply);    //���ڷ����ַ���"receive success��\n"
    Stop_Receiving=0;           //ֹͣ���ձ�־λ���㣬Ϊ�´η���ָ��������׼��
		
		switch(Buffer[1])           //���ջ��������2���ֽ���ֵ����
		{
			//1.���ݽ��ջ��������2���ֽڵ���ֵ����ѡ��
			//2.���ݽ��ջ��������3���ֽڵ���ֵ������Ӧ�Ĳ�����Ϊ0�����LED��Ϊ��0ֵ��Ϩ��LED
      case 0x01: if(Buffer[2]==0){led0=0;} else{led0=1;} break;
      case 0x02: if(Buffer[2]==0){led1=0;} else{led1=1;} break;
      case 0x03: if(Buffer[2]==0){led2=0;} else{led2=1;} break;
      case 0x04: if(Buffer[2]==0){led3=0;} else{led3=1;} break;
      case 0x05: if(Buffer[2]==0){led4=0;} else{led4=1;} break;
      case 0x06: if(Buffer[2]==0){led5=0;} else{led5=1;} break;
      case 0x07: if(Buffer[2]==0){led6=0;} else{led6=1;} break;
      case 0x08: if(Buffer[2]==0){led7=0;} else{led7=1;} break;
			default: break;
    }
  }
}