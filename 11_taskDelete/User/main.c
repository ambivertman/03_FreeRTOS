#include "stm32f10x.h"    
#include "freertos.h"
#include "task.h"
#include "UART1.h" 

TaskHandle_t hand1, hand2, begin_task_hand;

int times = 0;

void Print_Task_Info() {
	char Task_info[160];
	vTaskList(Task_info);
	printf1("%s\r\n", Task_info);
}

void task1(void* arg) {
	while (1) {
		vTaskDelay(250);
		printf1("Task 1 is running \r\n");
		vTaskDelay(750);
	}
}

void task2(void* arg) {
	while (1) {
		vTaskDelay(500);
		printf1("Task 2 is running \r\n");
		vTaskDelay(500);
		times++;
		printf1("times: %d\r\n", times);
	}
}

void begin_task(void* arg) {
	xTaskCreate(task1, "Task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &hand1);
	xTaskCreate(task2, "Task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &hand2);

	while (1) {
		vTaskDelay(1000);
		Print_Task_Info();
		vTaskDelay(1000);
		if (times >= 5 && hand1 != NULL) {
			vTaskDelete(hand1);
			hand1 = NULL;
		}
		if (times >= 6 && begin_task_hand != NULL) {
			vTaskDelete(begin_task_hand);
			begin_task_hand = NULL;
		}
	}
}

int main(void) {
	USART1_Init();
	xTaskCreate(begin_task, "begin_task", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1, &begin_task_hand);
	vTaskStartScheduler();

	while (1) {
	}
}


