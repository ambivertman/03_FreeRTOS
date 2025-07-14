#include "stm32f10x.h"    
#include "freertos.h"
#include "task.h"
#include "UART1.h" 

#define OPEN "OK"
#define CLOSE "ERROR"

char Buffer[BUFFER_SIZE] = { 0 };

//使用PA12 点亮LED
void LED_Init(void) {
	//开启时钟外设
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//初始化引脚, 高电平点亮
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//默认关闭
	GPIO_ResetBits(GPIOA, GPIO_Pin_12);
}

void task1(void* arg) {
	printf1("task1 is running\r\n");
	while (1) {
		if (strcmp(Buffer, "OK") == 0) {
			GPIO_SetBits(GPIOA, GPIO_Pin_12);
			printf1("light on\r\n");
			vTaskDelay(1000);
			memset(Buffer, 0, sizeof(Buffer));
		}
		else if (strcmp(Buffer, "ERROR") == 0) {
			GPIO_ResetBits(GPIOA, GPIO_Pin_12);
			printf1("light off\r\n");
			vTaskDelay(1000);
			memset(Buffer, 0, sizeof(Buffer));
		}
	}
}

int main(void) {
	USART1_Init();
	LED_Init();
	//设置USART的中断源分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	USART1_NVIC_Init();


	xTaskCreate(task1, "Task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	vTaskStartScheduler();

	while (1) {
	}
}

