#include "IRM.h"
#include "uart.h"

#define THRESHOLD ((unsigned int)(((FOSC * 7UL) + 6000000UL) / 12000000UL))
	
#define irtimeH  (unsigned int)(FOSC / 12000000 * 63)
#define irtimeL  (unsigned int)(FOSC / 12000000 * 33)
	
unsigned char  irtime;
bit irpro_ok,irok;
unsigned int IRcord[4];
unsigned char irdata[33];


void Ir_work(void)//�����ֵɢת����
{
		
		if(IRcord[2] == 0x700)
		{
			printf("P54 toggled\n");
			P54=~P54;
		}
		else if(IRcord[2] == 0x900)
		{
			printf("P55 toggled\n");
			P55=~P55;
		}
		else
		{
			printf("irm date: 0x%x",IRcord[2]);
		}
		irpro_ok=0;
}

void Ircordpro(void)//������ֵ������
{ 
	unsigned char i, j, k,cord,value;
	k=1;
	for(i=0;i<4;i++)      //����4���ֽ�
	{
		for(j=1;j<=8;j++) //����1���ֽ�8λ
		{
			cord=irdata[k];
			if (cord > THRESHOLD)
			value=value|0x80;
			if(j<8)
			{
				value>>=1;
			}
			k++;
		}
		IRcord[i]=value;
		value=0;     
	} 
	irpro_ok=1;
}

void Timer0_Init(void)//��ʱ��0��ʼ��
{
	TMOD=0x02;//��ʱ��0������ʽ2��TH0����װֵ��TL0�ǳ�ֵ
	TH0=0x00; //����ֵ
	TL0=0x00; //��ʼ��ֵ
	ET0=1;    //���ж�
	TR0=1;    
}
void INT0_Init(void)
{
	IT0 = 1;   //ָ���ⲿ�ж�0�½��ش�����INT0 (P3.2)
	EX0 = 1;   //ʹ���ⲿ�ж�
	EA = 1;    //�����ж�
}
void irm_init(void)
{
    Timer0_Init();
		INT0_Init();
}

void EX0_ISR (void) interrupt 0
{
	static unsigned char  i;             //���պ����źŴ���
	static bit startflag=0;                //�Ƿ�ʼ�����־λ
	if(startflag)                         
	{
		if(irtime<irtimeH&&irtime>=irtimeL)//������ TC9012��ͷ�룬9ms+4.5ms
            i=0;
    		irdata[i]=irtime;//�洢ÿ����ƽ�ĳ���ʱ�䣬�����Ժ��ж���0����1
    		irtime=0;
    		i++;
   			if(i==33)
				{
	  			irok=1;
					i=0;
	  		}
	}
	else
	{
		irtime=0;
		startflag=1;
	}
}

void tim0_isr (void) interrupt 1 using 1
{
  irtime++;
}