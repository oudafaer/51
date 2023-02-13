#ifndef __IRRECEIVE_H__
#define __IRRECEIVE_H__

#include "delay.h"//请自行根据晶振编写相匹配的延时函数；

typedef struct{
	uchar usercode;//用户码
	uchar cmd;//操作码
	uchar ok;//完成标志位
	uchar irtime; //时间控制
	uint  count_repeat;//重复码计数器
	u32 receive;//数据缓存
}ir_value_t;

extern ir_value_t IrValue;//结构体声明

void irReceiveInit(void);//初始化
void IrValueParse(u32 recv);//数据处理函数

#endif
