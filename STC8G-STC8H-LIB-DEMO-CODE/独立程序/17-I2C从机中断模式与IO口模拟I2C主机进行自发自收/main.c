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
#include	"STC8G_H_I2C.h"
#include	"STC8G_H_Soft_I2C.h"
#include	"STC8G_H_Timer.h"
#include	"STC8G_H_UART.h"
#include	"STC8G_H_NVIC.h"
#include	"STC8G_H_Switch.h"

/*************	����˵��	**************

�����̻���STC8H8K64UΪ����оƬ�Ŀ��츫���İ���б�д���ԣ�STC8G��STC8Hϵ��оƬ��ͨ�òο�.

�ڲ�����I2C�������߿��������ӻ�ģʽ��SCL->P2.5, SDA->P2.4;
IO��ģ��I2C������ģʽ��SCL->P0.0, SDA->P0.1;
ͨ���ⲿ�������� P0.0->P2.5, P0.1->P2.4��ʵ��I2C�Է����չ��ܡ�

ʹ��Timer0��16λ�Զ���װ������1ms����,�������������������,�û��޸�MCU��ʱ��Ƶ��ʱ,�Զ���ʱ��1ms.
������ÿ���Ӽ�1, ������ΧΪ0~199.

�ϵ������ÿ���ӷ���һ�μ������ݣ����ڴ�ӡ�������ݣ��ӻ����յ����ݺ�ͨ�����ڴ�ӡ���ս����

��Ҫ��"STC8G_H_UART.h"�����ã� #define	PRINTF_SELECT  UART1

����ʱ, ѡ��ʱ�� 24MHz (�����������ļ�"config.h"���޸�).

******************************************/

/*************	���س�������	**************/


/*************	���ر�������	**************/
bit B_1ms;          //1ms��־

u8  temp[4];     //ͨ������
u16 msecond;
u8  second;

/*************	���غ�������	**************/

void DisplayScan(void);

/*************  �ⲿ�����ͱ������� *****************/


/******************** IO������ ********************/
void	GPIO_config(void)
{
	P0_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1);		//P0.0,P0.1 ����Ϊ׼˫���
	P2_MODE_IO_PU(GPIO_Pin_4 | GPIO_Pin_5);		//P2.4,P2.5 ����Ϊ׼˫���
	P3_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1);		//P3.0,P3.1 ����Ϊ׼˫���
}

/************************ ��ʱ������ ****************************/
void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;						//�ṹ����
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;  //ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;         //ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;              //�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 1000UL);   //��ֵ,
	TIM_InitStructure.TIM_Run       = ENABLE;               //�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);               //��ʼ��Timer0	  Timer0,Timer1,Timer2,Timer3,Timer4
	NVIC_Timer0_Init(ENABLE,Priority_0);    //�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}

/****************  I2C��ʼ������ *****************/
void	I2C_config(void)
{
	I2C_InitTypeDef		I2C_InitStructure;

	I2C_InitStructure.I2C_Mode      = I2C_Mode_Slave;		//����ѡ��   I2C_Mode_Master, I2C_Mode_Slave
	I2C_InitStructure.I2C_Enable    = ENABLE;						//I2C����ʹ��,   ENABLE, DISABLE
	I2C_InitStructure.I2C_SL_MA     = ENABLE;						//ʹ�ܴӻ���ַ�ȽϹ���,   ENABLE, DISABLE
	I2C_InitStructure.I2C_SL_ADR    = 0x2d;							//�ӻ��豸��ַ,  0~127  (0x2d<<1 = 0x5a)
	I2C_Init(&I2C_InitStructure);
	NVIC_I2C_Init(I2C_Mode_Slave,I2C_ESTAI|I2C_ERXI|I2C_ETXI|I2C_ESTOI,Priority_0);	//����ģʽ, I2C_Mode_Master, I2C_Mode_Slave; �ж�ʹ��, I2C_ESTAI/I2C_ERXI/I2C_ETXI/I2C_ESTOI/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

	I2C_SW(I2C_P24_P25);					//I2C_P14_P15,I2C_P24_P25,I2C_P33_P32
}

/****************  UART��ʼ������ *****************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����

	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//ģʽ, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//ѡ�����ʷ�����, BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
	UART_Configuration(UART1, &COMx_InitStructure);		//��ʼ������1 UART1,UART2,UART3,UART4
	NVIC_UART1_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

	UART1_SW(UART1_SW_P30_P31);		//UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

/******************** task A **************************/
void main(void)
{
	EAXSFR();		/* ��չ�Ĵ�������ʹ�� */
	GPIO_config();
	Timer_config();
	I2C_config();
    UART_config();
	EA = 1;
    
	while (1)
	{
		if(B_1ms)
		{
			B_1ms = 0;
            
            if(DisplayFlag)
            {
                DisplayFlag = 0;
                printf("I2C Read: %bd%bd%bd%bd\r\n",I2C_Buffer[0],I2C_Buffer[1],I2C_Buffer[2],I2C_Buffer[3]);
            }
            
            if(++msecond >= 1000)   //1�뵽
            {
                msecond = 0;        //��1000ms����
                second++;         //�����+1
                if(second >= 200)    second = 0;   //�������ΧΪ0~199

                printf("SI2C Send: %04bu\r\n",second);
                temp[0] = second / 1000;
                temp[1] = (second % 1000) / 100;
                temp[2] = (second % 100) / 10;
                temp[3] = second % 10;

                SI2C_WriteNbyte(SLAW, 0, temp, 4);
            }
		}
	}
}
