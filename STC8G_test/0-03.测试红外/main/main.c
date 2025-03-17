#include "config.h"
#include "uart.h"
#include "IRM.h"

void delay_ms(unsigned char ms)
{
	unsigned int i;
	do{
		i = FOSC / 10000;
		while(--i);
	}while(--ms);
}

void System_Init(void)
{
    // P3.3 ��Ϊ DS18B20 �������ţ���ͨ IO��
    P3M1 &= ~(1 << 3); // ����Ϊ׼˫���
    P3M0 &= ~(1 << 3);
    
    // P5.4, P5.5 ��Ϊ��ͨIO�����������ܿ���
    P5M1 &= ~((1 << 4) | (1 << 5)); // ����Ϊ�������
    P5M0 |= ((1 << 4) | (1 << 5));
}

void main(void)
{
		P_SW2 |= 0x80;
	
    System_Init(); // ��ʼ��ϵͳ
		UartInit();
		irm_init();
		printf("System Run! \r\n");
		P54 = 0;
		P55 = 1;
    while (1)
    {
        if (ir_flag) {
            ir_flag = 0;
						printf("irm\r\n");
            UartSendStr(&ir_data[2]);
        }
    }
}
