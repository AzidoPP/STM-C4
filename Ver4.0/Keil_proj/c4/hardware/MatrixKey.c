#include "stm32f10x.h"
#include "MatrixKey.h"
#include "Systick.h"

#define COL_PORT GPIOB
#define ROW_PORT GPIOB

#define COL1_PIN GPIO_Pin_5
#define COL2_PIN GPIO_Pin_15
#define COL3_PIN GPIO_Pin_7

#define ROW1_PIN GPIO_Pin_4
#define ROW2_PIN GPIO_Pin_9
#define ROW3_PIN GPIO_Pin_8
#define ROW4_PIN GPIO_Pin_6

static char key_event = ' ';
static char key_stable = ' ';
static char key_sample = ' ';
static uint32_t key_change_ms = 0;

void MatrixKey_Init(void)
{
	GPIO_InitTypeDef gpio;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = COL1_PIN | COL2_PIN | COL3_PIN;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);

	gpio.GPIO_Mode = GPIO_Mode_IPU;
	gpio.GPIO_Pin = ROW1_PIN | ROW2_PIN | ROW3_PIN | ROW4_PIN;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);

	GPIO_SetBits(COL_PORT, COL1_PIN | COL2_PIN | COL3_PIN);
}

static uint8_t MatrixKey_ReadRows(void)
{
	if (GPIO_ReadInputDataBit(ROW_PORT, ROW1_PIN) == 0) { return 1; }
	if (GPIO_ReadInputDataBit(ROW_PORT, ROW4_PIN) == 0) { return 4; }
	if (GPIO_ReadInputDataBit(ROW_PORT, ROW2_PIN) == 0) { return 2; }
	if (GPIO_ReadInputDataBit(ROW_PORT, ROW3_PIN) == 0) { return 3; }
	return 0;
}

static char MatrixKey_ScanRaw(void)
{
	uint8_t key = 0;
	uint8_t row = 0;

	GPIO_SetBits(COL_PORT, COL1_PIN | COL2_PIN | COL3_PIN);
	GPIO_ResetBits(COL_PORT, COL1_PIN);
	row = MatrixKey_ReadRows();
	if (row != 0) { key = row; }

	GPIO_SetBits(COL_PORT, COL1_PIN | COL2_PIN | COL3_PIN);
	GPIO_ResetBits(COL_PORT, COL2_PIN);
	if (key == 0)
	{
		row = MatrixKey_ReadRows();
		if (row != 0) { key = (uint8_t)(row + 4); }
	}

	GPIO_SetBits(COL_PORT, COL1_PIN | COL2_PIN | COL3_PIN);
	GPIO_ResetBits(COL_PORT, COL3_PIN);
	if (key == 0)
	{
		row = MatrixKey_ReadRows();
		if (row != 0) { key = (uint8_t)(row + 8); }
	}

	GPIO_SetBits(COL_PORT, COL1_PIN | COL2_PIN | COL3_PIN);

	switch (key)
	{
		case 1: return '1';
		case 2: return '4';
		case 3: return '7';
		case 4: return '*';
		case 5: return '2';
		case 6: return '5';
		case 7: return '8';
		case 8: return '0';
		case 9: return '3';
		case 10: return '6';
		case 11: return '9';
		case 12: return '#';
		default: return ' ';
	}
}

void MatrixKey_Update(void)
{
	uint32_t now = Timebase_Millis();
	char sample = MatrixKey_ScanRaw();

	if (sample != key_sample)
	{
		key_sample = sample;
		key_change_ms = now;
	}

	if ((now - key_change_ms) >= 20)
	{
		if (key_stable != key_sample)
		{
			key_stable = key_sample;
			if (key_stable != ' ')
			{
				key_event = key_stable;
			}
		}
	}
}

char MatrixKey_GetValue(void)
{
	char key = key_event;
	key_event = ' ';
	return key;
}

char MatrixKey_GetHold(void)
{
	return key_stable;
}
