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
#include	"STC8G_H_Exti.h"
#include	"STC8G_H_UART.h"
#include	"STC8G_H_Delay.h"
#include	"STC8G_H_NVIC.h"
#include	"STC8G_H_Switch.h"

/*************	����˵��	**************

�����̻���STC8H8K64UΪ����оƬ��ʵ����8���б�д���ԣ�STC8G��STC8Hϵ��оƬ��ͨ�òο�.

��ʾINT0~INT4 5������Դ��MCU�����߻���.

�Ӵ����������Դ�����Ѵ�����115200,N,8,1.

����ʱ, ѡ��ʱ�� 22.1184MHz (�û�����"config.h"�޸�Ƶ��).

******************************************/

/*************	���س�������	**************/


/*************	���ر�������	**************/

u8 WakeUpCnt;

/*************	���غ�������	**************/



/*************  �ⲿ�����ͱ������� *****************/



/******************** IO������ ********************/
void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;				//�ṹ����

	GPIO_InitStructure.Pin  = GPIO_Pin_All;			//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_PullUp;			//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//��ʼ��
}

/******************** INT���� ********************/
void	Exti_config(void)
{
	EXTI_InitTypeDef	Exti_InitStructure;							//�ṹ����

	Exti_InitStructure.EXTI_Mode      = EXT_MODE_Fall;//�ж�ģʽ,   EXT_MODE_RiseFall,EXT_MODE_Fall
	Ext_Inilize(EXT_INT0,&Exti_InitStructure);				//��ʼ��
	NVIC_INT0_Init(ENABLE,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

	Exti_InitStructure.EXTI_Mode      = EXT_MODE_Fall;//�ж�ģʽ,   EXT_MODE_RiseFall,EXT_MODE_Fall
	Ext_Inilize(EXT_INT1,&Exti_InitStructure);				//��ʼ��
	NVIC_INT1_Init(ENABLE,Priority_0);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

	NVIC_INT2_Init(ENABLE,NULL);		//�ж�ʹ��, ENABLE/DISABLE; �����ȼ�
	NVIC_INT3_Init(ENABLE,NULL);		//�ж�ʹ��, ENABLE/DISABLE; �����ȼ�
	NVIC_INT4_Init(ENABLE,NULL);		//�ж�ʹ��, ENABLE/DISABLE; �����ȼ�
}

/****************  ���ڳ�ʼ������ *****************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;				//�ṹ����

	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//ģʽ, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;		//ѡ�����ʷ�����, BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;		//������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;			//��������,   ENABLE��DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;		//�����ʼӱ�, ENABLE��DISABLE
	UART_Configuration(UART1, &COMx_InitStructure);		//��ʼ������1 UART1,UART2,UART3,UART4
	NVIC_UART1_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

	UART1_SW(UART1_SW_P30_P31);		//UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

/******************** ������***********************/
void main(void)
{
	GPIO_config();
	UART_config();
//	Exti_config();
	EA  = 1;		//Enable all interrupt
	
	PrintString1("STC8H8K64U EXINT Wakeup Test Programme!\r\n");	//UART1����һ���ַ���

	while(1)
	{
		while(!INT0);	//�ȴ����ж�Ϊ�ߵ�ƽ
		while(!INT1);	//�ȴ����ж�Ϊ�ߵ�ƽ
//		while(!INT2);	//�ȴ����ж�Ϊ�ߵ�ƽ
//		while(!INT3);	//�ȴ����ж�Ϊ�ߵ�ƽ
//		while(!INT4);	//�ȴ����ж�Ϊ�ߵ�ƽ
		delay_ms(10);	//delay 10ms

		while(!INT0);	//�ȴ����ж�Ϊ�ߵ�ƽ
		while(!INT1);	//�ȴ����ж�Ϊ�ߵ�ƽ
//		while(!INT2);	//�ȴ����ж�Ϊ�ߵ�ƽ
//		while(!INT3);	//�ȴ����ж�Ϊ�ߵ�ƽ
//		while(!INT4);	//�ȴ����ж�Ϊ�ߵ�ƽ

		Exti_config();
		WakeUpSource = 0;

		PCON |= 0x02; 	//Sleep
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		
		if(WakeUpSource == 1)	PrintString1("���ж�INT0����  ");
		if(WakeUpSource == 2)	PrintString1("���ж�INT1����  ");
		if(WakeUpSource == 3)	PrintString1("���ж�INT2����  ");
		if(WakeUpSource == 4)	PrintString1("���ж�INT3����  ");
		if(WakeUpSource == 5)	PrintString1("���ж�INT4����  ");
		WakeUpSource = 0;
		
		WakeUpCnt++;
		TX1_write2buff(WakeUpCnt/100+'0');
		TX1_write2buff(WakeUpCnt%100/10+'0');
		TX1_write2buff(WakeUpCnt%10+'0');
		PrintString1("�λ���\r\n");
	}

}

