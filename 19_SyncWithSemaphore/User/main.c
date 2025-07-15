#include "stm32f10x.h"    
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "UART1.h" 
#include <string.h>

SemaphoreHandle_t xBinarySemaphore;

void Devices_Init(void);

void task1(void* arg) {

	while (1) {

		// 控制灯亮
		GPIO_SetBits(GPIOA, GPIO_Pin_1);
		xSemaphoreGive(xBinarySemaphore);
		vTaskDelay(1000);

		// 控制灯灭
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		xSemaphoreGive(xBinarySemaphore);
		vTaskDelay(1000);
	}
}

void task2(void* arg) {
	BaseType_t toggle = 0;

	while (1) {
		if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE) {
			if (toggle == 0) {
				GPIO_SetBits(GPIOA, GPIO_Pin_2);
				toggle = 1;
			}
			else {
				GPIO_ResetBits(GPIOA, GPIO_Pin_2);
				toggle = 0;
			}
		}
	}
}



int main(void) {
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);

	USART1_Init();
	Devices_Init();


	xTaskCreate(task1, "Task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(task2, "Task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	xBinarySemaphore = xSemaphoreCreateBinary();
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
