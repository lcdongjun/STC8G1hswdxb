#ifndef __IRM_H
#define __IRM_H

#include "config.h"


extern volatile unsigned char ir_data[4];  // �洢 4 �ֽں�������
extern volatile bit ir_flag;           // ���ݽ�����ɱ�־
extern volatile unsigned char bit_count; // �ѽ���λ��

void irm_init(void);


#endif

