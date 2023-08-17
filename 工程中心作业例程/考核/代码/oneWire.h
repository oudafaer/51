#ifndef __ONEWIRE_H__
#define __ONEWIRE_H__

unsigned char Ds18b20Init();
void Ds18b20WriteByte(unsigned char dat);
unsigned char Ds18b20ReadByte();

#endif