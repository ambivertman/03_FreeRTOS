#include "stm32f10x.h"    
#include "freertos.h"
#include "task.h"
void task1 ( void * arg ){	
	// ??PB1?Led??
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef initType;
	initType.GPIO_Pin = GPIO_Pin_1 ;
	initType.GPIO_Mode = GPIO_Mode_Out_PP;
	initType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &initType);
	while(1){
			GPIO_SetBits(GPIOB, GPIO_Pin_1);
			vTaskDelay(1000);
			GPIO_ResetBits(GPIOB, GPIO_Pin_1);
			vTaskDelay(1000);
	}
}
void task2 ( void * arg ){
	// ??PA2?Led??
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef initType;
	initType.GPIO_Pin =  GPIO_Pin_2;
	initType.GPIO_Mode = GPIO_Mode_Out_PP;
	initType.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &initType);
	while(1){
			GPIO_SetBits(GPIOA, GPIO_Pin_2);
			vTaskDelay(1000);
			GPIO_ResetBits(GPIOA, GPIO_Pin_2);
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
