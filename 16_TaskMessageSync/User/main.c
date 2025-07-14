#include "stm32f10x.h"    
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "UART1.h" 
#include <string.h>

QueueHandle_t queue;

void task1(void* arg) {
	char buf1[10] = "hihao\r\n";
	while (1) {
		xQueueSend(queue, buf1, portMAX_DELAY);
		vTaskDelay(1000);
	}
}

void task2(void* arg) {
	char buf2[10] = { 0 };
	while (1) {
		if (xQueueReceive(queue, buf2, portMAX_DELAY)) {
			printf1("%s", buf2);
		}
		vTaskDelay(1000);
	}
}


int main(void) {
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);

	USART1_Init();

	queue = xQueueCreate(5, 10);

	xTaskCreate(task1, "Task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(task2, "Task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

	vTaskStartScheduler();

	while (1) {
	}
}


