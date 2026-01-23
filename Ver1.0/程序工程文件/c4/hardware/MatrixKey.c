#include "stm32f10x.h"                  // Device header
#include "Delay.h"
uint8_t MatrixKey_determine();

void MatrixKey_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_7 | GPIO_Pin_5 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_4 | GPIO_Pin_9 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
}

uint8_t MatrixKey_GetValue()
{
	uint8_t Key=0;
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
	GPIO_SetBits(GPIOB, GPIO_Pin_15);

	if(MatrixKey_determine())
	{
		Key = MatrixKey_determine();
	}
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
	

	if(MatrixKey_determine())
	{
		Key = MatrixKey_determine() + 4;
	}
	
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_7); 
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
	
	if(MatrixKey_determine())
	{
		Key = MatrixKey_determine() + 8;
	}
	
	switch(Key){
		case 1: Key='1';
				break;
		case 2: Key='4';
				break;
		case 3: Key='7';
				break;
		case 4: Key='*';
				break;
		case 5: Key='2';
				break;
		case 6: Key='5';
				break;
		case 7: Key='8';
				break;
		case 8: Key='0';
				break;
		case 9: Key='3';
				break;
		case 10: Key='6';
				break;
		case 11: Key='9';
				break;
		case 12: Key='#';
				break;
		default: Key=' ';
	}
	return Key;
}

uint8_t MatrixKey_determine()
{
	uint8_t Key = 0;
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0)
	{
		Delay_ms(10);
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0){Key = 1;}
	}
	
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0)
	{
		Delay_ms(10);
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0){Key = 4;}
	}
	
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 0)
	{
		Delay_ms(10);
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 0){Key = 2;}
	}
	
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0)
	{
		Delay_ms(10);
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0){Key = 3;}
	}
	return Key;
}