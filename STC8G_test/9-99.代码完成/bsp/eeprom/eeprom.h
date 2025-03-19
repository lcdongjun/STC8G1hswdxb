#ifndef __EEPROM_H
#define __EEPROM_H

#include "config.h"

#define FLASH_ADDR  0x0234  // Flash �洢���ݵĵ�ַ

char IapRead(int addr);
void IapProgram(int addr, char dat);
void IapErase(int addr);
void IapSafeWrite(int addr, char dat);


#endif

