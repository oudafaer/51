#include<reg51.h> //����ͷ�ļ�
#define uchar unsigned char	  //�궨��
#define uint unsigned int

void delay(uint ms) //������ʱ�Ӻ���
{
	uint i,j;
	for(i=ms;i>0;i--) //ͨ���ı�ms�Ĵ������ı���ʱʱ��	 
	  for(j=110;j>0;j--); 
}
	
void main()		 //������
{
	while(1)
	{
		P2 = 0Xfe;	  // 1111 1110
		P0 = 0X06;    // 0000 0110 ��ʾ1
		delay(500);

		P2 = 0Xfd;	  // 1111 1101
		P0 = 0X5b;    	// ��ʾ2
		delay(500);		

		P2 = 0Xfb;		// 1111 1011
		P0 = 0X4f;    	//��ʾ3
		delay(500);
						   
		P2 = 0Xf7;			// 1111 0111
		P0 = 0X66;    		//��ʾ4
		delay(500);			
		}
}
