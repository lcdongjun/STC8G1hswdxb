#ifndef __IRM_H
#define __IRM_H

#include "config.h"


extern volatile unsigned char ir_data[4];  // �洢��������
extern volatile bit ir_flag;  // ���ݽ�����ɱ�־
extern volatile unsigned char bit_count, byte_count;


void irm_init(void);


#endif

