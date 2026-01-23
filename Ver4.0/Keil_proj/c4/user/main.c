#include "stm32f10x.h"
#include "MatrixKey.h"
#include "1602a.h"
#include "Delay.h"
#include "LED.h"
#include "Systick.h"
#include "Serial.h"
#include "mp3.h"
#include <string.h>

#define PASSWORD_LEN 7
#define LCD_COLS 16

#define COUNTDOWN_MS 40000U
#define ARM_DELAY_MS 800U
#define BEEP_LEN_MS 125U
#define BREATH_PERIOD_MS 500U
#define SCROLL_INTERVAL_MS 200U
#define LONG_PRESS_MS 1000U
#define MANUAL_DEFUSE_MS 10000U
#define EXTERNAL_DEFUSE_MS 5000U
#define DEFUSE_DISPLAY_HOLD_MS 1200U

typedef enum
{
	STATE_IDLE = 0,
	STATE_COUNTDOWN,
	STATE_DEFUSE_SUCCESS,
	STATE_EXPLODED
} AppState;

typedef enum
{
	DEFUSE_NONE = 0,
	DEFUSE_MANUAL,
	DEFUSE_EXTERNAL
} DefuseMode;

typedef struct
{
	uint8_t active;
	DefuseMode mode;
	uint32_t step_ms;
	uint32_t next_step_ms;
	uint8_t digit_index;
	uint8_t cycle_step;
	char display[PASSWORD_LEN + 1];
} DefuseAnim;

static AppState app_state = STATE_IDLE;
static DefuseMode defuse_mode = DEFUSE_NONE;

static char arm_input[PASSWORD_LEN + 1];
static char arm_code[PASSWORD_LEN + 1];
static char defuse_input[PASSWORD_LEN + 1];
static uint8_t defuse_pos = 0;

static uint32_t arm_last_change_ms = 0;
static uint32_t countdown_end_ms = 0;
static uint32_t next_beep_ms = 0;
static uint32_t beep_off_ms = 0;
static uint8_t beep_active = 0;

static uint32_t scroll_next_ms = 0;
static uint8_t scroll_pos = 0;
static int8_t scroll_dir = 1;

static uint32_t last_defuse_input_ms = 0;
static uint32_t hash_hold_ms = 0;

static uint8_t startup_beep_state = 0;
static uint32_t startup_beep_next_ms = 0;

static uint32_t explosion_music_ms = 0;

static DefuseAnim defuse_anim;

static void DefuserInput_Init(void)
{
    GPIO_InitTypeDef gpio;

    // 1. 开启 GPIOB 和 AFIO 时钟 (必须开启 AFIO)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    // 2. 执行重映射，关闭 JTAG，保留 SWD (这样 PB3, PB4, PA15 才能当 GPIO 用)
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    // 3. 正常配置 GPIO
    gpio.GPIO_Pin = GPIO_Pin_3;
    gpio.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio);
}

static uint8_t Defuser_IsActive(void)
{
	return (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 0) ? 1U : 0U;
}

static void Password_Reset(char *buf)
{
	uint8_t i;
	for (i = 0; i < PASSWORD_LEN; i++)
	{
		buf[i] = '*';
	}
	buf[PASSWORD_LEN] = '\0';
}

static uint8_t Password_IsComplete(const char *buf)
{
	return (buf[0] != '*') ? 1U : 0U;
}

static void Password_InputRight(char *buf, char key)
{
	uint8_t i;
	if (key >= '0' && key <= '9')
	{
		for (i = 0; i < PASSWORD_LEN - 1; i++)
		{
			buf[i] = buf[i + 1];
		}
		buf[PASSWORD_LEN - 1] = key;
	}
	else if (key == '*')
	{
		for (i = PASSWORD_LEN - 1; i > 0; i--)
		{
			buf[i] = buf[i - 1];
		}
		buf[0] = '*';
	}
	else if (key == '#')
	{
		Password_Reset(buf);
	}
}

static void Password_InputLeft(char *buf, uint8_t *pos, char key)
{
	if (key >= '0' && key <= '9')
	{
		if (*pos < PASSWORD_LEN)
		{
			buf[*pos] = key;
			(*pos)++;
		}
	}
	else if (key == '*')
	{
		if (*pos > 0)
		{
			(*pos)--;
			buf[*pos] = '*';
		}
	}
	else if (key == '#')
	{
		Password_Reset(buf);
		*pos = 0;
	}
}

static uint8_t Password_Match(const char *a, const char *b)
{
	return (memcmp(a, b, PASSWORD_LEN) == 0) ? 1U : 0U;
}

static void LCD_ClearLine(void)
{
	static unsigned char spaces[] = "                ";
	LCD_WRITE_StrDATA(spaces, 0);
}

static void LCD_ShowPassword(const char *buf)
{
	LCD_ClearLine();
	LCD_WRITE_StrDATA((unsigned char *)buf, 3);
}

static void LCD_ShowScroll(uint8_t pos)
{
	char line[LCD_COLS + 1];
	uint8_t i;
	for (i = 0; i < LCD_COLS; i++)
	{
		line[i] = ' ';
	}
	line[LCD_COLS] = '\0';
	if (pos < LCD_COLS)
	{
		line[pos] = '*';
	}
	LCD_WRITE_StrDATA((unsigned char *)line, 0);
}

static void LED_Breath(uint32_t now)
{
	uint32_t half = BREATH_PERIOD_MS / 2U;
	uint32_t phase = now % BREATH_PERIOD_MS;
	uint16_t level;

	if (phase < half)
	{
		level = (uint16_t)((phase * LED_PWM_MAX) / half);
	}
	else
	{
		level = (uint16_t)(((BREATH_PERIOD_MS - phase) * LED_PWM_MAX) / half);
	}
	LED_SetYellow(level);
}

static void StartupBeep_Update(uint32_t now)
{
	if (startup_beep_state >= 4)
	{
		return;
	}
	if (now < startup_beep_next_ms)
	{
		return;
	}
	switch (startup_beep_state)
	{
		case 0:
			Buzzer_On();
			startup_beep_next_ms = now + 60U;
			startup_beep_state = 1;
			break;
		case 1:
			Buzzer_Off();
			startup_beep_next_ms = now + 80U;
			startup_beep_state = 2;
			break;
		case 2:
			Buzzer_On();
			startup_beep_next_ms = now + 60U;
			startup_beep_state = 3;
			break;
		case 3:
			Buzzer_Off();
			startup_beep_state = 4;
			break;
		default:
			break;
	}
}

static void Countdown_Start(uint32_t now)
{
	countdown_end_ms = now + COUNTDOWN_MS;
	next_beep_ms = now + 1000U;
	beep_off_ms = 0;
	beep_active = 0;
	hash_hold_ms = 0;
	scroll_pos = 0;
	scroll_dir = 1;
	scroll_next_ms = now;
	defuse_mode = DEFUSE_NONE;
	Password_Reset(defuse_input);
	defuse_pos = 0;
	last_defuse_input_ms = 0;
	app_state = STATE_COUNTDOWN;
	LCD_ShowScroll(scroll_pos);
}

static void Countdown_UpdateBeep(uint32_t now)
{
	if (beep_active && now >= beep_off_ms)
	{
		beep_active = 0;
		Buzzer_Off();
	}
	if (now >= next_beep_ms)
	{
		uint32_t time_left = 0;
		float f_complete;
		float interval;

		if (now < countdown_end_ms)
		{
			time_left = countdown_end_ms - now;
		}
		f_complete = (float)time_left / (float)COUNTDOWN_MS;
		interval = 0.1f + 0.9f * f_complete;
		if (interval < 0.15f)
		{
			interval = 0.15f;
		}
		next_beep_ms = now + (uint32_t)(interval * 1000.0f);
		beep_active = 1;
		beep_off_ms = now + BEEP_LEN_MS;
		Buzzer_On();
	}
}

static void Countdown_UpdateScroll(uint32_t now)
{
	if (now < scroll_next_ms)
	{
		return;
	}
	scroll_next_ms = now + SCROLL_INTERVAL_MS;
	if (scroll_pos == 0)
	{
		scroll_dir = 1;
	}
	else if (scroll_pos >= (LCD_COLS - 1))
	{
		scroll_dir = -1;
	}
	scroll_pos = (uint8_t)(scroll_pos + scroll_dir);
	LCD_ShowScroll(scroll_pos);
}

static void DefuseAnim_Start(uint32_t now, DefuseMode mode)
{
	uint32_t duration = (mode == DEFUSE_EXTERNAL) ? EXTERNAL_DEFUSE_MS : MANUAL_DEFUSE_MS;
	uint32_t steps = PASSWORD_LEN * 11U;
	uint32_t step_ms = duration / steps;

	defuse_anim.active = 1;
	defuse_anim.mode = mode;
	defuse_anim.step_ms = (step_ms == 0U) ? 1U : step_ms;
	defuse_anim.next_step_ms = now;
	defuse_anim.digit_index = 0;
	defuse_anim.cycle_step = 0;
	Password_Reset(defuse_anim.display);
	defuse_mode = mode;
	LCD_ShowPassword(defuse_anim.display);
}

static uint8_t DefuseAnim_Update(uint32_t now, uint8_t defuser_active)
{
	if (!defuse_anim.active)
	{
		return 0;
	}
	if (defuse_anim.mode == DEFUSE_EXTERNAL && !defuser_active)
	{
		defuse_anim.active = 0;
		defuse_mode = DEFUSE_NONE;
		LCD_ShowScroll(scroll_pos);
		return 0;
	}
	if (now < defuse_anim.next_step_ms)
	{
		return 0;
	}
	defuse_anim.next_step_ms = now + defuse_anim.step_ms;
	if (defuse_anim.digit_index >= PASSWORD_LEN)
	{
		defuse_anim.active = 0;
		return 1;
	}
	if (defuse_anim.cycle_step < 10)
	{
		defuse_anim.display[defuse_anim.digit_index] = (char)('0' + defuse_anim.cycle_step);
		defuse_anim.cycle_step++;
	}
	else
	{
		defuse_anim.display[defuse_anim.digit_index] = arm_code[defuse_anim.digit_index];
		defuse_anim.digit_index++;
		defuse_anim.cycle_step = 0;
	}
	LCD_ShowPassword(defuse_anim.display);
	return 0;
}

static void Defuse_Success(uint32_t now)
{
	( void )now;
	Buzzer_Off();
	Relay_Off();
	beep_active = 0;
	defuse_anim.active = 0;
	defuse_mode = DEFUSE_NONE;
	LED_SetGreen(LED_PWM_MAX);
	LCD_ShowPassword(arm_code);
	mp3_over();
	app_state = STATE_DEFUSE_SUCCESS;
}

static void Explosion_Start(uint32_t now)
{
	Buzzer_Off();
	beep_active = 0;
	defuse_anim.active = 0;
	defuse_mode = DEFUSE_NONE;
	Relay_On();
	LED_SetYellow(LED_PWM_MAX);
	LCD_ClearLine();
	mp3_boom();
	explosion_music_ms = now + 200U;
	app_state = STATE_EXPLODED;
}

int main(void)
{
	uint32_t now;

	Timebase_Init();
	LED_Init();
	Buzzer_Init();
	DefuserInput_Init();
	LCD_INIT();
	MatrixKey_Init();
	mp3_Init();
	Delay_ms(200);
	MP3CMD(0x06, 25);

	Password_Reset(arm_input);
	Password_Reset(arm_code);
	Password_Reset(defuse_input);
	LCD_ShowPassword(arm_input);

	startup_beep_next_ms = Timebase_Millis();

	while (1)
	{
		char key;
		char hold;
		uint8_t defuser_active;

		now = Timebase_Millis();
		MatrixKey_Update();
		key = MatrixKey_GetValue();
		hold = MatrixKey_GetHold();
		defuser_active = Defuser_IsActive();

		StartupBeep_Update(now);

		switch (app_state)
		{
			case STATE_IDLE:
				LED_Breath(now);
				if (key != ' ')
				{
					Password_InputRight(arm_input, key);
					LCD_ShowPassword(arm_input);
					arm_last_change_ms = now;
				}
				if (Password_IsComplete(arm_input) && (now - arm_last_change_ms) >= ARM_DELAY_MS)
				{
					memcpy(arm_code, arm_input, PASSWORD_LEN + 1);
					Countdown_Start(now);
				}
				break;
			case STATE_COUNTDOWN:
				if (defuse_mode == DEFUSE_NONE && hold == '#')
				{
					if (hash_hold_ms == 0)
					{
						hash_hold_ms = now;
					}
					else if ((now - hash_hold_ms) >= LONG_PRESS_MS)
					{
						DefuseAnim_Start(now, DEFUSE_MANUAL);
						hash_hold_ms = 0;
					}
				}
				else
				{
					hash_hold_ms = 0;
				}

				if (defuse_mode == DEFUSE_NONE && defuser_active)
				{
					DefuseAnim_Start(now, DEFUSE_EXTERNAL);
				}

				if (DefuseAnim_Update(now, defuser_active))
				{
					Defuse_Success(now);
					break;
				}

				if (defuse_mode == DEFUSE_NONE)
				{
					if (key != ' ')
					{
						Password_InputLeft(defuse_input, &defuse_pos, key);
						LCD_ShowPassword(defuse_input);
						last_defuse_input_ms = now;
					}

					if (defuse_pos >= PASSWORD_LEN)
					{
						if (Password_Match(arm_code, defuse_input))
						{
							Defuse_Success(now);
							break;
						}
						Password_Reset(defuse_input);
						defuse_pos = 0;
						LCD_ShowPassword(defuse_input);
					}
					if (defuse_pos == 0 && (now - last_defuse_input_ms) > DEFUSE_DISPLAY_HOLD_MS)
					{
						Countdown_UpdateScroll(now);
					}
				}

				Countdown_UpdateBeep(now);
			if (beep_active)
			{
				LED_SetColor(LED_PWM_MAX, 0);
			}
			else
			{
				LED_AllOff();
			}

				if (now >= countdown_end_ms)
				{
					Explosion_Start(now);
				}
				break;
			case STATE_DEFUSE_SUCCESS:
				LED_SetGreen(LED_PWM_MAX);
				break;
			case STATE_EXPLODED:
				if (explosion_music_ms != 0 && now >= explosion_music_ms)
				{
					mp3_boom_music();
					explosion_music_ms = 0;
				}
				LED_SetYellow(LED_PWM_MAX);
				break;
		default:
			LED_Breath(now);
			break;
		}
	}
}
