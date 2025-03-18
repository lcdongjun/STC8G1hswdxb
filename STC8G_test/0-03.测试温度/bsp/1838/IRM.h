#ifndef __IRM_H
#define __IRM_H

#include "config.h"


extern volatile unsigned char ir_data[4];  // 存储接收数据
extern volatile bit ir_flag;  // 数据接收完成标志
extern volatile unsigned char bit_count, byte_count;


void irm_init(void);


#endif

