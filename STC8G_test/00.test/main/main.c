#include <STC8G.h>
#include <stdio.h>
#include <intrins.h>

char putchar(char ch)
{
    SBUF = ch;//����1���ݼĴ���
    while(TI == 0);//����1�жϷ��������־
    TI = 0;
    return ch;
}

void Delay_us(unsigned int us)
{
    while (us--)
    {
        _nop_(); // ִ��һ���ղ���������1us @ 24MHz
    }
}

void Delay_ms(unsigned int ms)
{
    while (ms--)
    {
        Delay_us(1000); // ����΢����ʱ����
    }
}

void System_Init(void)
{
    // ����ϵͳʱ�ӣ�Ĭ���ڲ�ʱ��24MHz��
    CLKDIV = 0x00; // ��ϵͳ������Ĭ��ʱ��
    
    // ����1��ʼ�� (P3.0 RxD, P3.1 TxD)
    SCON = 0x50;  // 8λUART���������
    T2L = 0xF3;   // �������趨��115200bps @ 24MHz��
    T2H = 0xFF;
    AUXR |= 0x14; // ʹ��TIMER2��Ϊ�����ʷ�����

    // P3.2 ��Ϊ�ⲿ�ж�0�����պ����źţ�
    IT0 = 1;  // �½��ش���
    EX0 = 1;  // ʹ���ⲿ�ж� 0
    EA  = 1;  // �����ж�
    
    // P3.3 ��Ϊ DS18B20 �������ţ���ͨ IO��
    P3M1 &= ~(1 << 3); // ����Ϊ׼˫���
    P3M0 &= ~(1 << 3);
    
    // P5.4, P5.5 ��Ϊ��ͨIO�����������ܿ���
    P5M1 &= ~((1 << 4) | (1 << 5)); // ����Ϊ�������
    P5M0 |= ((1 << 4) | (1 << 5));
}

void main(void)
{
    System_Init(); // ��ʼ��ϵͳ
    
    printf("System Initialized!\n");
    
		P54 = 1;
		P55 = 0;
    while (1)
    {
				P54=~P54;
				P55=~P55;
        Delay_ms(500);
    }
}
