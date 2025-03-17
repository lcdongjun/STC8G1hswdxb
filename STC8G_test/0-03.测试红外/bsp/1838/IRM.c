#include "IRM.h"


#define TIMER_SCALE (FOSC / 1000000) // 1us 计时

volatile unsigned char ir_data[4];  // 存储 4 字节红外数据
volatile bit ir_flag = 0;           // 数据接收完成标志
volatile unsigned char bit_count = 0; // 已接收位数

void delay_us(unsigned int us) {
    while (us--) {
        _nop_(); _nop_(); _nop_(); _nop_();  // 简单延时
    }
}

void Timer0_Init() {
//    TMOD &= 0xF0; // 清空T0模式
//    TMOD |= 0x01; // T0 16位计数
		TMOD = 0x08; 
    TH0 = TL0 = 0; // 清零
    TR0 = 1; // 启动T0
}

void EXTI0_Init() {
    IT0 = 1; // 下降沿触发
    EX0 = 1; // 允许外部中断0
    EA = 1;  // 开总中断
}

void Timer0_Reset() {
    TH0 = 0;
    TL0 = 0;
    TR0 = 1;  // 启动定时器
}

unsigned int Timer0_Read() {
    TR0 = 0; // 先停止计时
    return (TH0 << 8) | TL0;  // 获取定时器值
}

// **外部中断 0 处理函数**
void exint0() interrupt 0 {
    unsigned int time = Timer0_Read();
    Timer0_Reset();  // 重置定时器
    if (time > 8500 && time < 9500) {  // 9ms 低电平 (起始信号)
        bit_count = 0;
    } 
    else if (time > 4000 && time < 5000) {  // 4.5ms 高电平 (起始信号)
        bit_count = 0;
    } 
    else if (time > 400 && time < 700) {  // 560μs 低电平（数据位）
        // 等待高电平
    } 
    else if (time > 400 && time < 700) {  // 560μs 高电平（数据 0）
        ir_data[bit_count / 8] <<= 1;
        bit_count++;
    } 
    else if (time > 1500 && time < 1800) {  // 1.69ms 高电平（数据 1）
        ir_data[bit_count / 8] <<= 1;
        ir_data[bit_count / 8] |= 1;
        bit_count++;
    }
		else
		{
			printf("time: %ld\r\n",time);
		}

    if (bit_count >= 32) {  // 接收完32位数据
        bit_count = 0;
        ir_flag = 1;
    }
}

void irm_init(void)
{
    Timer0_Init();
    EXTI0_Init();

}