#include "config.h"
#include "uart.h"
#include "eeprom.h"
#include "ds18b20.h"
#include "IRM.h"

unsigned char temp_flag = 1; 

void System_Init(void);
void Restore_Settings(void);
void Save_Settings(void);

void main(void)
{

		P_SW2 |= 0x80;
    System_Init(); // 初始化系统
		printf("System Run! \r\n");
		P54 = 0;
		P55 = 0;
		Restore_Settings();
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
										Save_Settings();
										break;

								case 0x43:
										printf("Toggling P55\n");
										temp_flag = 0;
										P54 = 0;
										P55 = ~P55;
										Save_Settings();
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


void Restore_Settings(void)
{
		unsigned char gpio_status = IapRead(FLASH_ADDR + 1);
    temp_flag = IapRead(FLASH_ADDR);
    P54 = (gpio_status & 0x01) ? 1 : 0;
    P55 = (gpio_status & 0x02) ? 1 : 0;

    printf("Restored: temp_flag = %d, P54 = %d, P55 = %d\n", temp_flag, P54, P55);
}

void Save_Settings(void)
{
    IapErase(FLASH_ADDR);
    IapSafeWrite(FLASH_ADDR, temp_flag);
		printf("gpio_status: 0x%x",(P54 ? 0x01 : 0) | (P55 ? 0x02 : 0));
    IapSafeWrite(FLASH_ADDR + 1, (P54 ? 0x01 : 0) | (P55 ? 0x02 : 0));
}
