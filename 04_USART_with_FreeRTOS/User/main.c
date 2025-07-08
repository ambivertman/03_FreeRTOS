#include <stdio.h>
#include "stm32f10x.h"    
#include "freertos.h"
#include "task.h"


int fputc(int ch, FILE* f);
void InitAllDevices(void);
void SendByte(uint8_t Byte);
void task1(void* arg) {


}

int main(void) {
	xTaskCreate(task1, "Task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	vTaskStartScheduler();
	while (1) {
	}
}

void InitAllDevices(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

	//初始化A10/A9 作为USART1 的RX/TX
	GPIO_InitTypeDef GPIO_InitStruct;
	//A10 作为接受采用浮空输入
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	//输入无需设置速度
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//A9 作为输出采用通用推挽
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//输入无需设置速度
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//初始化USART1外设
	USART_InitTypeDef


}