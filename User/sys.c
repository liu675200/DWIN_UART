#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK  STM32开发板
//系统中断分组设置化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/10
//版本：V1.4
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************  
//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
__asm void WFI_SET(void)
{
	//__ASM volatile("wfi");		
	WFI;
}
//关闭所有中断
__asm void INTX_DISABLE(void)
{		  
	//__ASM volatile("cpsid i");
	CPSID i;
}
//开启所有中断
__asm void INTX_ENABLE(void)
{
	//__ASM volatile("cpsie i");		
	CPSIE i;	
}
//设置栈顶地址
//addr:栈顶地址

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

