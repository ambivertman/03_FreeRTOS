#include "stm32f10x.h"    
#include "freertos.h"
#include "task.h"

void task1(void* arg);
void task2(void* arg);
void LED_Init(void);
void Buzzer_Init(void);

void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer,
	StackType_t** ppxIdleTaskStackBuffer,
	configSTACK_DEPTH_TYPE* puxIdleTaskStackSize);

//FreeRTOS静态创建任务流程
//为任务开辟空间(一个任务的空间包含两部分TCB与栈)
//需要一个空闲任务在所有的任务都挂起时在CPU上占位
StackType_t StackBuffer_IDLE[configMINIMAL_STACK_SIZE] = { 0 };
StaticTask_t TaksBuffer_IDLE;

//task1的内存
StackType_t StackBuffer_task1[configMINIMAL_STACK_SIZE] = { 0 };
StaticTask_t TaksBuffer_task1;

//task2的内存
StackType_t StackBuffer_task2[configMINIMAL_STACK_SIZE] = { 0 };
StaticTask_t TaksBuffer_task2;




int main(void) {
	LED_Init();
	Buzzer_Init();

	TaskHandle_t hand1, hand2;
	hand1 = xTaskCreateStatic(task1, "task1", configMINIMAL_STACK_SIZE, NULL,
		tskIDLE_PRIORITY + 1, StackBuffer_task1, &TaksBuffer_task1);
	hand2 = xTaskCreateStatic(task2, "task2", configMINIMAL_STACK_SIZE, NULL,
		tskIDLE_PRIORITY + 1, StackBuffer_task2, &TaksBuffer_task2);

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

void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer,
	StackType_t** ppxIdleTaskStackBuffer,
	configSTACK_DEPTH_TYPE* puxIdleTaskStackSize) {
	*ppxIdleTaskTCBBuffer = &TaksBuffer_IDLE;
	*ppxIdleTaskStackBuffer = StackBuffer_IDLE;
	*puxIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

