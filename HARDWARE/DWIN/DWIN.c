#include "DWIN.h"

u8 Num = 0;

u8 times = 0;

u8 USART3_RX_STA[20] = { 0 };

struct voice{
	int date;
	int qian;
	int qian_flag; //
	int bai;
	int ge;
	int node;
};

u8 FontBuf[10] = {0x5A,0xA5,0x07,0x82,0x00,0x84,0x5A,0x01,0x00,0x00};	//设置图片  发送数据包
u8 DataBuf[8] = {0x5A,0xA5,0x05,0x82,0x00,0x00,0x00,0x00};		//设置变量值  发送数据包
u8 getComm[7] = {0x5A,0xA5,0x04,0x83,0x00,0x00,0x00};					//获取变量值  发送数据包

u8 str[6] = {0x5A,0xA5,0x03,0x82,0x4F,0x4B};		//0x82 设置后返回字符

u8 rec_str[9] = {0x5A,0xA5,0x06,0x83,0x00,0x00,0x00,0x00,0x00}; //0x83获取数据返回

u8 getData[4] = {0x00,0x00,0x00,0x00};		//接收数据缓冲


void dataCalc(struct voice voice)
{
	voice.qian = voice.date /1000;
	//voice.qian_flag ;
	voice.bai = voice.date/100%10;
	voice.ge = voice.date/10%10;
	voice.node = voice.date;
}
void DWIN_Init(u32 baudRate)			//串口
{
	NVIC_InitTypeDef 	NVIC_InitStruct;//定义一个设置中断的结构体	
	USART_InitTypeDef USART_InitSturct;//定义串口1的初始化结构体
	GPIO_InitTypeDef GPIO_InitStruct;//定义串口对应管脚的结构体

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);//打开串口管脚时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	//USART3_Tx_Pin Configure 	B10
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;//输出引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;//设置最高速度50MHz
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//推挽复用输出
	GPIO_Init(GPIOB , &GPIO_InitStruct);//将初始化好的结构体装入寄存器

//USART3_Rx_Pin Configure  B11
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO模式悬浮输入
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;//输入引脚
  GPIO_Init(GPIOB, &GPIO_InitStruct);//将初始化好的结构体装入寄存器

//USART3 Configure	
	USART_InitSturct.USART_BaudRate = baudRate;//波特率115200
	USART_InitSturct.USART_WordLength = USART_WordLength_8b;//数据宽度8位
	USART_InitSturct.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitSturct.USART_Parity = USART_Parity_No;//无奇偶校验
	USART_InitSturct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitSturct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//使能发送与接收
	USART_Init(USART3 , &USART_InitSturct);//将初始化好的结构体装入寄存器	
	//USART1_INT Configure
	USART_ITConfig(USART3 , USART_IT_RXNE , ENABLE);//使能接收中断
//	USART_ITConfig(USART3 , USART_IT_TXE , ENABLE);
	USART_Cmd(USART3 , ENABLE);//打开串口
	USART_ClearFlag(USART3 , USART_FLAG_TC);//解决第一个数据发送失败的问题
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//打开该中断
	NVIC_Init(&NVIC_InitStruct);
}

u8 myStrcmp(u8 *s,u8 *p,u8 t)	//得到的字符串，比较的字符串，比较的长度
{
		u8 i;
		for(i = 0; i < t; i++)
		{
				if(s[i] != p[i])
				{
						return 0;
				}
		}
		return 1;
}

void PutChar(u8 Data)		//发送一个字符
{
//	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	
	USART_SendData(USART3 , Data);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);//等待发送完毕
}


void setVoice(float date)
{
	u8 i = 0;
	struct voice temp;
	temp.date = (int)date;
	dataCalc(temp);
	//953.6
}
void SetFont(u8 id)
{
	u8 i,m;
	times = 0;
	Num = 0;	//重新接收数据
	FontBuf[9] = id;
	for(i = 0; i <10; i++)
	{
		PutChar(FontBuf[i]);
	}
	while(Num < 6)
	{
		times ++;
		delay_ms(10);
		if(times>20)
		{
			times = 0;
			printf("SetFont times out \r\n");
			break;
		}
	}
	m = myStrcmp(USART3_RX_STA,str,6);
	for(i = 0; i < 6; i++)
	{
			printf("%d",USART3_RX_STA[i]);
	}
	printf("\r\n SetFont strcmp value is <%d> \r\n",m);
}


void SetData(u16 addr,u16 data)
{
	u8 i,m;
	
	times = 0;
	Num = 0;		//重新接收数据

	DataBuf[4] = addr>>8;
	DataBuf[5] = addr&0x00FF;
	DataBuf[6] = data>>8;
	DataBuf[7] = data&0x00FF;
	
	for(i = 0; i < 8; i++)
	{
			PutChar(DataBuf[i]);
	}
	
	while(Num < 6)
	{
			times ++;
			delay_ms(10);
			if(times>20)
			{
				times = 0;
				printf("SetData times out \r\n");
				break;
			}
	}
	
	m = myStrcmp(USART3_RX_STA,str,6);
	
	for(i = 0; i < 6; i++)
	{
			printf("%d",USART3_RX_STA[i]);
	}
	
	printf("\r\n SetData strcmp value is <%d> \r\n",m);
}

u16 GetData(u16 addr, u8 len)
{
	u8 i,m;
	u16 Buffer;
	times = 0;
	Num = 0;		//重新接收数据
	
	getComm[4] = addr>>8;
	getComm[5] = addr&0x00FF;
	getComm[6] = len;
	
	rec_str[4] = getComm[4];
	rec_str[5] = getComm[5];
	rec_str[6] = getComm[6];
	for(i = 0; i < 7;i++)
	{
		PutChar(getComm[i]);
	}
	while(Num < 9)
	{
		
			times ++;
			delay_ms(10);
			if(times>20)
			{
				times = 0;
				printf("getData times out \r\n");
				break;
			}
	}
	m = myStrcmp(USART3_RX_STA,rec_str,6);
	for(i = 0; i < 9; i++)
	{
			printf("%d",USART3_RX_STA[i]);
	}
	
	printf("\r\n GetData strcmp value is <%d> \r\n",m);
	
	Buffer = USART3_RX_STA[7]<<8|USART3_RX_STA[8];
	printf("buffer : <%d>\r\n",Buffer);
	
	if(Buffer > 3000 || Buffer < 100)
	{
		return 1000;
	}
	return Buffer;
}

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
	{
		USART3_RX_STA[Num++] =USART_ReceiveData(USART3);	//读取接收到的数据
	}
}










/*   ------------ end file ---------*/



