#include "stm32f10x.h"    
#include "freertos.h"
#include "task.h"

void LED_Init(void);

void Buzzer_Init(void);

void task1(void* arg) {
	while (1) {
		vTaskDelay(100);
		GPIO_SetBits(GPIOA, GPIO_Pin_10);
		vTaskDelay(200);
		GPIO_ResetBits(GPIOA, GPIO_Pin_10);
		vTaskDelay(200);
	}
}

void task2(void* arg) {
	while (1) {
		GPIO_SetBits(GPIOA, GPIO_Pin_9);
		vTaskDelay(200);
		GPIO_ResetBits(GPIOA, GPIO_Pin_9);
		vTaskDelay(200);
	}
}

int main(void) {
	LED_Init();
	Buzzer_Init();

	TaskHandle_t hand1, hand2;

	xTaskCreate(task1, "Task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &hand1);
	xTaskCreate(task2, "Task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &hand2);

	vTaskStartScheduler();

	while (1) {
	}
}

//使用PA10 点亮LED
void LED_Init(void) {
	//开启时钟外设
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//初始化引脚, 高电平点亮
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//默认关闭
	GPIO_ResetBits(GPIOA, GPIO_Pin_10);
}
// 使用PA9 控制蜂鸣器
void Buzzer_Init(void) {
	//开启始终外设
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//初始化引脚, 高电平点亮
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//默认关闭
	GPIO_SetBits(GPIOA, GPIO_Pin_9);
}

