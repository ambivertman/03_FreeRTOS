// UART1.c
#include "UART1.h"

void USART1_Init(void) {
	// 开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	// 初始化引脚
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//PA9:USART1_TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10:USART1_RX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// USART配置
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);

	// 启动USART1
	USART_Cmd(USART1, ENABLE);
}

// 发送字节
void USART1_SendByte(uint8_t Byte) {
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}


void printf1(char* format, ...) {
	char strs[100];

	// 替换内容 -> 存储到strs
	va_list list;
	va_start(list, format);
	vsprintf(strs, format, list);
	va_end(list);

	// strs: 通过串口发走
	for (uint8_t i = 0; strs[i] != '\0'; i++) {
		USART1_SendByte(strs[i]);
	}

}


void USART1_NVIC_Init(void) {
	//中断源:发送数据寄存器非空
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	//初始化NVIC设备
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 12;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;

	NVIC_Init(&NVIC_InitStruct);
}

void USART1_IRQHandler(void) {
	// printf1("irq is running\r\n");
	/*如果在这里加上一个调试输出, 那么硬件的工作逻辑是
	1.接收到第一次字符, 触发中断
	2.通过串口输出调试信息,串口被占用,导致后续数据丢失
	3.最后导致每次输入一次只能接收一个字符
	*/
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
		char data = USART_ReceiveData(USART1);
		printf1("%c", data);
		Buffer[strlen(Buffer)] = data;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
