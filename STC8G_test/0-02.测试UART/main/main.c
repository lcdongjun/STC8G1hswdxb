#include "config.h"
#include "uart.h"


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
		UartInit(); 
		ES = 1; 
		EA = 1; 
		printf("System Run! \r\n");
		P54 = 0;
		P55 = 1;
    while (1)
    {
			delay_ms(250);
			delay_ms(250);
			delay_ms(250);
			delay_ms(250);
			printf("System Run! \r\n");
//				if (rptr != wptr) 
//					{
//					 UartSend(buffer[rptr++]); 
//					 rptr &= 0x0f; 
//					}
    }
}
