#include "stm32f10x.h"
#include "MatrixKey.h"
#include "1601a.h"
#include "Delay.h"
#include "LED.h"
#include "Systick.h"
#include "Serial.h"
#include "mp3.h"
#include "config.h"
#include <string.h>

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
	char display[CONFIG_PASSWORD_LEN + 1];
} DefuseAnim;

static AppState app_state = STATE_IDLE;
static DefuseMode defuse_mode = DEFUSE_NONE;

static char arm_input[CONFIG_PASSWORD_LEN + 1];
static char arm_code[CONFIG_PASSWORD_LEN + 1];
static char defuse_input[CONFIG_PASSWORD_LEN + 1];
static uint8_t defuse_pos = 0;

static uint32_t arm_last_change_ms = 0;
static uint32_t countdown_end_ms = 0;
static uint32_t next_beep_ms = 0;
static uint32_t beep_off_ms = 0;
static uint8_t beep_active = 0;

static uint32_t scroll_next_ms = 0;
static uint16_t scroll_pos = 0;
static uint8_t scroll_dir = 0;
static unsigned char scroll_pattern[CONFIG_SCROLL_PATTERN_LEN + 1] = "***";

static uint32_t last_defuse_input_ms = 0;
static uint32_t hash_hold_ms = 0;

static uint8_t startup_beep_state = 0;
static uint32_t startup_beep_next_ms = 0;

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
	for (i = 0; i < CONFIG_PASSWORD_LEN; i++)
	{
		buf[i] = '*';
	}
	buf[CONFIG_PASSWORD_LEN] = '\0';
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
		for (i = 0; i < CONFIG_PASSWORD_LEN - 1; i++)
		{
			buf[i] = buf[i + 1];
		}
		buf[CONFIG_PASSWORD_LEN - 1] = key;
	}
	else if (key == '*')
	{
		for (i = CONFIG_PASSWORD_LEN - 1; i > 0; i--)
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
		if (*pos < CONFIG_PASSWORD_LEN)
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
	return (memcmp(a, b, CONFIG_PASSWORD_LEN) == 0) ? 1U : 0U;
}

static void LCD_ClearLine(void)
{
	static unsigned char spaces[] = "                ";
	LCD_WRITE_StrDATA(spaces, 0);
}

static void LCD_ShowPassword(const char *buf)
{
	LCD_ClearLine();
	LCD_WRITE_StrDATA((unsigned char *)buf, CONFIG_PASSWORD_COL);
}

static void LCD_ShowScroll(uint16_t pos)
{
	LCD_ClearLine();
	LCD_WRITE_StrDATA(scroll_pattern, (unsigned char)pos);
}

static void LED_Breath(uint32_t now)
{
	uint32_t half = CONFIG_LED_BREATH_PERIOD_MS / 2U;
	uint32_t phase = now % CONFIG_LED_BREATH_PERIOD_MS;
	uint16_t level;

	if (phase < half)
	{
		level = (uint16_t)((phase * LED_PWM_MAX) / half);
	}
	else
	{
		level = (uint16_t)(((CONFIG_LED_BREATH_PERIOD_MS - phase) * LED_PWM_MAX) / half);
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
			startup_beep_next_ms = now + CONFIG_STARTUP_BEEP_ON_MS;
			startup_beep_state = 1;
			break;
		case 1:
			Buzzer_Off();
			startup_beep_next_ms = now + CONFIG_STARTUP_BEEP_GAP_MS;
			startup_beep_state = 2;
			break;
		case 2:
			Buzzer_On();
			startup_beep_next_ms = now + CONFIG_STARTUP_BEEP_ON_MS;
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
	countdown_end_ms = now + CONFIG_COUNTDOWN_MS;
	next_beep_ms = now + CONFIG_BEEP_INITIAL_MS;
	beep_off_ms = 0;
	beep_active = 0;
	hash_hold_ms = 0;
	scroll_pos = 0;
	scroll_dir = 0;
	scroll_next_ms = now;
	defuse_mode = DEFUSE_NONE;
	Password_Reset(defuse_input);
	defuse_pos = 0;
	last_defuse_input_ms = 0;
	app_state = STATE_COUNTDOWN;
	LCD_ShowScroll(scroll_pos);
	Buzzer_SetFreq(CONFIG_BUZZER_COUNTDOWN_FREQ_HZ);
#if CONFIG_MP3_ARM_SUCCESS_ENABLE
	mp3_over();
#endif
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
		float interval_ms;

		if (now < countdown_end_ms)
		{
			time_left = countdown_end_ms - now;
		}
		f_complete = (float)time_left / (float)CONFIG_COUNTDOWN_MS;
		interval_ms = (float)CONFIG_BEEP_INTERVAL_BASE_MS + ((float)CONFIG_BEEP_INTERVAL_SCALE_MS * f_complete);
		if (interval_ms < (float)CONFIG_BEEP_INTERVAL_MIN_MS)
		{
			interval_ms = (float)CONFIG_BEEP_INTERVAL_MIN_MS;
		}
		next_beep_ms = now + (uint32_t)(interval_ms);
		beep_active = 1;
		beep_off_ms = now + CONFIG_BEEP_LEN_MS;
		Buzzer_On();
	}
}

static void Countdown_UpdateScroll(uint32_t now)
{
	uint16_t max_pos;

	if (now < scroll_next_ms)
	{
		return;
	}
	scroll_next_ms = now + CONFIG_SCROLL_INTERVAL_MS;
	max_pos = (uint16_t)(CONFIG_LCD_COLS - CONFIG_SCROLL_PATTERN_LEN);
	if (scroll_dir == 0)
	{
		if (scroll_pos < max_pos)
		{
			scroll_pos++;
		}
		if (scroll_pos >= max_pos)
		{
			scroll_dir = 1;
		}
	}
	else
	{
		if (scroll_pos > 0)
		{
			scroll_pos--;
		}
		if (scroll_pos == 0)
		{
			scroll_dir = 0;
		}
	}
	LCD_ShowScroll(scroll_pos);
}

static void DefuseAnim_Reset(void)
{
	defuse_anim.active = 0;
	defuse_anim.mode = DEFUSE_NONE;
	defuse_anim.step_ms = 0;
	defuse_anim.next_step_ms = 0;
	defuse_anim.digit_index = 0;
	defuse_anim.cycle_step = 0;
	Password_Reset(defuse_anim.display);
}

static void DefuseAnim_Start(uint32_t now, DefuseMode mode)
{
	uint32_t duration = (mode == DEFUSE_EXTERNAL) ? CONFIG_EXTERNAL_DEFUSE_MS : CONFIG_MANUAL_DEFUSE_MS;
	uint32_t steps = CONFIG_PASSWORD_LEN * (CONFIG_DEFUSE_CYCLE_STEPS + 1U);
	uint32_t step_ms = duration / steps;

	DefuseAnim_Reset();
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

static uint8_t DefuseAnim_Update(uint32_t now, uint8_t defuser_active, char hold)
{
	if (!defuse_anim.active)
	{
		return 0;
	}
	if (defuse_anim.mode == DEFUSE_EXTERNAL && !defuser_active)
	{
		DefuseAnim_Reset();
		defuse_mode = DEFUSE_NONE;
		LCD_ShowScroll(scroll_pos);
		return 0;
	}
	if (defuse_anim.mode == DEFUSE_MANUAL && hold != '#')
	{
		DefuseAnim_Reset();
		defuse_mode = DEFUSE_NONE;
		LCD_ShowScroll(scroll_pos);
		return 0;
	}
	if (now < defuse_anim.next_step_ms)
	{
		return 0;
	}
	defuse_anim.next_step_ms = now + defuse_anim.step_ms;
	if (defuse_anim.digit_index >= CONFIG_PASSWORD_LEN)
	{
		DefuseAnim_Reset();
		return 1;
	}
	if (defuse_anim.cycle_step < CONFIG_DEFUSE_CYCLE_STEPS)
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
	uint8_t i;
	( void )now;
	Buzzer_Off();
	Relay_Off();
	beep_active = 0;
	DefuseAnim_Reset();
	defuse_mode = DEFUSE_NONE;
	Buzzer_SetFreq(CONFIG_BUZZER_STARTUP_FREQ_HZ);

	LCD_Backlight_On();
	LCD_ClearLine();
	for (i = 0; i < CONFIG_DEFUSE_FLASH_TOGGLES; i++)
	{
		if (i & 1U)
		{
			LED_AllOff();
			Buzzer_Off();
		}
		else
		{
			LED_SetRed(LED_PWM_MAX);
			Buzzer_On();
		}
		LCD_Backlight_Toggle();
		Delay_ms(CONFIG_DEFUSE_FLASH_TOGGLE_MS);
	}

	LCD_WRITE_StrDATA((unsigned char *)arm_code, CONFIG_PASSWORD_COL);
	Delay_ms(CONFIG_DEFUSE_FLASH_TOGGLE_MS);
	for (i = 0; i < CONFIG_DEFUSE_FLASH_TOGGLES; i++)
	{
		if (i & 1U)
		{
			LED_AllOff();
			Buzzer_Off();
		}
		else
		{
			LED_SetRed(LED_PWM_MAX);
			Buzzer_On();
		}
		LCD_Backlight_Toggle();
		Delay_ms(CONFIG_DEFUSE_FLASH_TOGGLE_MS);
	}
	Buzzer_Off();

	LCD_ClearLine();
	LCD_Backlight_Off();
	Delay_ms(CONFIG_DEFUSE_BLINK_MS);
	LCD_WRITE_StrDATA((unsigned char *)arm_code, CONFIG_PASSWORD_COL);
	LCD_Backlight_On();
	Delay_ms(CONFIG_DEFUSE_BLINK_MS);
	LCD_ClearLine();
	LCD_Backlight_Off();
	Delay_ms(CONFIG_DEFUSE_BLINK_MS);
	LCD_WRITE_StrDATA((unsigned char *)arm_code, CONFIG_PASSWORD_COL);
	LCD_Backlight_On();

	LED_SetGreen(LED_PWM_MAX);
	app_state = STATE_DEFUSE_SUCCESS;
}

static void Explosion_Start(uint32_t now)
{
	( void )now;
	Buzzer_Off();
	beep_active = 0;
	DefuseAnim_Reset();
	defuse_mode = DEFUSE_NONE;
	Relay_On();
	LED_SetYellow(LED_PWM_MAX);
	LCD_ClearLine();
#if CONFIG_MP3_EXPLOSION_ENABLE
#if CONFIG_MP3_EXPLOSION_USE_MUSIC
	mp3_boom_music();
#else
	mp3_boom();
#endif
#endif
	app_state = STATE_EXPLODED;
}

int main(void)
{
	uint32_t now;

	Timebase_Init();
	LED_Init();
	Buzzer_Init();
	Buzzer_SetFreq(CONFIG_BUZZER_STARTUP_FREQ_HZ);
	Buzzer_SetDuty(CONFIG_BUZZER_DUTY_PCT);
	DefuserInput_Init();
	LCD_INIT();
	MatrixKey_Init();
	mp3_Init();
	Delay_ms(200);
	MP3CMD(0x06, CONFIG_MP3_VOLUME);

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
				if (Password_IsComplete(arm_input) && (now - arm_last_change_ms) >= CONFIG_ARM_DELAY_MS)
				{
					#if CONFIG_ARM_PRESET_ENABLE
					if (Password_Match(arm_input, CONFIG_ARM_PRESET_PASSWORD))
					{
						memcpy(arm_code, CONFIG_ARM_PRESET_PASSWORD, CONFIG_PASSWORD_LEN + 1);
						Countdown_Start(now);
					}
					#else
					memcpy(arm_code, arm_input, CONFIG_PASSWORD_LEN + 1);
					Countdown_Start(now);
					#endif
				}
				break;
			case STATE_COUNTDOWN:
				#if CONFIG_DEFUSE_ENABLE_MANUAL
				if (defuse_mode == DEFUSE_NONE && hold == '#')
				{
					if (hash_hold_ms == 0)
					{
						hash_hold_ms = now;
					}
					else if ((now - hash_hold_ms) >= CONFIG_LONG_PRESS_MS)
					{
						DefuseAnim_Start(now, DEFUSE_MANUAL);
						hash_hold_ms = 0;
					}
				}
				else
				{
					hash_hold_ms = 0;
				}
				#else
				hash_hold_ms = 0;
				#endif

				#if CONFIG_DEFUSE_ENABLE_EXTERNAL
				if (defuse_mode == DEFUSE_NONE && defuser_active)
				{
					DefuseAnim_Start(now, DEFUSE_EXTERNAL);
				}
				#endif

				if (DefuseAnim_Update(now, defuser_active, hold))
				{
					Defuse_Success(now);
					break;
				}

				#if CONFIG_DEFUSE_ENABLE_PASSWORD
				if (defuse_mode == DEFUSE_NONE)
				{
					if (key != ' ')
					{
						Password_InputLeft(defuse_input, &defuse_pos, key);
						LCD_ShowPassword(defuse_input);
						last_defuse_input_ms = now;
					}

					if (defuse_pos >= CONFIG_PASSWORD_LEN)
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
					if (defuse_pos == 0 && (now - last_defuse_input_ms) > CONFIG_DEFUSE_DISPLAY_HOLD_MS)
					{
						Countdown_UpdateScroll(now);
					}
				}
				#else
				if (defuse_mode == DEFUSE_NONE)
				{
					if ((now - last_defuse_input_ms) > CONFIG_DEFUSE_DISPLAY_HOLD_MS)
					{
						Countdown_UpdateScroll(now);
					}
				}
				#endif

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
				LED_SetYellow(LED_PWM_MAX);
				break;
		default:
			LED_Breath(now);
			break;
		}
	}
}
