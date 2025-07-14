#include "stm32f10x.h"    
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "UART1.h" 
#include <string.h>

QueueHandle_t queue;

void task1(void* arg) {
	char buf[10] = { 0 };
	char data = 0;
	while (1) {
		BaseType_t ret = xQueueReceive(queue, &data, portMAX_DELAY);
		if (ret == pdTRUE) {
			buf[strlen(buf)] = data;
		}
		if (strcmp(buf, "OK") == 0) {
			GPIO_SetBits(GPIOA, GPIO_Pin_12);
			printf1("light on\r\n");
			vTaskDelay(1000);
			memset(buf, 0, sizeof(buf));
		}
		else if (strcmp(buf, "ERROR") == 0) {
			GPIO_ResetBits(GPIOA, GPIO_Pin_12);
			printf1("light off\r\n");
			vTaskDelay(1000);
			memset(buf, 0, sizeof(buf));
		}
	}
}

int main(void) {
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);

	USART1_Init();

	queue = xQueueCreate(5, 1);

	xTaskCreate(task1, "Task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

	vTaskStartScheduler();

	while (1) {
	}
}


