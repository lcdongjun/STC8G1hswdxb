#include "IRM.h"


#define TIMER_SCALE (FOSC / 1000000) // 1us ��ʱ

volatile unsigned char ir_data[4];  // �洢 4 �ֽں�������
volatile bit ir_flag = 0;           // ���ݽ�����ɱ�־
volatile unsigned char bit_count = 0; // �ѽ���λ��

void delay_us(unsigned int us) {
    while (us--) {
        _nop_(); _nop_(); _nop_(); _nop_();  // ����ʱ
    }
}

void Timer0_Init() {
//    TMOD &= 0xF0; // ���T0ģʽ
//    TMOD |= 0x01; // T0 16λ����
		TMOD = 0x08; 
    TH0 = TL0 = 0; // ����
    TR0 = 1; // ����T0
}

void EXTI0_Init() {
    IT0 = 1; // �½��ش���
    EX0 = 1; // �����ⲿ�ж�0
    EA = 1;  // �����ж�
}

void Timer0_Reset() {
    TH0 = 0;
    TL0 = 0;
    TR0 = 1;  // ������ʱ��
}

unsigned int Timer0_Read() {
    TR0 = 0; // ��ֹͣ��ʱ
    return (TH0 << 8) | TL0;  // ��ȡ��ʱ��ֵ
}

// **�ⲿ�ж� 0 ������**
void exint0() interrupt 0 {
    unsigned int time = Timer0_Read();
    Timer0_Reset();  // ���ö�ʱ��
    if (time > 8500 && time < 9500) {  // 9ms �͵�ƽ (��ʼ�ź�)
        bit_count = 0;
    } 
    else if (time > 4000 && time < 5000) {  // 4.5ms �ߵ�ƽ (��ʼ�ź�)
        bit_count = 0;
    } 
    else if (time > 400 && time < 700) {  // 560��s �͵�ƽ������λ��
        // �ȴ��ߵ�ƽ
    } 
    else if (time > 400 && time < 700) {  // 560��s �ߵ�ƽ������ 0��
        ir_data[bit_count / 8] <<= 1;
        bit_count++;
    } 
    else if (time > 1500 && time < 1800) {  // 1.69ms �ߵ�ƽ������ 1��
        ir_data[bit_count / 8] <<= 1;
        ir_data[bit_count / 8] |= 1;
        bit_count++;
    }
		else
		{
			printf("time: %ld\r\n",time);
		}

    if (bit_count >= 32) {  // ������32λ����
        bit_count = 0;
        ir_flag = 1;
    }
}

void irm_init(void)
{
    Timer0_Init();
    EXTI0_Init();

}