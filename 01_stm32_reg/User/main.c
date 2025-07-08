// ??PA1?PA2??
#include "stm32f10x.h"      
int main(void){	
	RCC->APB2ENR = 0x00000004;
	GPIOA->CRL = 0x00000330;
	GPIOA->ODR = 0x00000006;
	while(1){
	}
}
