#include "reg52.h"

void UART_init()    //���ô���ͨ�� ������Ϊ11.0592MHZ�������ľ��Լ���һ��Ӧ�����ö���
{
      TMOD = 0x20;
      TH1 = 0xfd;
      TL1 = 0xfd;  //������9600
      SM0 = 0;  
      SM1 = 1;   // ���ڹ�����ʽ1 10λ�첽
      REN = 1;  //�����������
      TR1 = 1;
      EA = 1;
      ES =1 ;  //�����ж�
}
/*
void UART_INTERRUPT() interrupt 4
{        
              pass = SBUF;
              RI = 0;   
              SBUF = pass;    //�ֻ�������ģ��ͨѶʱ�����ǿ��Դ򿪵��Դ��ڵ��Թ��߲鿴���ǵķ���ֵ��
              while(!TI);
              TI = 0;  
}
*/
