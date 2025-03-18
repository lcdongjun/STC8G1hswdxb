#include "config.h"
#include "uart.h"
#include "ds18b20.h"
#include "IRM.h"


void System_Init(void)
{
    P3M1 &= ~(1 << 3); // 设置为准双向口
    P3M0 &= ~(1 << 3);
    
    P5M1 &= ~((1 << 4) | (1 << 5)); // 设置为推挽输出
    P5M0 |= ((1 << 4) | (1 << 5));
	
		UartInit();
		EA = 1;
		DS18B20_Init();
}

void main(void)
{
		P_SW2 |= 0x80;
	
    System_Init(); // 初始化系统
		printf("System Run! \r\n");
		P54 = 0;
		P55 = 1;
    while (1)
    {
			float temp = DS18B20_GetTemp();
			printf("temp = %.3f\r\n",temp);
			Delay_ms(1000);
    }
}


