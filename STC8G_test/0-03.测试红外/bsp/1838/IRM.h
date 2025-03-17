#ifndef __IRM_H
#define __IRM_H

#include "config.h"


extern volatile unsigned char ir_data[4];  // 存储 4 字节红外数据
extern volatile bit ir_flag;           // 数据接收完成标志
extern volatile unsigned char bit_count; // 已接收位数

void irm_init(void);


#endif

