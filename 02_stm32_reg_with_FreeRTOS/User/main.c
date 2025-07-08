#include "stm32f10x.h"    
#include "freertos.h"
#include "task.h"
void task1 ( void * arg ){	
	// ??PB0?Led??
	RCC->APB2ENR = 0x0000000C;
	GPIOB->CRL = 0x00000003;
	GPIOB->ODR = 0x00000001;
	while(1){
		GPIOB->ODR = 0x00000001;
		vTaskDelay(1000);
		GPIOB->ODR = 0x00000000;
		vTaskDelay(1000);
	}
}
void task2 ( void * arg ){
	// ??PA1?Led??
	RCC->APB2ENR = 0x0000000C;
	GPIOA->CRL = 0x00000030;
	GPIOA->ODR = 0x00000002;
	while(1){
		GPIOA->ODR = 0x00000002;
		vTaskDelay(1000);
		GPIOA->ODR = 0x00000000;
		vTaskDelay(1000);
	}
}
int main(void){
	xTaskCreate(task1,"Task1",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY+1,NULL);
	xTaskCreate(task2,"Task2",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY+1, NULL);
 vTaskStartScheduler();
	while(1){
	}
}
