#include "1601a.h"
#include "Delay.h"
#include "config.h"
#include "stm32f10x_tim.h"

#define LCD_COLS 16

#define LCD_RS_PORT GPIOB
#define LCD_RS_PIN GPIO_Pin_12
#define LCD_EN_PORT GPIOB
#define LCD_EN_PIN GPIO_Pin_14

#define LCD_D4_PORT GPIOA
#define LCD_D4_PIN GPIO_Pin_4
#define LCD_D5_PORT GPIOA
#define LCD_D5_PIN GPIO_Pin_5
#define LCD_D6_PORT GPIOA
#define LCD_D6_PIN GPIO_Pin_6
#define LCD_D7_PORT GPIOA
#define LCD_D7_PIN GPIO_Pin_7

#define LCD_DATA_PORTA_PINS (LCD_D4_PIN | LCD_D5_PIN | LCD_D6_PIN | LCD_D7_PIN)

#define LCD_BLED_PORT GPIOA
#define LCD_BLED_PIN GPIO_Pin_3

static const char lcd_glyph_chars[] = "0123456789*";

static const uint8_t lcd_glyphs[][8] = {
	{0x00, 0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E},
	{0x00, 0x0E, 0x04, 0x04, 0x04, 0x04, 0x06, 0x04},
	{0x00, 0x1F, 0x02, 0x04, 0x08, 0x10, 0x11, 0x0E},
	{0x00, 0x0E, 0x11, 0x10, 0x0C, 0x10, 0x11, 0x0E},
	{0x00, 0x08, 0x08, 0x1F, 0x09, 0x0A, 0x0C, 0x08},
	{0x00, 0x0E, 0x11, 0x10, 0x10, 0x0F, 0x01, 0x1F},
	{0x00, 0x0E, 0x11, 0x11, 0x0F, 0x01, 0x02, 0x0C},
	{0x00, 0x02, 0x02, 0x02, 0x04, 0x08, 0x10, 0x1F},
	{0x00, 0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E},
	{0x00, 0x06, 0x08, 0x10, 0x1E, 0x11, 0x11, 0x0E},
	{0x00, 0x00, 0x0A, 0x04, 0x1F, 0x04, 0x0A, 0x00},
};

static char lcd_slots[8] = {0};
static uint8_t lcd_slot_next = 0;
static uint16_t lcd_backlight_default = 0;
static uint16_t lcd_backlight_saved = 0;

static void LCD_SetDataNibble(uint8_t data)
{
	if (data & 0x01) { GPIO_SetBits(LCD_D4_PORT, LCD_D4_PIN); } else { GPIO_ResetBits(LCD_D4_PORT, LCD_D4_PIN); }
	if (data & 0x02) { GPIO_SetBits(LCD_D5_PORT, LCD_D5_PIN); } else { GPIO_ResetBits(LCD_D5_PORT, LCD_D5_PIN); }
	if (data & 0x04) { GPIO_SetBits(LCD_D6_PORT, LCD_D6_PIN); } else { GPIO_ResetBits(LCD_D6_PORT, LCD_D6_PIN); }
	if (data & 0x08) { GPIO_SetBits(LCD_D7_PORT, LCD_D7_PIN); } else { GPIO_ResetBits(LCD_D7_PORT, LCD_D7_PIN); }
}

static void LCD_PulseEnable(void)
{
	GPIO_SetBits(LCD_EN_PORT, LCD_EN_PIN);
	Delay_us(1);
	GPIO_ResetBits(LCD_EN_PORT, LCD_EN_PIN);
	Delay_us(50);
}

static void LCD_Write4Bits(uint8_t nibble)
{
	LCD_SetDataNibble(nibble & 0x0F);
	LCD_PulseEnable();
}

static void LCD_Write8Bits(uint8_t value)
{
	LCD_Write4Bits(value >> 4);
	LCD_Write4Bits(value & 0x0F);
}

static void LCD_GPIO_Init(void)
{
	GPIO_InitTypeDef gpio;
	TIM_OCInitTypeDef oc;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	gpio.GPIO_Pin = LCD_RS_PIN | LCD_EN_PIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_OD;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);

	gpio.GPIO_Pin = LCD_DATA_PORTA_PINS;
	gpio.GPIO_Mode = GPIO_Mode_Out_OD;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);

	gpio.GPIO_Pin = LCD_BLED_PIN;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);

	oc.TIM_OCMode = TIM_OCMode_PWM1;
	oc.TIM_OutputState = TIM_OutputState_Enable;
	oc.TIM_Pulse = 0;
	oc.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM2, &oc);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
}

static void LCD_SetCursorPhysical(uint8_t col)
{
	if (col <= 7)
	{
		LCD_WRITE_CMD(0x80 | col);
	}
	else
	{
		LCD_WRITE_CMD(0x80 | (col - 8) + 0x40);
	}
}

static int LCD_GlyphIndex(char ch)
{
	int i;
	for (i = 0; i < (int)(sizeof(lcd_glyph_chars) - 1); i++)
	{
		if (lcd_glyph_chars[i] == ch)
		{
			return i;
		}
	}
	return -1;
}

static void LCD_LoadGlyph(uint8_t slot, const uint8_t *pattern)
{
	uint8_t i;
	LCD_WRITE_CMD(0x40 | (slot << 3));
	for (i = 0; i < 8; i++)
	{
		LCD_WRITE_ByteDATA(pattern[i]);
	}
}

static uint8_t LCD_GetGlyphSlot(char ch)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		if (lcd_slots[i] == ch)
		{
			return i;
		}
	}
	{
		uint8_t slot = lcd_slot_next;
		int idx = LCD_GlyphIndex(ch);
		lcd_slot_next = (uint8_t)((lcd_slot_next + 1) % 8);
		if (idx >= 0)
		{
			LCD_LoadGlyph(slot, lcd_glyphs[idx]);
			lcd_slots[slot] = ch;
		}
		return slot;
	}
}

static uint8_t LCD_EncodeChar(char ch)
{
	if (ch == ' ')
	{
		return (uint8_t)ch;
	}
	if (LCD_GlyphIndex(ch) >= 0)
	{
		return LCD_GetGlyphSlot(ch);
	}
	return (uint8_t)' ';
}

void LCD_INIT(void)
{
	uint8_t i;
	LCD_GPIO_Init();
	for (i = 0; i < 8; i++)
	{
		lcd_slots[i] = 0;
	}
	lcd_slot_next = 0;

	GPIO_ResetBits(LCD_RS_PORT, LCD_RS_PIN);
	GPIO_ResetBits(LCD_EN_PORT, LCD_EN_PIN);

	Delay_ms(20);
	LCD_Write4Bits(0x03);
	Delay_ms(5);
	LCD_Write4Bits(0x03);
	Delay_ms(5);
	LCD_Write4Bits(0x03);
	Delay_ms(1);
	LCD_Write4Bits(0x02);

	LCD_WRITE_CMD(0x28);
	LCD_WRITE_CMD(0x08);
	LCD_WRITE_CMD(0x01);
	Delay_ms(2);
	LCD_WRITE_CMD(0x06);
	LCD_WRITE_CMD(0x0C);

	lcd_backlight_default = (uint16_t)((CONFIG_LED_PWM_MAX * CONFIG_LCD_BACKLIGHT_PCT) / 100U);
	if (lcd_backlight_default > CONFIG_LED_PWM_MAX)
	{
		lcd_backlight_default = CONFIG_LED_PWM_MAX;
	}
	lcd_backlight_saved = lcd_backlight_default;
	LCD_Backlight_Set(lcd_backlight_default);
}

void LCD_WRITE_CMD(unsigned char cmd)
{
	GPIO_ResetBits(LCD_RS_PORT, LCD_RS_PIN);
	LCD_Write8Bits(cmd);
	if (cmd == 0x01 || cmd == 0x02)
	{
		Delay_ms(2);
	}
	else
	{
		Delay_us(50);
	}
}

void LCD_WRITE_ByteDATA(unsigned char data)
{
	GPIO_SetBits(LCD_RS_PORT, LCD_RS_PIN);
	LCD_Write8Bits(data);
	Delay_us(50);
}

void LCD_WRITE_StrDATA(unsigned char *str, unsigned char col)
{
	unsigned char i;
	for (i = 0; str[i] != '\0'; i++)
	{
		unsigned char user_col = (unsigned char)(col + i);
		uint8_t phys_col;
		uint8_t data;
		if (user_col >= LCD_COLS)
		{
			break;
		}
		phys_col = (uint8_t)((LCD_COLS - 1) - user_col);
		data = LCD_EncodeChar((char)str[i]);
		LCD_SetCursorPhysical(phys_col);
		LCD_WRITE_ByteDATA(data);
	}
}

void LCD_WRITE_StrDATA_Password(unsigned char *str, unsigned char col, unsigned char max_len)
{
	unsigned char i;
	for (i = 0; i < max_len && str[i] != '\0'; i++)
	{
		unsigned char user_col = (unsigned char)(col + i);
		uint8_t phys_col;
		char ch;
		int idx;
		uint8_t slot;

		if (user_col >= LCD_COLS)
		{
			break;
		}

		phys_col = (uint8_t)((LCD_COLS - 1) - user_col);
		ch = (char)str[i];
		if (ch == ' ')
		{
			LCD_SetCursorPhysical(phys_col);
			LCD_WRITE_ByteDATA(' ');
			continue;
		}

		idx = LCD_GlyphIndex(ch);
		if (idx >= 0)
		{
			slot = (uint8_t)(i % 8U);
			if (lcd_slots[slot] != ch)
			{
				LCD_LoadGlyph(slot, lcd_glyphs[idx]);
				lcd_slots[slot] = ch;
			}
			LCD_SetCursorPhysical(phys_col);
			LCD_WRITE_ByteDATA(slot);
		}
		else
		{
			LCD_SetCursorPhysical(phys_col);
			LCD_WRITE_ByteDATA(' ');
		}
	}

	for (; i < max_len; i++)
	{
		unsigned char user_col = (unsigned char)(col + i);
		uint8_t phys_col;
		if (user_col >= LCD_COLS)
		{
			break;
		}
		phys_col = (uint8_t)((LCD_COLS - 1) - user_col);
		LCD_SetCursorPhysical(phys_col);
		LCD_WRITE_ByteDATA(' ');
	}
}

void LCD_Clear(void)
{
	LCD_WRITE_CMD(0x01);
	Delay_ms(2);
}

void LCD_Backlight_Set(uint16_t level)
{
	if (level > CONFIG_LED_PWM_MAX)
	{
		level = CONFIG_LED_PWM_MAX;
	}
	if (level > 0U)
	{
		lcd_backlight_saved = level;
	}
	TIM_SetCompare4(TIM2, level);
}

void LCD_Backlight_On(void)
{
	LCD_Backlight_Set(lcd_backlight_saved);
}

void LCD_Backlight_Off(void)
{
	TIM_SetCompare4(TIM2, 0);
}

void LCD_Backlight_Toggle(void)
{
	if (TIM_GetCapture4(TIM2) != 0U)
	{
		TIM_SetCompare4(TIM2, 0);
	}
	else
	{
		if (lcd_backlight_saved == 0U)
		{
			lcd_backlight_saved = lcd_backlight_default;
		}
		LCD_Backlight_Set(lcd_backlight_saved);
	}
}
