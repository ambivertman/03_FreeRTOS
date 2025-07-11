#include "stm32f10x.h"    
#include "freertos.h"
#include "task.h"
#include "UART1.h" 

TaskHandle_t hand1, hand2;

int times = 0;
TaskHandle_t hand1, hand2;

void Print_Task_Info() {
	char Task_info[160];
	vTaskList(Task_info);
	printf1("%s\r\n", Task_info);
}

void task1(void* arg) {
	while (1) {
		printf1("Task 1 is running \r\n");
	}
}

void task2(void* arg) {
	while (1) {
		printf1("Task 2 is running \r\n");
	}
}

void begin_task(void* arg) {
	xTaskCreate(task1, "Task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &hand1);
	xTaskCreate(task2, "Task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &hand2);

	while (1) {
		Print_Task_Info();
		times++;
		if (times == 10) {
			vTaskPrioritySet(hand1, 2);
			vTaskPrioritySet(hand2, 3);
		}
	}
}

int main(void) {
	USART1_Init();
	xTaskCreate(begin_task, "begin_task", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1, NULL);
	vTaskStartScheduler();

	while (1) {
	}
}


