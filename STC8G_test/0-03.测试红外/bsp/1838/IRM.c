#include "IRM.h"
#include "uart.h"

#define IR_PIN P32  // 1838 ���������ӵ� P3.2

volatile unsigned char ir_data[4];  // �洢��������
volatile bit ir_flag = 0;  // ���ݽ�����ɱ�־
volatile unsigned char bit_count = 0, byte_count = 0;

void Timer0_Init(void) {
//    TMOD &= 0xF0;  // �趨��ʱ��ģʽ
//    TMOD |= 0x01;  // ģʽ 1��16 λ��ʱ����
		AUXR |= (1<<7); 
		TMOD |= (1<<3);
		
    TH0 = 0;
    TL0 = 0;
    ET0 = 1;  // ʹ�ܶ�ʱ�� 0 �ж�
    EA = 1;   // ʹ��ȫ���ж�
    TR0 = 1;  // ������ʱ��
}

void INT0_Init(void) {
    IT0 = 1;  // �½��ش���
    EX0 = 1;  // ʹ���ⲿ�ж� 0
    EA = 1;   // ʹ��ȫ���ж�
}

void Timer0_ISR(void) interrupt 1 {
    TR0 = 0;
	
    if (bit_count < 32) {
        if (TH0 >= 0xFC)
            ir_data[byte_count] |= (1 << (7 - (bit_count % 8)));
        bit_count++;
        if (bit_count % 8 == 0) byte_count++;
    } else {
        ir_flag = 1;
        bit_count = 0;
        byte_count = 0;
    }
}

void INT0_ISR(void) interrupt 0 {
    TH0 = 0;
    TL0 = 0;
    TR0 = 1;
}

void irm_init(void)
{
    INT0_Init();
    Timer0_Init();

}