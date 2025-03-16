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
#include	"STC8G_H_GPIO.h"
#include	"STC8G_H_ADC.h"
#include	"STC8G_H_UART.h"
#include	"STC8H_DMA.h"
#include	"STC8G_H_NVIC.h"
#include	"STC8G_H_Delay.h"
#include	"STC8G_H_Switch.h"

/*************   ����˵��   ***************

��������ʾ��·ADC DMA����.

��ʼ��ʱ�Ȱ�ҪADCת������������Ϊ��������.

����������������(DMA)���ܣ�����ͨ��һ��ѭ���ɼ��������Զ���ŵ�DMA�����xdata�ռ�.

ͨ������1(P3.0 P3.1)��DMA�����xdata�ռ����յ������ݷ��͸���λ����������115200,N,8,1

�ö�ʱ���������ʷ�����������ʹ��1Tģʽ(���ǵͲ�������12T)����ѡ��ɱ�������������ʱ��Ƶ�ʣ�����߾���.

����ʱ, ѡ��ʱ�� 22.1184MHz (�����������ļ�"config.h"���޸�).

******************************************/

/*************	���س�������	**************/

#define	ADC_CH		16			/* 1~16, ADCת��ͨ����, ��ͬ���޸�ת��ͨ�� */
#define	ADC_DATA	12			/* 6~n, ÿ��ͨ��ADCת����������, 2*ת������+4, ��ͬ���޸�ת������ */

/*************	���ر�������	**************/

u8 chn = 0;
u8 xdata DmaAdBuffer[ADC_CH][ADC_DATA];

/*************	���غ�������	**************/


/*************  �ⲿ�����ͱ������� *****************/


/******************** IO������ ********************/
void	GPIO_config(void)
{
	P0_MODE_IN_HIZ(GPIO_Pin_LOW | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);	//P0.0~P0.6 ����Ϊ��������
	P1_MODE_IN_HIZ(GPIO_Pin_All);		//P1.0~P1.7 ����Ϊ��������
}

/******************** UART���� ********************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;		//�ṹ����

	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//ģʽ,   UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;		//ѡ�����ʷ�����, BRT_Timer1,BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;		//������,     110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;			//��������,   ENABLE �� DISABLE
	UART_Configuration(UART1, &COMx_InitStructure);		//��ʼ������ UART1,UART2,UART3,UART4
	NVIC_UART1_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

	UART1_SW(UART1_SW_P30_P31);		//UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

/******************** ADC ���� ********************/
void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;		//�ṹ����

	ADC_InitStructure.ADC_SMPduty   = 31;		//ADC ģ���źŲ���ʱ�����, 0~31��ע�⣺ SMPDUTY һ����������С�� 10��
	ADC_InitStructure.ADC_CsSetup   = 0;		//ADC ͨ��ѡ��ʱ����� 0(Ĭ��),1
	ADC_InitStructure.ADC_CsHold    = 1;		//ADC ͨ��ѡ�񱣳�ʱ����� 0,1(Ĭ��),2,3
	ADC_InitStructure.ADC_Speed     = ADC_SPEED_2X16T;		//���� ADC ����ʱ��Ƶ��	ADC_SPEED_2X1T~ADC_SPEED_2X16T
	ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;	//ADC�������,	ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
	ADC_Inilize(&ADC_InitStructure);		//��ʼ��
	ADC_PowerControl(ENABLE);				//ADC��Դ����, ENABLE��DISABLE
	NVIC_ADC_Init(DISABLE,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}

/******************** DMA ���� ********************/
void	DMA_config(void)
{
	DMA_ADC_InitTypeDef		DMA_ADC_InitStructure;		//�ṹ����

	DMA_ADC_InitStructure.DMA_Enable = ENABLE;			//DMAʹ��  	ENABLE,DISABLE
	DMA_ADC_InitStructure.DMA_Channel = 0xffff;			//ADCͨ��ʹ�ܼĴ���, 1:ʹ��, bit15~bit0 ��Ӧ ADC15~ADC0
	DMA_ADC_InitStructure.DMA_Buffer = (u16)DmaAdBuffer;	//ADCת�����ݴ洢��ַ
	DMA_ADC_InitStructure.DMA_Times = ADC_4_Times;	//ÿ��ͨ��ת������, ADC_1_Times,ADC_2_Times,ADC_4_Times,ADC_8_Times,ADC_16_Times,ADC_32_Times,ADC_64_Times,ADC_128_Times,ADC_256_Times
	DMA_ADC_Inilize(&DMA_ADC_InitStructure);		//��ʼ��
	NVIC_DMA_ADC_Init(ENABLE,Priority_0,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0~Priority_3; �������ȼ�(�͵���) Priority_0~Priority_3
	DMA_ADC_TRIG();		//��������ת��
}

/******************** task A **************************/
void main(void)
{
	u8	i,n;

	EAXSFR();		//��չSFR(XFR)����ʹ�� 
	GPIO_config();
	UART_config();
	ADC_config();
	DMA_config();
	EA = 1;
	
	while (1)
	{
		delay_ms(200);	//200ms���һ�β������������۲�
		
		if(DmaADCFlag)  //�ж�ADC DMA�����Ƿ����
		{
			DmaADCFlag = 0; //�����ɱ�־
			for(i=0; i<ADC_CH; i++)     //��ӡÿ������ͨ���Ĳ������
			{
				for(n=0; n<ADC_DATA; n++)   //��ӡ��ǰͨ���Ĳ�������
				{
					printf("0x%02bx ",DmaAdBuffer[i][n]);	//��1������,...,��n������,ADͨ��,ƽ������,ƽ��ֵ
				}
				printf("\r\n"); //��������س����з�
			}
			printf("\r\n");     //��������س����з�
			DMA_ADC_TRIG();		//���´���������һ��ת��
		}
	}
}



