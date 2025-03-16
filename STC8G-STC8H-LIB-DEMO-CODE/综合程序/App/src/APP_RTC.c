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

#include	"APP.h"
#include	"STC8H_RTC.h"
#include	"STC8G_H_GPIO.h"
#include	"STC8G_H_I2C.h"
#include	"STC8G_H_NVIC.h"
#include	"STC8G_H_UART.h"
#include	"STC8G_H_Switch.h"

/*************	����˵��	**************

�����̻���STC8H8K64UΪ����оƬ��ʵ������б�д���ԣ�STC8Hϵ�д�RTCģ���оƬ��ͨ�òο�.

��доƬ�ڲ����ɵ�RTCģ��.

��STC��MCU��IO��ʽ����8λ����ܡ�

ʹ��Timer0��16λ�Զ���װ������1ms����,�������������������, �û��޸�MCU��ʱ��Ƶ��ʱ,�Զ���ʱ��1ms.

8λ�������ʾʱ��(Сʱ-����-��).

����ɨ�谴������Ϊ25~32.

����ֻ֧�ֵ�������, ��֧�ֶ��ͬʱ����, ���������в���Ԥ֪�Ľ��.

�����³���1���,����10��/����ٶ��ṩ�ؼ����. �û�ֻ��Ҫ���KeyCode�Ƿ��0���жϼ��Ƿ���.

����ʱ���:
����25: Сʱ+.
����26: Сʱ-.
����27: ����+.
����28: ����-.

����ʱ, ѡ��ʱ�� 24MHz (�����������ļ�"config.h"���޸�).

******************************************/

//========================================================================
//                               ���س�������	
//========================================================================

#define SleepModeSet  0        //0:��������ģʽ��ʹ���������ʾʱ���ܽ�����; 1:ʹ������ģʽ

//========================================================================
//                               ���ر�������
//========================================================================


//========================================================================
//                               ���غ�������
//========================================================================

void IO_KeyScan(void);   //50ms call
void DisplayRTC(void);
void WriteRTC(void);

//========================================================================
//                            �ⲿ�����ͱ�������
//========================================================================

extern bit B_1S;
extern bit B_Alarm;

//========================================================================
// ����: RTC_init
// ����: �û���ʼ������.
// ����: None.
// ����: None.
// �汾: V1.0, 2020-09-25
//========================================================================
void RTC_init(void)
{
	u8  i;
	RTC_InitTypeDef		RTC_InitStructure;
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����

	RTC_InitStructure.RTC_Clock  = RTC_X32KCR;	//RTC ʱ��, RTC_IRC32KCR, RTC_X32KCR
	RTC_InitStructure.RTC_Enable = ENABLE;			//I2C����ʹ��,   ENABLE, DISABLE
	RTC_InitStructure.RTC_Year   = 21;					//RTC ��, 00~99, ��Ӧ2000~2099��
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

	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,   UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
//	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//ѡ�����ʷ�����, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2, ���Բ���ѡ��)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������,     110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = DISABLE;			//���ս�ֹ,   ENABLE �� DISABLE
	UART_Configuration(UART2, &COMx_InitStructure);		//��ʼ������2 UART1,UART2,UART3,UART4
	NVIC_UART2_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
	
	P0_MODE_IO_PU(GPIO_Pin_All);		//P0 ����Ϊ׼˫���
	P4_MODE_IO_PU(GPIO_Pin_6 | GPIO_Pin_7);		//P4.6,P4.7 ����Ϊ׼˫���
	P6_MODE_IO_PU(GPIO_Pin_All);		//P6 ����Ϊ׼˫���
	P7_MODE_IO_PU(GPIO_Pin_All);		//P7 ����Ϊ׼˫���
	display_index = 0;
	
	for(i=0; i<8; i++)  LED8[i] = 0x10; //�ϵ�����
    
	KeyHoldCnt = 0; //�����¼�ʱ
	KeyCode = 0;    //���û�ʹ�õļ���

	IO_KeyState = 0;
	IO_KeyState1 = 0;
	IO_KeyHoldCnt = 0;
	cnt50ms = 0;

	printf("STC8H RTC Test!\r\n");
}

//========================================================================
// ����: Sample_RTC
// ����: �û�Ӧ�ó���.
// ����: None.
// ����: None.
// �汾: V1.0, 2020-09-25
//========================================================================
void Sample_RTC(void)
{
	if(B_1S)
	{
		B_1S = 0;
		DisplayRTC();
		printf("Year=20%d,Month=%d,Day=%d,Hour=%d,Minute=%d,Second=%d\r\n",YEAR,MONTH,DAY,HOUR,MIN,SEC);
	}

	if(B_Alarm)
	{
		B_Alarm = 0;
		printf("RTC Alarm!\r\n");
	}

#if(SleepModeSet == 1)
		_nop_();
		_nop_();
		PD = 1;  //STC32G оƬʹ���ڲ�32Kʱ�ӣ������޷�����
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
#else
	DisplayScan();
	
	if(++cnt50ms >= 50)     //50msɨ��һ�����м���
	{
		cnt50ms = 0;
		IO_KeyScan();
	}
	
	if(KeyCode != 0)        //�м�����
	{
		if(KeyCode == 25)   //hour +1
		{
			if(++hour >= 24)    hour = 0;
			WriteRTC();
			DisplayRTC();
		}
		if(KeyCode == 26)   //hour -1
		{
			if(--hour >= 24)    hour = 23;
			WriteRTC();
			DisplayRTC();
		}
		if(KeyCode == 27)   //minute +1
		{
			second = 0;
			if(++minute >= 60)  minute = 0;
			WriteRTC();
			DisplayRTC();
		}
		if(KeyCode == 28)   //minute -1
		{
			second = 0;
			if(--minute >= 60)  minute = 59;
			WriteRTC();
			DisplayRTC();
		}

		KeyCode = 0;
	}
#endif
}

//========================================================================
// ����: DisplayRTC
// ����: ��ʾʱ�Ӻ���.
// ����: None.
// ����: None.
// �汾: V1.0, 2020-09-25
//========================================================================
void DisplayRTC(void)
{
    hour = HOUR;
    minute = MIN;

    if(HOUR >= 10)  LED8[0] = HOUR / 10;
    else            LED8[0] = DIS_BLACK;
    LED8[1] = HOUR % 10;
    LED8[2] = DIS_;
    LED8[3] = MIN / 10;
    LED8[4] = MIN % 10;
    LED8[5] = DIS_;
    LED8[6] = SEC / 10;
    LED8[7] = SEC % 10;
}


//========================================================================
// ����: WriteRTC
// ����: дRTC����.
// ����: None.
// ����: None.
// �汾: V1.0, 2020-09-25
//========================================================================
void WriteRTC(void)
{
    INIYEAR = YEAR;   //�̳е�ǰ������
    INIMONTH = MONTH;
    INIDAY = DAY;

    INIHOUR = hour;   //�޸�ʱ����
    INIMIN = minute;
    INISEC = 0;
    INISSEC = 0;
    RTCCFG |= 0x01;   //����RTC�Ĵ�����ʼ��
}


/*****************************************************
    ���м�ɨ�����
    ʹ��XY����4x4���ķ�����ֻ�ܵ������ٶȿ�

   Y     P04      P05      P06      P07
          |        |        |        |
X         |        |        |        |
P00 ---- K00 ---- K01 ---- K02 ---- K03 ----
          |        |        |        |
P01 ---- K04 ---- K05 ---- K06 ---- K07 ----
          |        |        |        |
P02 ---- K08 ---- K09 ---- K10 ---- K11 ----
          |        |        |        |
P03 ---- K12 ---- K13 ---- K14 ---- K15 ----
          |        |        |        |
******************************************************/


//========================================================================
// ����: IO_KeyDelay
// ����: ����ɨ���ӳٳ���.
// ����: None.
// ����: None.
// �汾: V1.0, 2020-09-25
//========================================================================
void IO_KeyDelay(void)
{
    u8 i;
    i = 60;
    while(--i)  ;
}

//========================================================================
// ����: IO_KeyScan
// ����: ����ɨ�����.
// ����: None.
// ����: None.
// �汾: V1.0, 2020-09-25
//========================================================================
void IO_KeyScan(void)    //50ms call
{
    u8  j;

    j = IO_KeyState1;   //������һ��״̬

    P0 = 0xf0;  //X�ͣ���Y
    IO_KeyDelay();
    IO_KeyState1 = P0 & 0xf0;

    P0 = 0x0f;  //Y�ͣ���X
    IO_KeyDelay();
    IO_KeyState1 |= (P0 & 0x0f);
    IO_KeyState1 ^= 0xff;   //ȡ��
    
    if(j == IO_KeyState1)   //�������ζ����
    {
        j = IO_KeyState;
        IO_KeyState = IO_KeyState1;
        if(IO_KeyState != 0)    //�м�����
        {
            F0 = 0;
            if(j == 0)  F0 = 1; //��һ�ΰ���
            else if(j == IO_KeyState)
            {
                if(++IO_KeyHoldCnt >= 20)   //1����ؼ�
                {
                    IO_KeyHoldCnt = 18;
                    F0 = 1;
                }
            }
            if(F0)
            {
                j = T_KeyTable[IO_KeyState >> 4];
                if((j != 0) && (T_KeyTable[IO_KeyState& 0x0f] != 0)) 
                    KeyCode = (j - 1) * 4 + T_KeyTable[IO_KeyState & 0x0f] + 16;    //������룬17~32
            }
        }
        else    IO_KeyHoldCnt = 0;
    }
    P0 = 0xff;
}

