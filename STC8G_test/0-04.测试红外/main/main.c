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
		irm_init();
		DS18B20_Init();
		EA = 1;
}

void main(void)
{
		unsigned char temp_flag = 1; 
		P_SW2 |= 0x80;
    System_Init(); // 初始化系统
		printf("System Run! \r\n");
		P54 = 0;
		P55 = 0;
    while (1)
    {
				float temp = DS18B20_GetTemp();
				printf("temp = %.3f\r\n", temp);

				if (irok) {  
						Ircordpro();
						irok = 0;
				}

				// 处理温度控制
				if (temp_flag) {
						if (temp >= 16) {
								P55 = 1;
								P54 = 0;
						} else if (temp >= 15) {
								P55 = 0;
								P54 = 1;
						} else {
								P55 = 0;
								P54 = 0;
						}
				}

				// 处理红外控制
				if (irpro_ok) {
						printf("IR received: 0x%x\n", IRcord[2]);

						switch (IRcord[2]) {
								case 0x44:
										printf("Toggling P54\n");
										temp_flag = 0;
										P55 = 0;
										P54 = ~P54;
										break;

								case 0x43:
										printf("Toggling P55\n");
										temp_flag = 0;
										P54 = 0;
										P55 = ~P55;
										break;

								default:
										printf("Unknown IR command, resuming temperature control.\n");
										temp_flag = 1;
										break;
						}
						
						irpro_ok = 0;
				}
				
    }
}