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
    // 设置系统时钟（默认内部时钟24MHz）
    CLKDIV = 0x00; // 让系统运行在默认时钟
    
    // P3.3 作为 DS18B20 数据引脚（普通 IO）
    P3M1 &= ~(1 << 3); // 设置为准双向口
    P3M0 &= ~(1 << 3);
    
    // P5.4, P5.5 作为普通IO，控制三极管开关
    P5M1 &= ~((1 << 4) | (1 << 5)); // 设置为推挽输出
    P5M0 |= ((1 << 4) | (1 << 5));
}

void main(void)
{
		P_SW2 |= 0x80;
	
    System_Init(); // 初始化系统
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
