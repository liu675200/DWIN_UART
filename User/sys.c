#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK  STM32������
//ϵͳ�жϷ������û�		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/10
//�汾��V1.4
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************  
//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
__asm void WFI_SET(void)
{
	//__ASM volatile("wfi");		
	WFI;
}
//�ر������ж�
__asm void INTX_DISABLE(void)
{		  
	//__ASM volatile("cpsid i");
	CPSID i;
}
//���������ж�
__asm void INTX_ENABLE(void)
{
	//__ASM volatile("cpsie i");		
	CPSIE i;	
}
//����ջ����ַ
//addr:ջ����ַ

__ASM void MSR_MSP(u32 addr)
{
	MSR MSP, r0 //set Main Stack value
	BX r14
}


/*void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
	__ASM volatile("MSR MSP, r0"); 
__ASM volatile("BX r14");
}
*/
