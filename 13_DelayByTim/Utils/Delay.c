#include "Delay.h"

void Delay_Init(void) {
    //开启外设时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    //初始化外设
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = 0xFFFF;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 71;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

    //开启外设
    TIM_Cmd(TIM2, ENABLE);

}

void Delay_us(uint16_t us) {
    //重置CNT计数器
    TIM_SetCounter(TIM2, 0);
    while (TIM_GetCounter(TIM2) < us);
}

void Delay_ms(uint16_t ms) {
    Delay_us(1000);
}