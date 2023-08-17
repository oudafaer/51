#include "reg52.h"

void Delay1ms(unsigned int y)
{
	unsigned int x;
	for(y;y>0;y--)
		for(x=110;x>0;x--);
}