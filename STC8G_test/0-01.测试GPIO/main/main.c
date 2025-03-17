#include <STC8G.h>
#include <stdio.h>
#include <intrins.h>

char putchar(char ch)
{
    SBUF = ch;
    while(TI == 0);
    TI = 0;
    return ch;
}

void Delay_us(unsigned int us)
{
    while (us--)
    {
        _nop_();
    }
}

void Delay_ms(unsigned int ms)
{
    while (ms--)
    {
        Delay_us(1000);
    }
}

void System_Init(void)
{
    // ����ϵͳʱ�ӣ�Ĭ���ڲ�ʱ��24MHz��
    CLKDIV = 0x00; // ��ϵͳ������Ĭ��ʱ��
    
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
    EA = 1;
		P54 = 0;
		P55 = 1;
    while (1)
    {
				P54=~P54;
				P55=~P55;
        Delay_ms(1000);
    }
}
