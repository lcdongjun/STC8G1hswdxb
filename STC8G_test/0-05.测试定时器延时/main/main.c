#include "config.h"
#include "uart.h"
#include "ds18b20.h"
#include "IRM.h"


void System_Init(void)
{
    P3M1 &= ~(1 << 3); // ����Ϊ׼˫���
    P3M0 &= ~(1 << 3);
    
    P5M1 &= ~((1 << 4) | (1 << 5)); // ����Ϊ�������
    P5M0 |= ((1 << 4) | (1 << 5));
	
		UartInit();
		irm_init();
		EA = 1;
//		DS18B20_Init();
}

void main(void)
{
//		P_SW2 |= 0x80;
	
    System_Init(); // ��ʼ��ϵͳ
		printf("System Run! \r\n");
		P54 = 0;
		P55 = 0;
    while (1)
    {
//			float temp = DS18B20_GetTemp();
//			printf("temp = %.3f\r\n",temp);
//			Delay_ms(1000);
//			printf("System Run! \r\n");
				if(irok)                        //������պ��˽��к��⴦��
				{
					Ircordpro();
					irok=0;
				}
				if(irpro_ok)                   //�������ú���й��������簴��Ӧ�İ�������ʾ��Ӧ�����ֵ�
				{
					Ir_work();
				}
    }
}