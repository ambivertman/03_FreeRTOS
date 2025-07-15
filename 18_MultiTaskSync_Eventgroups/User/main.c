#include "stm32f10x.h"    
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "UART1.h" 
#include <string.h>

#define BIT_0 ( 1 << 0 )
#define BIT_1 ( 1 << 1 )


EventGroupHandle_t xEventGroup;

void LED_Init(void);

void task1(void* arg) {

	while (1) {
		GPIO_SetBits(GPIOA, GPIO_Pin_1);
		xEventGroupSetBits(xEventGroup, BIT_0);
		vTaskDelay(1000);

		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		xEventGroupClearBits(xEventGroup, BIT_0);
		vTaskDelay(1000);
	}
}

void task2(void* arg) {

	while (1) {
		GPIO_SetBits(GPIOA, GPIO_Pin_2);
		xEventGroupSetBits(xEventGroup, BIT_1);
		vTaskDelay(1500);

		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
		xEventGroupClearBits(xEventGroup, BIT_1);
		vTaskDelay(1500);
	}
}

void task3(void* arg) {
	while (1) {
		if (xEventGroupWaitBits(xEventGroup, BIT_0 | BIT_1, pdFALSE, pdTRUE, portMAX_DELAY)) {
			GPIO_SetBits(GPIOA, GPIO_Pin_3);
		}
		GPIO_ResetBits(GPIOA, GPIO_Pin_3);
	}
}

int main(void) {
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);

	USART1_Init();
	LED_Init();

	xEventGroup = xEventGroupCreate();

	xTaskCreate(task1, "Task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(task2, "Task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(task3, "Task3", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

	vTaskStartScheduler();

	while (1) {
	}
}


void LED_Init(void) {
	//开启外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//初始化PA1PA2PA3
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//默认关闭
	GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
}
