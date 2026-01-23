#include "LED.h"

static uint16_t led_red = 0;
static uint16_t led_green = 0;
static uint16_t buzzer_pulse = 0;

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

void LED_Init(void)
{
	GPIO_InitTypeDef gpio;
	TIM_TimeBaseInitTypeDef tim;
	TIM_OCInitTypeDef oc;
	uint32_t timer_clk;
	uint16_t prescaler;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_13;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpio);
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);

	gpio.GPIO_Pin = GPIO_Pin_1;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);

	timer_clk = TimerClockPclk1();
	prescaler = (uint16_t)(timer_clk / 1000000U) - 1U;

	tim.TIM_Period = LED_PWM_MAX - 1U;
	tim.TIM_Prescaler = prescaler;
	tim.TIM_ClockDivision = 0;
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &tim);

	oc.TIM_OCMode = TIM_OCMode_PWM1;
	oc.TIM_OutputState = TIM_OutputState_Enable;
	oc.TIM_Pulse = 0;
	oc.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC2Init(TIM2, &oc);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

	TIM_OC3Init(TIM2, &oc);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

	LED_AllOff();
}

void LED_SetColor(uint16_t red, uint16_t green)
{
	if (red > LED_PWM_MAX) { red = LED_PWM_MAX; }
	if (green > LED_PWM_MAX) { green = LED_PWM_MAX; }
	led_red = red;
	led_green = green;
	TIM_SetCompare3(TIM2, led_red);
	TIM_SetCompare2(TIM2, led_green);
}

void LED_SetRed(uint16_t red)
{
	LED_SetColor(red, led_green);
}

void LED_SetGreen(uint16_t green)
{
	LED_SetColor(led_red, green);
}

void LED_SetYellow(uint16_t level)
{
	uint16_t red = (uint16_t)((level * 70U) / 100U);
	LED_SetColor(red, level);
}

void LED_AllOff(void)
{
	LED_SetColor(0, 0);
}

void Relay_On(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
}

void Relay_Off(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
}

void Buzzer_Init(void)
{
	GPIO_InitTypeDef gpio;
	TIM_TimeBaseInitTypeDef tim;
	TIM_OCInitTypeDef oc;
	uint32_t timer_clk;
	uint16_t prescaler;
	uint32_t period;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_0;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);

	timer_clk = TimerClockPclk1();
	prescaler = (uint16_t)(timer_clk / 1000000U) - 1U;
	period = (1000000U / 4250U);
	if (period == 0U) { period = 1U; }

	tim.TIM_Period = (uint16_t)(period - 1U);
	tim.TIM_Prescaler = prescaler;
	tim.TIM_ClockDivision = 0;
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &tim);

	buzzer_pulse = (uint16_t)((period - 1U) / 2U);

	oc.TIM_OCMode = TIM_OCMode_PWM1;
	oc.TIM_OutputState = TIM_OutputState_Enable;
	oc.TIM_Pulse = 0;
	oc.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM3, &oc);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);

	Buzzer_Off();
}

void Buzzer_On(void)
{
	TIM_SetCompare3(TIM3, buzzer_pulse);
}

void Buzzer_Off(void)
{
	TIM_SetCompare3(TIM3, 0);
}
