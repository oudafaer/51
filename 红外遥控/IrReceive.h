#ifndef __IRRECEIVE_H__
#define __IRRECEIVE_H__

#include "delay.h"//�����и��ݾ����д��ƥ�����ʱ������

typedef struct{
	uchar usercode;//�û���
	uchar cmd;//������
	uchar ok;//��ɱ�־λ
	uchar irtime; //ʱ�����
	uint  count_repeat;//�ظ��������
	u32 receive;//���ݻ���
}ir_value_t;

extern ir_value_t IrValue;//�ṹ������

void irReceiveInit(void);//��ʼ��
void IrValueParse(u32 recv);//���ݴ�����

#endif
