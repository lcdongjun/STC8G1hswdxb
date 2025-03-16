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
#include	"STC8G_PCA.h"
#include	"STC8G_H_GPIO.h"
#include	"STC8G_H_Delay.h"
#include	"STC8G_H_Switch.h"

/*************   ����˵��   ***************

�����̻���STC8G1K08-20PIN���б�д���ԣ�STC8Gϵ��оƬ��ͨ�òο�.

���3·�仯��PWM�ź�, ����"������"������.

PCA0, PCA1, PCA2 Ϊ16λPWM.

����ʱ, ѡ��ʱ�� 24MHz (�û�����"config.h"�޸�Ƶ��).

******************************************/

/*************	���س�������	**************/

#define		PWM_DUTY		6000		//����PWM�����ڣ���ֵΪPCA��ѡ���ʱ�����������
#define		PWM_HIGH_MIN	80			//����PWM�������Сռ�ձȡ�
#define		PWM_HIGH_MAX	(PWM_DUTY - PWM_HIGH_MIN)	//����PWM��������ռ�ձȡ�

/*************	���ر�������	**************/

u16	pwm0,pwm1,pwm2;
bit	B_PWM0_Dir,B_PWM1_Dir,B_PWM2_Dir;	//����, 0Ϊ+, 1Ϊ-.

u16	PWM0_low;	//PWM����͵�ƽ��PCAʱ���������
u16	PWM1_low;	//PWM����͵�ƽ��PCAʱ���������
u16	PWM2_low;	//PWM����͵�ƽ��PCAʱ���������

/*************	���غ�������	**************/

void PWMn_Update(u8 PCA_id, u16 high);

/*************  �ⲿ�����ͱ������� *****************/


/******************** IO������ ********************/
void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;				//�ṹ����

	GPIO_InitStructure.Pin  = GPIO_Pin_All;			//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_PullUp;			//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	//��ʼ��

	GPIO_InitStructure.Pin  = GPIO_Pin_7;				//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
	GPIO_InitStructure.Mode = GPIO_PullUp;			//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//��ʼ��
}

/******************** PCA���� ********************/
void	PCA_config(void)
{
	PCA_InitTypeDef		PCA_InitStructure;

	PCA_InitStructure.PCA_Clock    = PCA_Clock_1T;		//PCA_Clock_1T, PCA_Clock_2T, PCA_Clock_4T, PCA_Clock_6T, PCA_Clock_8T, PCA_Clock_12T, PCA_Clock_Timer0_OF, PCA_Clock_ECI
	PCA_InitStructure.PCA_RUN      = DISABLE;			//ENABLE, DISABLE
	PCA_Init(PCA_Counter,&PCA_InitStructure);			//���ù���Counter

	PCA_InitStructure.PCA_PWM_Wide = 0;					//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit, PCA_PWM_10bit
	PCA_InitStructure.PCA_Value    = 65535;				//���������ʱ, Ϊƥ��Ƚ�ֵ
	PCA_Init(PCA0,&PCA_InitStructure);

	PCA_InitStructure.PCA_PWM_Wide = 0;					//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit, PCA_PWM_10bit
	PCA_InitStructure.PCA_Value    = 65535;				//���������ʱ, Ϊƥ��Ƚ�ֵ
	PCA_Init(PCA1,&PCA_InitStructure);

	PCA_InitStructure.PCA_PWM_Wide = 0;					//PCA_PWM_8bit, PCA_PWM_7bit, PCA_PWM_6bit, PCA_PWM_10bit
	PCA_InitStructure.PCA_Value    = 65535;				//���������ʱ, Ϊƥ��Ƚ�ֵ
	PCA_Init(PCA2,&PCA_InitStructure);

	NVIC_PCA_Init(PCA_Counter,PCA_Mode_HighPulseOutput,Priority_0);
	NVIC_PCA_Init(PCA0,PCA_Mode_HighPulseOutput,Priority_0);
	NVIC_PCA_Init(PCA1,PCA_Mode_HighPulseOutput,Priority_0);
	NVIC_PCA_Init(PCA2,PCA_Mode_HighPulseOutput,Priority_0);

	PCA_SW(PCA_P12_P11_P10_P37);	//PCA_P12_P11_P10_P37,PCA_P34_P35_P36_P37,PCA_P24_P25_P26_P27
	CR = 1;							//����PCA
}


/******************** task A **************************/
void main(void)
{
	EAXSFR();		/* ��չ�Ĵ�������ʹ�� */
    GPIO_config();
    PCA_config();
    pwm0 = (PWM_DUTY / 4 * 1);	//��PWMһ����ֵ
    pwm1 = (PWM_DUTY / 4 * 2);
    pwm2 = (PWM_DUTY / 4 * 3);
    B_PWM0_Dir = 0;
    B_PWM1_Dir = 0;
    B_PWM2_Dir = 0;

    PWMn_Update(PCA0,pwm0);
    PWMn_Update(PCA1,pwm1);
    PWMn_Update(PCA2,pwm2);
    EA = 1;

    while (1)
    {
        delay_ms(1);

        if(B_PWM0_Dir)
        {
            if(--pwm0 <= PWM_HIGH_MIN)	B_PWM0_Dir = 0;
        }
        else if(++pwm0 >= PWM_HIGH_MAX)	B_PWM0_Dir = 1;
        PWMn_Update(PCA0,pwm0);

        if(B_PWM1_Dir)
        {
            if(--pwm1 <= PWM_HIGH_MIN)		B_PWM1_Dir = 0;
        }
        else if(++pwm1 >= PWM_HIGH_MAX)	B_PWM1_Dir = 1;
        PWMn_Update(PCA1,pwm1);

        if(B_PWM2_Dir)
        {
            if(--pwm2 <= PWM_HIGH_MIN)		B_PWM2_Dir = 0;
        }
        else if(++pwm2 >= PWM_HIGH_MAX)	B_PWM2_Dir = 1;
        PWMn_Update(PCA2,pwm2);
    }
}

void PWMn_Update(u8 PCA_id, u16 high)
{
	if(PCA_id == PCA0)
	{
		if(high > PWM_HIGH_MAX)	high = PWM_HIGH_MAX;	//���д��������ռ�ձ����ݣ�ǿ��Ϊ���ռ�ձȡ�
		if(high < PWM_HIGH_MIN)	high = PWM_HIGH_MIN;	//���д��С����Сռ�ձ����ݣ�ǿ��Ϊ��Сռ�ձȡ�
		CR = 0;							//ֹͣPCA��
		PCA_Timer0 = high;				//��������ȷ��Χ����װ��ռ�ձȼĴ�����
		PWM0_low = PWM_DUTY - high;	//���㲢����PWM����͵�ƽ��PCAʱ�����������
		CR = 1;							//����PCA��
	}
	else if(PCA_id == PCA1)
	{
		if(high > PWM_HIGH_MAX)	high = PWM_HIGH_MAX;	//���д��������ռ�ձ����ݣ�ǿ��Ϊ���ռ�ձȡ�
		if(high < PWM_HIGH_MIN)	high = PWM_HIGH_MIN;	//���д��С����Сռ�ձ����ݣ�ǿ��Ϊ��Сռ�ձȡ�
		CR = 0;							//ֹͣPCA��
		PCA_Timer1 = high;				//��������ȷ��Χ����װ��ռ�ձȼĴ�����
		PWM1_low = PWM_DUTY - high;	//���㲢����PWM����͵�ƽ��PCAʱ�����������
		CR = 1;							//����PCA��
	}
	else if(PCA_id == PCA2)
	{
		if(high > PWM_HIGH_MAX)		high = PWM_HIGH_MAX;	//���д��������ռ�ձ����ݣ�ǿ��Ϊ���ռ�ձȡ�
		if(high < PWM_HIGH_MIN)		high = PWM_HIGH_MIN;	//���д��С����Сռ�ձ����ݣ�ǿ��Ϊ��Сռ�ձȡ�
		CR = 0;							//ֹͣPCA��
		PCA_Timer2 = high;				//��������ȷ��Χ����װ��ռ�ձȼĴ�����
		PWM2_low = PWM_DUTY - high;	//���㲢����PWM����͵�ƽ��PCAʱ�����������
		CR = 1;							//����PCA��
	}
}

//========================================================================
// ����: void PCA_Handler (void) interrupt PCA_VECTOR
// ����: PCA�жϴ������.
// ����: None
// ����: none.
// �汾: V1.0, 2012-11-22
//========================================================================
void PCA_Handler (void) interrupt PCA_VECTOR
{
	if(CCF0)		//PCAģ��0�ж�
	{
		CCF0 = 0;		//��PCAģ��0�жϱ�־
		if(P11)	CCAP0_tmp += PCA_Timer0;	//���Ϊ�ߵ�ƽ�����Ӱ��Ĵ���װ�ظߵ�ƽʱ�䳤��
		else	CCAP0_tmp += PWM0_low;	//���Ϊ�͵�ƽ�����Ӱ��Ĵ���װ�ص͵�ƽʱ�䳤��
		CCAP0L = (u8)CCAP0_tmp;			//��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
		CCAP0H = (u8)(CCAP0_tmp >> 8);	//��дCCAP0H
	}

	if(CCF1)	//PCAģ��1�ж�
	{
		CCF1 = 0;		//��PCAģ��1�жϱ�־
		if(P10)	CCAP1_tmp += PCA_Timer1;	//���Ϊ�ߵ�ƽ�����Ӱ��Ĵ���װ�ظߵ�ƽʱ�䳤��
		else	CCAP1_tmp += PWM1_low;	//���Ϊ�͵�ƽ�����Ӱ��Ĵ���װ�ص͵�ƽʱ�䳤��
		CCAP1L = (u8)CCAP1_tmp;			//��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
		CCAP1H = (u8)(CCAP1_tmp >> 8);	//��дCCAP0H
	}

	if(CCF2)	//PCAģ��2�ж�
	{
		CCF2 = 0;		//��PCAģ��1�жϱ�־
		if(P37)	CCAP2_tmp += PCA_Timer2;	//���Ϊ�ߵ�ƽ�����Ӱ��Ĵ���װ�ظߵ�ƽʱ�䳤��
		else	CCAP2_tmp += PWM2_low;	//���Ϊ�͵�ƽ�����Ӱ��Ĵ���װ�ص͵�ƽʱ�䳤��
		CCAP2L = (u8)CCAP2_tmp;			//��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
		CCAP2H = (u8)(CCAP2_tmp >> 8);	//��дCCAP0H
	}

	if(CF)	//PCA����ж�
	{
		CF = 0;			//��PCA����жϱ�־
	}

}
