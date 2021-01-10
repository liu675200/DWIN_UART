#ifndef __DWIN_H_
#define __DWIN_H_

#include "stdio.h"
#include "delay.h"
#include "usart.h"
void DWIN_Init(u32 baudRate);

void SetFont(u8 id);
void SetData(u16 addr,u16 data);
u16 GetData(u16 addr, u8 len);

#endif

/*   ------------ end file ---------*/

