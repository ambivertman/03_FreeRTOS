#include "stm32f10x.h"    
#include "freertos.h"
#include "task.h"
#include "UART1.h" 

TaskHandle_t hand1, hand2;

void Print_Task_Info(TaskHandle_t TaskHandle) {
	TaskStatus_t xTaskDetails;
	vTaskGetInfo(TaskHandle, &xTaskDetails, pdTRUE, eInvalid);
	printf1("Task Name: %s \r\n", xTaskDetails.pcTaskName);
	printf1("Task Priority: %d\r\n", xTaskDetails.uxCurrentPriority);
	printf1("Base Priority: %d\r\n", xTaskDetails.uxBasePriority);
	printf1("Task State: %d\r\n", xTaskDetails.eCurrentState);
	printf1("Stack Base Address: %p\r\n", xTaskDetails.pxStackBase);
	printf1("Stack High Water Mark: %d\r\n", xTaskDetails.usStackHighWaterMark);
	printf1("Task Run Time: %d ticks\r\n", xTaskDetails.ulRunTimeCounter);
	printf1("\r\n");
}

void task1(void* arg) {
	while (1) {
		vTaskDelay(1000);
		Print_Task_Info(hand1);

		vTaskDelay(1000);
	}
}

void task2(void* arg) {
	while (1) {
		vTaskDelay(2000);
		Print_Task_Info(hand2);
	}
}


int main(void) {
	USART1_Init();

	xTaskCreate(task1, "Task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &hand1);
	xTaskCreate(task2, "Task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &hand2);

	vTaskStartScheduler();

	while (1) {
	}
}


