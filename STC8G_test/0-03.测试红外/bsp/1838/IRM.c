#include "IRM.h"
#include "uart.h"

#define IR_PIN P32  // 1838 接收器连接到 P3.2

volatile unsigned char ir_data[4];  // 存储接收数据
volatile bit ir_flag = 0;  // 数据接收完成标志
volatile unsigned char bit_count = 0, byte_count = 0;

void Timer0_Init(void) {
//    TMOD &= 0xF0;  // 设定定时器模式
//    TMOD |= 0x01;  // 模式 1（16 位定时器）
		AUXR |= (1<<7); 
		TMOD |= (1<<3);
		
    TH0 = 0;
    TL0 = 0;
    ET0 = 1;  // 使能定时器 0 中断
    EA = 1;   // 使能全局中断
    TR0 = 1;  // 启动定时器
}

void INT0_Init(void) {
    IT0 = 1;  // 下降沿触发
    EX0 = 1;  // 使能外部中断 0
    EA = 1;   // 使能全局中断
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