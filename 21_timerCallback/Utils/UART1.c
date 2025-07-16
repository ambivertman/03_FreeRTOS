// UART1.c
#include "UART1.h"

void USART1_Init(void){
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
	
	// 初始化中断: 用中断, 读取串口1回传的信息
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 12;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	

	// 启动USART1
	USART_Cmd(USART1, ENABLE);
}


// 串口1的中断处理函数
void USART1_IRQHandler(void) {
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
			
		// 读取一个字节
		char data = USART_ReceiveData(USART1);


		// 存储到消息队列:  queue
		// 注意中断不是任务,  没有TCB这个东西, 可以让出CPU, 但是没有办法进入阻塞列表
		// 注意: 中断语法上允许阻塞, 使用和设计上阻塞是一件极其荒谬的事情 ( 中断要迅速响应和结束)
		
		
		// 参数1:  向那个消息队列中存放消息
		// 参数2:  要存储的消息
		// 参数3:  如果此时此刻, 有任务等待读数据(因为等待读数据陷入阻塞)
		//                 给它pdTRUE, 等中断结束, 主动触发任务切换 
//		BaseType_t bt = pdTRUE;
//		xQueueSendFromISR(queue, (void *)&data,  &bt);
				
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

// 发送字节
void USART1_SendByte(uint8_t Byte){
	USART_SendData(USART1, Byte);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}


void printf1(char *format, ...)
{
	
	char strs[100] = {0};	

	va_list list;
	va_start(list, format);
	vsprintf(strs, format, list);
	va_end(list);
	
	// 先获取信号量
	//xSemaphoreTake(semaphr, portMAX_DELAY);
	for (uint8_t i=0; strs[i] != '\0'; i++){
		USART1_SendByte(strs[i]);
	}
	// 释放信号量
	//xSemaphoreGive(semaphr);
	
  // vTaskDelay(2);
}
