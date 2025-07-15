#include "stm32f10x.h"    
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "UART1.h" 
#include <string.h>

SemaphoreHandle_t semaphore;

void Devices_Init(void);

void task1(void* arg) {

	while (1) {
		xSemaphoreTake(semaphore, portMAX_DELAY);
		GPIO_SetBits(GPIOA, GPIO_Pin_1);
		vTaskDelay(1000);
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		xSemaphoreGive(semaphore);
		vTaskDelay(1000);
	}
}


void task2(void* arg) {

	while (1) {
		xSemaphoreTake(semaphore, portMAX_DELAY);
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
		vTaskDelay(1000);
		GPIO_SetBits(GPIOA, GPIO_Pin_2);
		xSemaphoreGive(semaphore);
		vTaskDelay(1000);
	}
}


int main(void) {
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);

	USART1_Init();
	Devices_Init();

	xTaskCreate(task1, "Task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(task2, "Task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	semaphore = xSemaphoreCreateMutex();
	xSemaphoreGive(semaphore);
	vTaskStartScheduler();

	while (1) {
	}
}


void Devices_Init(void) {
	//开启外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//初始化PA1PA2PA3
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//默认关闭LED
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	//默认关闭Buzzer 
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
}
