/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCAI.com ---------------------------------------------*/
/* --- BBS: www.STCAIMCU.com  -----------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����            */
/*---------------------------------------------------------------------*/

#include	"config.h"
#include	"STC8H_RTC.h"
#include	"STC8G_H_GPIO.h"
#include	"STC8G_H_UART.h"
#include	"STC8G_H_NVIC.h"
#include	"STC8G_H_Switch.h"

/*************	����˵��	**************

��доƬ�ڲ����ɵ�RTCģ��.

ͨ������1��P3.0,P3.1����ӡʱ��(��-��-��-ʱ-��-��).

ͨ�� STC8G_H_UART.h ͷ�ļ������ PRINTF_SELECT ���壬����ʹ�� UART1 ��ӡ printf ��Ϣ��

����ʱ, ѡ��ʱ�� 24MHz (�����������ļ�"config.h"���޸�).

******************************************/

/*************	���س�������	**************/


/*************	���ر�������	**************/


/*************	���غ�������	**************/


/*************  �ⲿ�����ͱ������� *****************/

extern bit B_1S;
extern bit B_Alarm;

/******************** IO������ ********************/
void	GPIO_config(void)
{
	P3_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1);	//P3.0,P3.1 ����Ϊ׼˫���
}

/***************  ���ڳ�ʼ������ *****************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����

	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,   UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//ѡ�����ʷ�����, BRT_Timer1,BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������,     110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	UART_Configuration(UART1, &COMx_InitStructure);		//��ʼ������2 USART1,USART2,USART3,USART4
	NVIC_UART1_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

	UART1_SW(UART1_SW_P30_P31);		//UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

/****************  RTC��ʼ������ *****************/
void	RTC_config(void)
{
	RTC_InitTypeDef		RTC_InitStructure;
	RTC_InitStructure.RTC_Clock  = RTC_X32KCR;	//RTC ʱ��, RTC_IRC32KCR, RTC_X32KCR
	RTC_InitStructure.RTC_Enable = ENABLE;			//RTC����ʹ��,   ENABLE, DISABLE
	RTC_InitStructure.RTC_Year   = 23;					//RTC ��, 00~99, ��Ӧ2000~2099��
	RTC_InitStructure.RTC_Month  = 12;					//RTC ��, 01~12
	RTC_InitStructure.RTC_Day    = 31;					//RTC ��, 01~31
	RTC_InitStructure.RTC_Hour   = 23;					//RTC ʱ, 00~23
	RTC_InitStructure.RTC_Min    = 59;					//RTC ��, 00~59
	RTC_InitStructure.RTC_Sec    = 55;					//RTC ��, 00~59
	RTC_InitStructure.RTC_Ssec   = 00;					//RTC 1/128��, 00~127

	RTC_InitStructure.RTC_ALAHour= 00;					//RTC ����ʱ, 00~23
	RTC_InitStructure.RTC_ALAMin = 00;					//RTC ���ӷ�, 00~59
	RTC_InitStructure.RTC_ALASec = 00;					//RTC ������, 00~59
	RTC_InitStructure.RTC_ALASsec= 00;					//RTC ����1/128��, 00~127
	RTC_Inilize(&RTC_InitStructure);
	NVIC_RTC_Init(RTC_ALARM_INT|RTC_SEC_INT,Priority_0);		//�ж�ʹ��, RTC_ALARM_INT/RTC_DAY_INT/RTC_HOUR_INT/RTC_MIN_INT/RTC_SEC_INT/RTC_SEC2_INT/RTC_SEC8_INT/RTC_SEC32_INT/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}

/******************** main **************************/
void main(void)
{
	EAXSFR();		//��չSFR(XFR)����ʹ�� 

	GPIO_config();
	UART_config();
	RTC_config();
	EA = 1;

	while (1)
	{
		if(B_1S)
		{
			B_1S = 0;
			printf("Year=20%bd,Month=%bd,Day=%bd,Hour=%bd,Minute=%bd,Second=%bd\r\n",YEAR,MONTH,DAY,HOUR,MIN,SEC);
		}

		if(B_Alarm)
		{
			B_Alarm = 0;
			printf("RTC Alarm!\r\n");
		}
	}
}
