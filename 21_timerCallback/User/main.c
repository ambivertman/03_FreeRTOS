#include "stm32f10x.h"
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "UART1.h"

void Devices_Init(void);

void LED1(TimerHandle_t xTimer) {
	static int toggle = 0;
	if (toggle == 0) {
		GPIO_SetBits(GPIOA, GPIO_Pin_1);
		toggle = 1;
	}
	else {
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		toggle = 0;
	}
}

void LED2(TimerHandle_t xTimer) {
	static int toggle = 0;
	if (toggle == 0) {
		GPIO_SetBits(GPIOA, GPIO_Pin_2);
		toggle = 1;
	}
	else {
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
		toggle = 0;
	}
}

void task1(void* arg) {
	TimerHandle_t xTimer1 = xTimerCreate("LED1", 2000, pdTRUE, NULL, LED1);
	TimerHandle_t xTimer2 = xTimerCreate("LED2", 3000, pdTRUE, NULL, LED2);

	xTimerStart(xTimer1, 0);
	xTimerStart(xTimer2, 0);
	while (1) {
	}
}



int main(void) {
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	USART1_Init();
	Devices_Init();

	xTaskCreate(task1, "Task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	vTaskStartScheduler();
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
	GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);
}
