#include <STC8G.h>
#include <stdio.h>
#include <intrins.h>

char putchar(char ch)
{
    SBUF = ch;//串口1数据寄存器
    while(TI == 0);//串口1中断发送请求标志
    TI = 0;
    return ch;
}

void Delay_us(unsigned int us)
{
    while (us--)
    {
        _nop_(); // 执行一个空操作，近似1us @ 24MHz
    }
}

void Delay_ms(unsigned int ms)
{
    while (ms--)
    {
        Delay_us(1000); // 调用微秒延时函数
    }
}

void System_Init(void)
{
    // 设置系统时钟（默认内部时钟24MHz）
    CLKDIV = 0x00; // 让系统运行在默认时钟
    
    // 串口1初始化 (P3.0 RxD, P3.1 TxD)
    SCON = 0x50;  // 8位UART，允许接收
    T2L = 0xF3;   // 波特率设定（115200bps @ 24MHz）
    T2H = 0xFF;
    AUXR |= 0x14; // 使用TIMER2作为波特率发生器

    // P3.2 作为外部中断0（接收红外信号）
    IT0 = 1;  // 下降沿触发
    EX0 = 1;  // 使能外部中断 0
    EA  = 1;  // 开总中断
    
    // P3.3 作为 DS18B20 数据引脚（普通 IO）
    P3M1 &= ~(1 << 3); // 设置为准双向口
    P3M0 &= ~(1 << 3);
    
    // P5.4, P5.5 作为普通IO，控制三极管开关
    P5M1 &= ~((1 << 4) | (1 << 5)); // 设置为推挽输出
    P5M0 |= ((1 << 4) | (1 << 5));
}

void main(void)
{
    System_Init(); // 初始化系统
    
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
