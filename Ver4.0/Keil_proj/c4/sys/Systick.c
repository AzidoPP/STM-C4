#include "stm32f10x.h"
#include "Systick.h"

volatile uint32_t g_time_ms = 0;

static uint32_t TimerClockPclk1(void)
{
	RCC_ClocksTypeDef clocks;
	RCC_GetClocksFreq(&clocks);
	if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_CFGR_PPRE1_DIV1)
	{
		return clocks.PCLK1_Frequency * 2U;
	}
	return clocks.PCLK1_Frequency;
}

void Timebase_Init(void)
{
	TIM_TimeBaseInitTypeDef tim;
	NVIC_InitTypeDef nvic;
	uint32_t timer_clk;
	uint16_t prescaler;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	timer_clk = TimerClockPclk1();
	prescaler = (uint16_t)(timer_clk / 1000000U) - 1U;

	tim.TIM_Period = 1000U - 1U;
	tim.TIM_Prescaler = prescaler;
	tim.TIM_ClockDivision = 0;
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &tim);

	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

	nvic.NVIC_IRQChannel = TIM4_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	TIM_Cmd(TIM4, ENABLE);
}

void Timebase_Tick(void)
{
	g_time_ms++;
}

uint32_t Timebase_Millis(void)
{
	return g_time_ms;
}
