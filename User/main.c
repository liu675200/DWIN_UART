#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "DWIN.h"
#include "exti.h"
#include "mt.h"
#include "beep.h"
#include "timer.h"


u16 data[20];

u8 mode = 0;		//�ܿ��أ���������ϵͳ������
u8 LCD_EN = 1;	//������ʾ���Ƿ��ƶ�
u8 BEEP_EN = 1;		//���Ʒ������Ƿ���
u8 BEEP_CN = 0;		//������ʱ�����
u8 M_EN = 1;			//���������
u16 frq = 0;


void StartMain(void)
{
	if(LCD_EN == 1)
	{
			SetFont(41);
	}
	else
	{
			SetFont(0);
	}
	if(BEEP_EN == 1)
	{
			BEEP_CN = 1;
	}
	if(M_EN == 1)
	{
			MT = 1;
	}
	delay_ms(200);
	BEEP_CN = 0;
	MT = 0;
	delay_ms(800);
	
	if(BEEP_EN == 1)
	{
			BEEP_CN = 1;
	}
	if(M_EN == 1)
	{
			MT = 1;
	}
	delay_ms(200);
	BEEP_CN = 0;
	MT = 0;
	delay_ms(800);
}

int main(void)
{
	delay_init();	    	 //��ʱ������ʼ��
	uart_init(115200);	 //����1��ʼ��Ϊ115200
	DWIN_Init(115200);
	LED_Init();
	MT_Init();
	BEEP_Init();
	SetFont(0);
	SetData(0x1013,0);
	SetData(0x1010,LCD_EN);
	SetData(0x1011,BEEP_EN);
	SetData(0x1012,M_EN);
	EXTIX_Init();
	delay_ms(1000);
	frq = GetData(0x1020,1);
	TIM3_Int_Init(1999,18000/frq);			//72000/719/

	while(1)
	{
			if(mode!=0){
					StartMain();
			}
			else
			{
				LED =! LED;
				delay_ms(300);
			}
	}
}
void EXTI9_5_IRQHandler(void)
{
	delay_ms(100);
		if(EXTI_GetITStatus(EXTI_Line6)!=RESET)
		{
				mode =! mode;
			if(mode ==0)
			{
				SetData(0x1013,0);
				printf("ϵͳֹͣ���� .... \r\n���ؿ�ʼ���� .... \r\n");
				LED = 0;
				BEEP_CN = 0;
				MT = 0;
				SetFont(0);
			}
			else{
				
				SetData(0x1013,1);
				LED = 1;
				frq = GetData(0x1020,1);
				TIM3_Int_Init(1999,18000/frq);			//72000/719/
				printf("ϵͳ��ʼ���� .... \r\n");
			}
				EXTI_ClearITPendingBit(EXTI_Line6);
		}
		if(EXTI_GetITStatus(EXTI_Line7)!=RESET)
		{
				LCD_EN =! LCD_EN;
				if(LCD_EN == 0)
				{
						printf("��Ļ�ر� ... \r\n");
						SetData(0x1010,0x00);
				}
				else
				{
						printf("��Ļ�� ... \r\n");
						SetData(0x1010,0x01);
				}
				EXTI_ClearITPendingBit(EXTI_Line7);
		}
		if(EXTI_GetITStatus(EXTI_Line8)!=RESET)
		{
				BEEP_EN =! BEEP_EN;
				if(BEEP_EN == 0)
				{
						SetData(0x1011,0x00);
						printf("�������ر� ... \r\n");
				}
				else
				{
						frq = GetData(0x1020,1);
						TIM3_Int_Init(1999,18000/frq);
						SetData(0x1011,0x01);
						printf("�������� ...\r\n");
				}
				EXTI_ClearITPendingBit(EXTI_Line8);
		}
		if(EXTI_GetITStatus(EXTI_Line9)!=RESET)
		{
				M_EN =! M_EN;
				if(M_EN == 0)
				{
					SetData(0x1012,0x00);
					printf("����ر� ... \r\n");
				}
				else
				{
					SetData(0x1012,0x01);
					printf("����� ... \r\n");
				}
				EXTI_ClearITPendingBit(EXTI_Line9);
		}
		
}

