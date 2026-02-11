#include "stm32f10x.h"
#include "MatrixKey.h"
#include "1601a.h"
#include "Delay.h"
#include "LED.h"
#include "Systick.h"
#include "mp3.h"
#include "config.h"
#include "config_manager.h"
#include <math.h>
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
	char display[CONFIG_PASSWORD_LEN_MAX + 1U];
} DefuseAnim;

typedef enum
{
	MP3_SEQ_NONE = 0,
	MP3_SEQ_DEFUSE,
	MP3_SEQ_EXPLOSION
} Mp3Sequence;

typedef enum
{
	CFG_MODE_IDLE = 0,
	CFG_MODE_INPUT_ID,
	CFG_MODE_CONFIRM_RESTORE,
	CFG_MODE_INPUT_VALUE,
	CFG_MODE_CONFIRM_SAVE
} ConfigModeState;

static AppState app_state = STATE_IDLE;
static DefuseMode defuse_mode = DEFUSE_NONE;

static char arm_input[CONFIG_PASSWORD_LEN_MAX + 1U];
static char arm_code[CONFIG_PASSWORD_LEN_MAX + 1U];
static char defuse_input[CONFIG_PASSWORD_LEN_MAX + 1U];
static uint8_t defuse_pos = 0;

static uint32_t arm_last_change_ms = 0;
static uint32_t countdown_end_ms = 0;
static uint32_t next_beep_ms = 0;
static uint32_t beep_off_ms = 0;
static uint8_t beep_active = 0;

static uint32_t scroll_next_ms = 0;
static uint16_t scroll_pos = 0;
static uint8_t scroll_dir = 0;
static unsigned char scroll_line[CONFIG_LCD_COLS_MAX + 1U];

static uint32_t last_defuse_input_ms = 0;
static uint32_t hash_hold_ms = 0;
static uint8_t config_boot_latched = 0;
static uint8_t config_hold_armed = 0;

static char success_stars[CONFIG_PASSWORD_LEN_MAX + 1U];
static uint8_t success_show_code = 0;
static uint32_t success_next_ms = 0;

static uint8_t startup_beep_state = 0;
static uint32_t startup_beep_next_ms = 0;
static uint8_t success_beep_state = 0;
static uint32_t success_beep_next_ms = 0;

static DefuseAnim defuse_anim;
static Mp3Sequence mp3_seq = MP3_SEQ_NONE;
static uint8_t mp3_seq_step = 0;
static uint32_t mp3_seq_next_ms = 0;

static ConfigModeState cfg_mode_state = CFG_MODE_IDLE;
static AppConfig cfg_edit;
static char cfg_input[17];
static uint8_t cfg_input_len = 0;
static uint16_t cfg_selected_id = 0;

uint8_t g_password_len = CONFIG_PASSWORD_LEN;
uint8_t g_lcd_cols = CONFIG_LCD_COLS;
uint8_t g_lcd_normal_mount = CONFIG_LCD_NORMAL_MOUNT;
uint8_t g_password_col = CONFIG_PASSWORD_COL;
uint32_t g_countdown_ms = CONFIG_COUNTDOWN_MS;
uint32_t g_arm_delay_ms = CONFIG_ARM_DELAY_MS;
uint8_t g_arm_preset_enable = CONFIG_ARM_PRESET_ENABLE;
char g_arm_preset_password[CONFIG_PASSWORD_LEN_MAX + 1U] = CONFIG_ARM_PRESET_PASSWORD;

uint8_t g_defuse_enable_password = CONFIG_DEFUSE_ENABLE_PASSWORD;
uint8_t g_defuse_enable_manual = CONFIG_DEFUSE_ENABLE_MANUAL;
uint8_t g_defuse_enable_external = CONFIG_DEFUSE_ENABLE_EXTERNAL;

uint8_t g_lcd_backlight_pct = CONFIG_LCD_BACKLIGHT_PCT;

uint32_t g_beep_len_ms = CONFIG_BEEP_LEN_MS;
uint32_t g_beep_interval_base_ms = CONFIG_BEEP_INTERVAL_BASE_MS;
uint32_t g_beep_interval_scale_ms = CONFIG_BEEP_INTERVAL_SCALE_MS;
uint32_t g_beep_interval_min_ms = CONFIG_BEEP_INTERVAL_MIN_MS;
uint32_t g_beep_initial_ms = CONFIG_BEEP_INITIAL_MS;

uint16_t g_led_pwm_max = CONFIG_LED_PWM_MAX;
uint32_t g_led_breath_period_ms = CONFIG_LED_BREATH_PERIOD_MS;
uint8_t g_led_yellow_red_pct = CONFIG_LED_YELLOW_RED_PCT;

uint32_t g_scroll_interval_ms = CONFIG_SCROLL_INTERVAL_MS;
uint8_t g_scroll_pattern_len = CONFIG_SCROLL_PATTERN_LEN;
uint8_t g_digital_countdown_enable = CONFIG_DIGITAL_COUNTDOWN_ENABLE;

uint32_t g_long_press_ms = CONFIG_LONG_PRESS_MS;
uint32_t g_manual_defuse_ms = CONFIG_MANUAL_DEFUSE_MS;
uint32_t g_external_defuse_ms = CONFIG_EXTERNAL_DEFUSE_MS;
uint32_t g_defuse_display_hold_ms = CONFIG_DEFUSE_DISPLAY_HOLD_MS;
uint8_t g_defuse_cycle_steps = CONFIG_DEFUSE_CYCLE_STEPS;

uint32_t g_defuse_flash_toggle_ms = CONFIG_DEFUSE_FLASH_TOGGLE_MS;
uint8_t g_defuse_flash_toggles = CONFIG_DEFUSE_FLASH_TOGGLES;
uint32_t g_defuse_blink_ms = CONFIG_DEFUSE_BLINK_MS;

uint32_t g_buzzer_startup_freq_hz = CONFIG_BUZZER_STARTUP_FREQ_HZ;
uint32_t g_buzzer_countdown_freq_hz = CONFIG_BUZZER_COUNTDOWN_FREQ_HZ;
uint8_t g_buzzer_duty_pct = CONFIG_BUZZER_DUTY_PCT;
uint32_t g_startup_beep_on_ms = CONFIG_STARTUP_BEEP_ON_MS;
uint32_t g_startup_beep_gap_ms = CONFIG_STARTUP_BEEP_GAP_MS;
uint8_t g_startup_beep_enable = CONFIG_STARTUP_BEEP_ENABLE;

uint8_t g_mp3_volume = CONFIG_MP3_VOLUME;
uint8_t g_mp3_arm_success_enable = CONFIG_MP3_ARM_SUCCESS_ENABLE;
uint8_t g_mp3_defuse_success_enable = CONFIG_MP3_DEFUSE_SUCCESS_ENABLE;

uint16_t g_mp3_track_arm_success = CONFIG_MP3_TRACK_ARM_SUCCESS;
uint16_t g_mp3_track_explosion = CONFIG_MP3_TRACK_EXPLOSION;
uint16_t g_mp3_track_defuse_success = CONFIG_MP3_TRACK_DEFUSE_SUCCESS;
uint16_t g_mp3_track_ctwin = CONFIG_MP3_TRACK_CTWIN;
uint16_t g_mp3_track_terwin = CONFIG_MP3_TRACK_TERWIN;

uint8_t g_mp3_defuse_stage_enable = CONFIG_MP3_DEFUSE_STAGE_ENABLE;
uint8_t g_mp3_exploded_stage_enable = CONFIG_MP3_EXPLODED_STAGE_ENABLE;

uint8_t g_mp3_explosion_enable = CONFIG_MP3_EXPLOSION_ENABLE;
uint8_t g_mp3_ct_win_musicbox_enable = CONFIG_MP3_CT_WIN_MUSICBOX_ENABLE;
uint8_t g_mp3_t_win_musicbox_enable = CONFIG_MP3_T_WIN_MUSICBOX_ENABLE;
uint8_t g_mp3_ctwin_enable = CONFIG_MP3_CTWIN_ENABLE;
uint8_t g_mp3_terwin_enable = CONFIG_MP3_TERWIN_ENABLE;

uint16_t g_mp3_ct_win_musicbox_track = CONFIG_MP3_CT_WIN_MUSICBOX_TRACK;
uint16_t g_mp3_t_win_musicbox_track = CONFIG_MP3_T_WIN_MUSICBOX_TRACK;

uint32_t g_mp3_defuse_success_wait_ms = CONFIG_MP3_DEFUSE_SUCCESS_WAIT_MS;
uint32_t g_mp3_ct_musicbox_wait_ms = CONFIG_MP3_CT_MUSICBOX_WAIT_MS;
uint32_t g_mp3_t_musicbox_wait_ms = CONFIG_MP3_T_MUSICBOX_WAIT_MS;
uint32_t g_mp3_explosion_only_wait_ms = CONFIG_MP3_EXPLOSION_ONLY_WAIT_MS;

#define MP3_CT_MUSICBOX_BASE 1000U
#define MP3_T_MUSICBOX_BASE 2000U

#define CONFIG_RESET_CODE CONFIG_DEFAULT_RESET_CODE

static void Runtime_Apply(const AppConfig *cfg)
{
	if (cfg == 0)
	{
		return;
	}

	g_password_len = (uint8_t)cfg->password_len;
	g_lcd_cols = (uint8_t)cfg->lcd_cols;
	g_lcd_normal_mount = (uint8_t)cfg->lcd_normal_mount;
	g_password_col = (uint8_t)cfg->password_col;
	g_countdown_ms = cfg->countdown_ms;
	g_arm_delay_ms = cfg->arm_delay_ms;
	g_arm_preset_enable = (uint8_t)cfg->arm_preset_enable;
	strncpy(g_arm_preset_password, cfg->arm_preset_password, CONFIG_PASSWORD_LEN_MAX);
	g_arm_preset_password[CONFIG_PASSWORD_LEN_MAX] = '\0';

	g_defuse_enable_password = (uint8_t)cfg->defuse_enable_password;
	g_defuse_enable_manual = (uint8_t)cfg->defuse_enable_manual;
	g_defuse_enable_external = (uint8_t)cfg->defuse_enable_external;

	g_lcd_backlight_pct = (uint8_t)cfg->lcd_backlight_pct;

	g_beep_len_ms = cfg->beep_len_ms;
	g_beep_interval_base_ms = cfg->beep_interval_base_ms;
	g_beep_interval_scale_ms = cfg->beep_interval_scale_ms;
	g_beep_interval_min_ms = cfg->beep_interval_min_ms;
	g_beep_initial_ms = cfg->beep_initial_ms;

	g_led_pwm_max = (uint16_t)cfg->led_pwm_max;
	g_led_breath_period_ms = cfg->led_breath_period_ms;
	g_led_yellow_red_pct = (uint8_t)cfg->led_yellow_red_pct;

	g_scroll_interval_ms = cfg->scroll_interval_ms;
	g_scroll_pattern_len = (uint8_t)cfg->scroll_pattern_len;
	g_digital_countdown_enable = (uint8_t)cfg->digital_countdown_enable;

	g_long_press_ms = cfg->long_press_ms;
	g_manual_defuse_ms = cfg->manual_defuse_ms;
	g_external_defuse_ms = cfg->external_defuse_ms;
	g_defuse_display_hold_ms = cfg->defuse_display_hold_ms;
	g_defuse_cycle_steps = (uint8_t)cfg->defuse_cycle_steps;

	g_defuse_flash_toggle_ms = cfg->defuse_flash_toggle_ms;
	g_defuse_flash_toggles = (uint8_t)cfg->defuse_flash_toggles;
	g_defuse_blink_ms = cfg->defuse_blink_ms;

	g_buzzer_startup_freq_hz = cfg->buzzer_startup_freq_hz;
	g_buzzer_countdown_freq_hz = cfg->buzzer_countdown_freq_hz;
	g_buzzer_duty_pct = (uint8_t)cfg->buzzer_duty_pct;
	g_startup_beep_on_ms = cfg->startup_beep_on_ms;
	g_startup_beep_gap_ms = cfg->startup_beep_gap_ms;
	g_startup_beep_enable = (uint8_t)cfg->startup_beep_enable;

	g_mp3_volume = (uint8_t)cfg->mp3_volume;
	g_mp3_arm_success_enable = (uint8_t)cfg->mp3_arm_success_enable;
	g_mp3_defuse_success_enable = (uint8_t)cfg->mp3_defuse_success_enable;

	g_mp3_track_arm_success = (uint16_t)cfg->mp3_track_arm_success;
	g_mp3_track_explosion = (uint16_t)cfg->mp3_track_explosion;
	g_mp3_track_defuse_success = (uint16_t)cfg->mp3_track_defuse_success;
	g_mp3_track_ctwin = (uint16_t)cfg->mp3_track_ctwin;
	g_mp3_track_terwin = (uint16_t)cfg->mp3_track_terwin;

	g_mp3_defuse_stage_enable = (uint8_t)cfg->mp3_defuse_stage_enable;
	g_mp3_exploded_stage_enable = (uint8_t)cfg->mp3_exploded_stage_enable;

	g_mp3_explosion_enable = (uint8_t)cfg->mp3_explosion_enable;
	g_mp3_ct_win_musicbox_enable = (uint8_t)cfg->mp3_ct_win_musicbox_enable;
	g_mp3_t_win_musicbox_enable = (uint8_t)cfg->mp3_t_win_musicbox_enable;
	g_mp3_ctwin_enable = (uint8_t)cfg->mp3_ctwin_enable;
	g_mp3_terwin_enable = (uint8_t)cfg->mp3_terwin_enable;

	g_mp3_ct_win_musicbox_track = (uint16_t)cfg->mp3_ct_win_musicbox_track;
	g_mp3_t_win_musicbox_track = (uint16_t)cfg->mp3_t_win_musicbox_track;

	g_mp3_defuse_success_wait_ms = cfg->mp3_defuse_success_wait_ms;
	g_mp3_ct_musicbox_wait_ms = cfg->mp3_ct_musicbox_wait_ms;
	g_mp3_t_musicbox_wait_ms = cfg->mp3_t_musicbox_wait_ms;
	g_mp3_explosion_only_wait_ms = cfg->mp3_explosion_only_wait_ms;
}

static void DefuserInput_Init(void)
{
	GPIO_InitTypeDef gpio;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_3;
	gpio.GPIO_Mode = GPIO_Mode_IPU;
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
	for (i = 0; i < g_password_len; i++)
	{
		buf[i] = '*';
	}
	buf[g_password_len] = '\0';
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
		for (i = 0; i < g_password_len - 1U; i++)
		{
			buf[i] = buf[i + 1U];
		}
		buf[g_password_len - 1U] = key;
	}
	else if (key == '*')
	{
		for (i = g_password_len - 1U; i > 0U; i--)
		{
			buf[i] = buf[i - 1U];
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
		if (*pos < g_password_len)
		{
			buf[*pos] = key;
			(*pos)++;
		}
	}
	else if (key == '*')
	{
		if (*pos > 0U)
		{
			(*pos)--;
			buf[*pos] = '*';
		}
	}
	else if (key == '#')
	{
		Password_Reset(buf);
		*pos = 0U;
	}
}

static uint8_t Password_Match(const char *a, const char *b)
{
	return (memcmp(a, b, g_password_len) == 0) ? 1U : 0U;
}

static void LCD_ClearLine(void)
{
	static unsigned char spaces[] = "                ";
	LCD_BeginFrame();
	LCD_WRITE_StrDATA(spaces, 0);
}

static void LCD_WriteFixed(const char *text, uint8_t col, uint8_t width)
{
	unsigned char out[17];
	uint8_t i;
	for (i = 0; i < width && i < 16U; i++)
	{
		char ch = text[i];
		if (ch == '\0')
		{
			break;
		}
		out[i] = (unsigned char)ch;
	}
	for (; i < width && i < 16U; i++)
	{
		out[i] = ' ';
	}
	out[(width > 16U) ? 16U : width] = '\0';
	LCD_WRITE_StrDATA(out, col);
}

static void LCD_ShowPassword(const char *buf)
{
	LCD_BeginFrame();
	LCD_ClearLine();
	LCD_WRITE_StrDATA_Password((unsigned char *)buf, g_password_col, g_password_len);
}

static void LCD_ShowScrollWithTime(uint16_t pos, uint32_t now)
{
	uint16_t max_pos = (uint16_t)((g_lcd_cols - 1U) / 2U);
	uint16_t i;
	uint32_t remain_ms;
	uint32_t remain_sec;
	uint32_t mm;
	uint32_t ss;
	unsigned char right[9];

	if (pos > max_pos)
	{
		pos = max_pos;
	}

	LCD_BeginFrame();

	for (i = 0; i < g_lcd_cols; i++)
	{
		scroll_line[i] = ' ';
	}
	scroll_line[g_lcd_cols] = '\0';

	if (pos > 0U)
	{
		scroll_line[pos - 1U] = '*';
	}
	scroll_line[pos] = '*';
	if (pos < max_pos)
	{
		scroll_line[pos + 1U] = '*';
	}

	if (g_digital_countdown_enable && defuse_mode == DEFUSE_NONE)
	{
		if (now >= countdown_end_ms)
		{
			remain_ms = 0U;
		}
		else
		{
			remain_ms = countdown_end_ms - now;
		}
		remain_sec = remain_ms / 1000U;
		mm = remain_sec / 60U;
		ss = remain_sec % 60U;

		right[0] = ' ';
		right[1] = ' ';
		right[2] = (unsigned char)('0' + ((mm / 10U) % 10U));
		right[3] = (unsigned char)('0' + (mm % 10U));
		right[4] = ':';
		right[5] = (unsigned char)('0' + ((ss / 10U) % 10U));
		right[6] = (unsigned char)('0' + (ss % 10U));
		right[7] = ' ';
		right[8] = '\0';

		for (i = 0; i < 8U && i < g_lcd_cols; i++)
		{
			scroll_line[g_lcd_cols - 8U + i] = right[i];
		}
	}

	LCD_WRITE_StrDATA(scroll_line, 0);
}

static void LED_Breath(uint32_t now)
{
	uint32_t half = g_led_breath_period_ms / 2U;
	uint32_t phase = (g_led_breath_period_ms == 0U) ? 0U : (now % g_led_breath_period_ms);
	uint16_t level;
	uint16_t max_pwm = LED_GetPwmMax();

	if (half == 0U)
	{
		half = 1U;
	}

	if (phase < half)
	{
		level = (uint16_t)((phase * max_pwm) / half);
	}
	else
	{
		level = (uint16_t)(((g_led_breath_period_ms - phase) * max_pwm) / half);
	}
	LED_SetYellow(level);
}

static void StartupBeep_Update(uint32_t now)
{
	if (startup_beep_state >= 4U)
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
			startup_beep_next_ms = now + g_startup_beep_on_ms;
			startup_beep_state = 1;
			break;
		case 1:
			Buzzer_Off();
			startup_beep_next_ms = now + g_startup_beep_gap_ms;
			startup_beep_state = 2;
			break;
		case 2:
			Buzzer_On();
			startup_beep_next_ms = now + g_startup_beep_on_ms;
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

static void SuccessBeep_Start(uint32_t now)
{
	success_beep_state = 0U;
	success_beep_next_ms = now;
	Buzzer_SetFreq(g_buzzer_startup_freq_hz);
}

static void SuccessBeep_Update(uint32_t now)
{
	if (success_beep_state >= 6U)
	{
		return;
	}
	if (now < success_beep_next_ms)
	{
		return;
	}
	switch (success_beep_state)
	{
		case 0:
			Buzzer_On();
			success_beep_next_ms = now + g_startup_beep_on_ms;
			success_beep_state = 1;
			break;
		case 1:
			Buzzer_Off();
			success_beep_next_ms = now + g_startup_beep_gap_ms;
			success_beep_state = 2;
			break;
		case 2:
			Buzzer_On();
			success_beep_next_ms = now + g_startup_beep_on_ms;
			success_beep_state = 3;
			break;
		case 3:
			Buzzer_Off();
			success_beep_next_ms = now + g_startup_beep_gap_ms;
			success_beep_state = 4;
			break;
		case 4:
			Buzzer_On();
			success_beep_next_ms = now + g_startup_beep_on_ms;
			success_beep_state = 5;
			break;
		case 5:
			Buzzer_Off();
			success_beep_state = 6;
			break;
		default:
			break;
	}
}

static void MP3Sequence_Stop(void)
{
	mp3_seq = MP3_SEQ_NONE;
	mp3_seq_step = 0U;
	mp3_seq_next_ms = 0U;
}

static uint16_t MP3_TrackCtMusicbox(uint16_t track)
{
	return (uint16_t)(MP3_CT_MUSICBOX_BASE + track);
}

static uint16_t MP3_TrackTMusicbox(uint16_t track)
{
	return (uint16_t)(MP3_T_MUSICBOX_BASE + track);
}

static void MP3Sequence_Update(uint32_t now)
{
	if (mp3_seq == MP3_SEQ_NONE)
	{
		return;
	}
	if (now < mp3_seq_next_ms)
	{
		return;
	}

	if (mp3_seq == MP3_SEQ_DEFUSE)
	{
		if (mp3_seq_step == 0U)
		{
			if (!g_mp3_defuse_stage_enable)
			{
				MP3Sequence_Stop();
				return;
			}
			if (g_mp3_defuse_success_enable)
			{
				mp3_PlayMp3Track(g_mp3_track_defuse_success);
				mp3_seq_step = 1U;
				mp3_seq_next_ms = now + g_mp3_defuse_success_wait_ms;
			}
			else
			{
				mp3_seq_step = 1U;
				mp3_seq_next_ms = now;
			}
		}
		else if (mp3_seq_step == 1U)
		{
			if (g_mp3_ct_win_musicbox_enable)
			{
				mp3_PlayMp3Track(MP3_TrackCtMusicbox(g_mp3_ct_win_musicbox_track));
				if (g_mp3_ctwin_enable)
				{
					mp3_seq_step = 2U;
					mp3_seq_next_ms = now + g_mp3_ct_musicbox_wait_ms;
				}
				else
				{
					MP3Sequence_Stop();
				}
			}
			else
			{
				if (g_mp3_ctwin_enable)
				{
					mp3_PlayMp3Track(g_mp3_track_ctwin);
				}
				MP3Sequence_Stop();
			}
		}
		else
		{
			if (g_mp3_ctwin_enable)
			{
				mp3_PlayMp3Track(g_mp3_track_ctwin);
			}
			MP3Sequence_Stop();
		}
	}
	else
	{
		if (mp3_seq_step == 0U)
		{
			if (!g_mp3_exploded_stage_enable)
			{
				MP3Sequence_Stop();
				return;
			}
			if (!g_mp3_explosion_enable)
			{
				if (g_mp3_terwin_enable)
				{
					mp3_PlayMp3Track(g_mp3_track_terwin);
				}
				MP3Sequence_Stop();
				return;
			}

			if (g_mp3_t_win_musicbox_enable)
			{
				mp3_PlayMp3Track(MP3_TrackTMusicbox(g_mp3_t_win_musicbox_track));
				if (g_mp3_terwin_enable)
				{
					mp3_seq_step = 1U;
					mp3_seq_next_ms = now + g_mp3_t_musicbox_wait_ms;
				}
				else
				{
					MP3Sequence_Stop();
				}
			}
			else
			{
				mp3_PlayMp3Track(g_mp3_track_explosion);
				if (g_mp3_terwin_enable)
				{
					mp3_seq_step = 1U;
					mp3_seq_next_ms = now + g_mp3_explosion_only_wait_ms;
				}
				else
				{
					MP3Sequence_Stop();
				}
			}
		}
		else
		{
			if (g_mp3_terwin_enable)
			{
				mp3_PlayMp3Track(g_mp3_track_terwin);
			}
			MP3Sequence_Stop();
		}
	}
}

static void MP3Sequence_StartDefuse(uint32_t now)
{
	mp3_seq = MP3_SEQ_DEFUSE;
	mp3_seq_step = 0U;
	mp3_seq_next_ms = now;
	MP3Sequence_Update(now);
}

static void MP3Sequence_StartExplosion(uint32_t now)
{
	mp3_seq = MP3_SEQ_EXPLOSION;
	mp3_seq_step = 0U;
	mp3_seq_next_ms = now;
	MP3Sequence_Update(now);
}

static void Countdown_Start(uint32_t now)
{
	countdown_end_ms = now + g_countdown_ms;
	next_beep_ms = now + g_beep_initial_ms;
	beep_off_ms = 0U;
	beep_active = 0U;
	hash_hold_ms = 0U;
	scroll_pos = 0U;
	scroll_dir = 0U;
	scroll_next_ms = now;
	defuse_mode = DEFUSE_NONE;
	Password_Reset(defuse_input);
	defuse_pos = 0U;
	last_defuse_input_ms = 0U;
	MP3Sequence_Stop();
	app_state = STATE_COUNTDOWN;
	LCD_ShowScrollWithTime(scroll_pos, now);
	Buzzer_SetFreq(g_buzzer_countdown_freq_hz);
	if (g_mp3_arm_success_enable)
	{
		mp3_PlayMp3Track(g_mp3_track_arm_success);
	}
}

static void Countdown_UpdateBeep(uint32_t now)
{
	if (beep_active && now >= beep_off_ms)
	{
		beep_active = 0U;
		Buzzer_Off();
	}
	if (now >= next_beep_ms)
	{
		float elapsed_ratio;
		float bps;
		float interval_ms;

		if (now >= countdown_end_ms)
		{
			elapsed_ratio = 1.0f;
		}
		else
		{
			elapsed_ratio = ((float)(g_countdown_ms - (countdown_end_ms - now))) / (float)g_countdown_ms;
		}

		bps = 1.049f * (float)exp((0.244f * elapsed_ratio) + (1.764f * elapsed_ratio * elapsed_ratio));
		if (bps < 0.001f)
		{
			bps = 0.001f;
		}
		interval_ms = 1000.0f / bps;
		next_beep_ms = now + (uint32_t)(interval_ms);
		beep_active = 1U;
		beep_off_ms = now + g_beep_len_ms;
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
	scroll_next_ms = now + g_scroll_interval_ms;
	max_pos = (uint16_t)((g_lcd_cols - 1U) / 2U);
	if (scroll_dir == 0U)
	{
		if (scroll_pos < max_pos)
		{
			scroll_pos++;
		}
		else
		{
			scroll_dir = 1U;
		}
	}
	else
	{
		if (scroll_pos > 0U)
		{
			scroll_pos--;
		}
		else
		{
			scroll_dir = 0U;
		}
	}
	LCD_ShowScrollWithTime(scroll_pos, now);
}

static void DefuseAnim_Reset(void)
{
	defuse_anim.active = 0U;
	defuse_anim.mode = DEFUSE_NONE;
	defuse_anim.step_ms = 0U;
	defuse_anim.next_step_ms = 0U;
	defuse_anim.digit_index = 0U;
	defuse_anim.cycle_step = 0U;
	Password_Reset(defuse_anim.display);
}

static void DefuseAnim_Start(uint32_t now, DefuseMode mode)
{
	uint32_t duration = (mode == DEFUSE_EXTERNAL) ? g_external_defuse_ms : g_manual_defuse_ms;
	uint32_t steps = g_password_len * ((uint32_t)g_defuse_cycle_steps + 1U);
	uint32_t step_ms = (steps == 0U) ? duration : (duration / steps);

	DefuseAnim_Reset();
	defuse_anim.active = 1U;
	defuse_anim.mode = mode;
	defuse_anim.step_ms = (step_ms == 0U) ? 1U : step_ms;
	defuse_anim.next_step_ms = now;
	defuse_anim.digit_index = 0U;
	defuse_anim.cycle_step = 0U;
	Password_Reset(defuse_anim.display);
	defuse_mode = mode;
	LCD_ShowPassword(defuse_anim.display);
}

static uint8_t DefuseAnim_Update(uint32_t now, uint8_t defuser_active, char hold)
{
	if (!defuse_anim.active)
	{
		return 0U;
	}
	if (defuse_anim.mode == DEFUSE_EXTERNAL && !defuser_active)
	{
		DefuseAnim_Reset();
		defuse_mode = DEFUSE_NONE;
		LCD_ShowScrollWithTime(scroll_pos, now);
		return 0U;
	}
	if (defuse_anim.mode == DEFUSE_MANUAL && hold != '#')
	{
		DefuseAnim_Reset();
		defuse_mode = DEFUSE_NONE;
		LCD_ShowScrollWithTime(scroll_pos, now);
		return 0U;
	}
	if (now < defuse_anim.next_step_ms)
	{
		return 0U;
	}
	defuse_anim.next_step_ms = now + defuse_anim.step_ms;
	if (defuse_anim.digit_index >= g_password_len)
	{
		DefuseAnim_Reset();
		return 1U;
	}
	if (defuse_anim.cycle_step < g_defuse_cycle_steps)
	{
		defuse_anim.display[defuse_anim.digit_index] = (char)('0' + defuse_anim.cycle_step);
		defuse_anim.cycle_step++;
	}
	else
	{
		defuse_anim.display[defuse_anim.digit_index] = arm_code[defuse_anim.digit_index];
		defuse_anim.digit_index++;
		defuse_anim.cycle_step = 0U;
	}
	LCD_ShowPassword(defuse_anim.display);
	return 0U;
}

static void Defuse_Success(uint32_t now)
{
	uint16_t max_pwm = LED_GetPwmMax();
	Buzzer_Off();
	Relay_Off();
	beep_active = 0U;
	DefuseAnim_Reset();
	defuse_mode = DEFUSE_NONE;

	Password_Reset(success_stars);
	success_show_code = 0U;
	success_next_ms = now + g_defuse_blink_ms;
	LCD_Backlight_On();
	LCD_ShowPassword(success_stars);

	LED_SetGreen(max_pwm);
	SuccessBeep_Start(now);
	MP3Sequence_StartDefuse(now);
	app_state = STATE_DEFUSE_SUCCESS;
}

static void Explosion_Start(uint32_t now)
{
	uint16_t max_pwm = LED_GetPwmMax();
	Buzzer_Off();
	beep_active = 0U;
	DefuseAnim_Reset();
	defuse_mode = DEFUSE_NONE;
	Relay_On();
	LED_SetYellow(max_pwm);
	LCD_ClearLine();
	MP3Sequence_StartExplosion(now);
	app_state = STATE_EXPLODED;
}

static uint8_t LocalParseUint(const char *text, uint32_t *value)
{
	uint32_t out = 0U;
	uint8_t has_digit = 0U;
	if (text == 0 || value == 0)
	{
		return 0U;
	}
	while (*text != '\0')
	{
		if (*text < '0' || *text > '9')
		{
			return 0U;
		}
		has_digit = 1U;
		out = out * 10U + (uint32_t)(*text - '0');
		text++;
	}
	if (!has_digit)
	{
		return 0U;
	}
	*value = out;
	return 1U;
}

static void CfgInput_Clear(void)
{
	cfg_input_len = 0U;
	cfg_input[0] = '\0';
}

static void CfgInput_Push(char key)
{
	if (key >= '0' && key <= '9')
	{
		if (cfg_input_len < sizeof(cfg_input) - 1U)
		{
			cfg_input[cfg_input_len++] = key;
			cfg_input[cfg_input_len] = '\0';
		}
	}
	else if (key == '*' && cfg_input_len > 0U)
	{
		cfg_input_len--;
		cfg_input[cfg_input_len] = '\0';
	}
	else if (key == '#')
	{
		CfgInput_Clear();
	}
}

static void CfgShowPromptId(void)
{
	LCD_BeginFrame();
	LCD_ClearLine();
	LCD_WriteFixed("ID:", 0, 3);
	LCD_WriteFixed(cfg_input, 4, 8);
}

static void CfgShowPromptValue(void)
{
	LCD_BeginFrame();
	LCD_ClearLine();
	LCD_WriteFixed("VAL:", 0, 4);
	LCD_WriteFixed(cfg_input, 4, 12);
}

static void CfgShowShort(const char *left, const char *right)
{
	LCD_BeginFrame();
	LCD_ClearLine();
	LCD_WriteFixed(left, 0, 8);
	LCD_WriteFixed(right, 8, 8);
}

static void ConfigMode_Init(void)
{
	ConfigManager_CopyCurrent(&cfg_edit);
	cfg_mode_state = CFG_MODE_INPUT_ID;
	cfg_selected_id = 0U;
	CfgInput_Clear();
	CfgShowPromptId();
}

static uint8_t ConfigMode_Update(char key)
{
	uint32_t id_num;
	char buf[17];

	switch (cfg_mode_state)
	{
		case CFG_MODE_INPUT_ID:
			if (key != ' ')
			{
				if (key == '#')
				{
					if (strcmp(cfg_input, CONFIG_RESET_CODE) == 0)
					{
						cfg_mode_state = CFG_MODE_CONFIRM_RESTORE;
						CfgShowShort("RESTORE?", "#=Y *=N");
					}
					else if (LocalParseUint(cfg_input, &id_num))
					{
						if (id_num < ConfigManager_GetCount())
						{
							cfg_selected_id = (uint16_t)id_num;
							ConfigManager_CopyCurrent(&cfg_edit);
							CfgInput_Clear();
							cfg_mode_state = CFG_MODE_INPUT_VALUE;
							CfgShowPromptValue();
						}
						else
						{
							CfgShowShort("ID ERR", "RETRY");
							CfgInput_Clear();
						}
					}
					return 0U;
				}
				CfgInput_Push(key);
				CfgShowPromptId();
			}
			break;
		case CFG_MODE_CONFIRM_RESTORE:
			if (key == '#')
			{
				if (ConfigManager_RestoreDefaults())
				{
					CfgShowShort("RESTORED", "RESET");
					return 1U;
				}
				else
				{
					CfgShowShort("RESTFAIL", "RETRY");
					cfg_mode_state = CFG_MODE_INPUT_ID;
					CfgInput_Clear();
				}
			}
			if (key == '*')
			{
				cfg_mode_state = CFG_MODE_INPUT_ID;
				CfgInput_Clear();
				CfgShowPromptId();
			}
			break;
		case CFG_MODE_INPUT_VALUE:
			if (key != ' ')
			{
				if (key == '#')
				{
					if (!ConfigManager_SetValueString(&cfg_edit, cfg_selected_id, cfg_input))
					{
						CfgShowShort("VAL ERR", "RETRY");
						CfgInput_Clear();
						return 0U;
					}
					if (!ConfigManager_Validate(&cfg_edit))
					{
						CfgShowShort("RANGEERR", "RETRY");
						CfgInput_Clear();
						return 0U;
					}
					cfg_mode_state = CFG_MODE_CONFIRM_SAVE;
					if (ConfigManager_GetValueString(&cfg_edit, cfg_selected_id, buf, sizeof(buf)))
					{
						LCD_ClearLine();
						LCD_WriteFixed("SAVE?", 0, 5);
						LCD_WriteFixed(buf, 6, 6);
					}
					else
					{
						CfgShowShort("SAVE?", "#=Y *=N");
					}
					return 0U;
				}
				CfgInput_Push(key);
				CfgShowPromptValue();
			}
			break;
		case CFG_MODE_CONFIRM_SAVE:
			if (key == '#')
			{
				if (ConfigManager_Save(&cfg_edit))
				{
					CfgShowShort("STORED", "RESET");
					return 1U;
				}
				else
				{
					CfgShowShort("SAVEFAIL", "RETRY");
					cfg_mode_state = CFG_MODE_INPUT_ID;
					CfgInput_Clear();
				}
			}
			if (key == '*')
			{
				cfg_mode_state = CFG_MODE_INPUT_ID;
				CfgInput_Clear();
				CfgShowPromptId();
			}
			break;
		default:
			break;
	}

	return 0U;
}

int main(void)
{
	uint32_t now;
	uint32_t init_now;
	uint32_t config_hold_start_ms;
	uint32_t config_boot_latch_deadline_ms;
	char key;
	char hold;
	uint8_t defuser_active;

	Timebase_Init();
	DefuserInput_Init();
	ConfigManager_Init();
	Runtime_Apply(ConfigManager_Get());

	LED_Init();
	Buzzer_Init();
	Buzzer_SetFreq(g_buzzer_startup_freq_hz);
	Buzzer_SetDuty(g_buzzer_duty_pct);
	LCD_INIT();
	MatrixKey_Init();
	mp3_Init();
	Delay_ms(200);
	mp3_SetVolume(g_mp3_volume);

	init_now = Timebase_Millis();
	startup_beep_next_ms = init_now;
	startup_beep_state = g_startup_beep_enable ? 0U : 4U;

	Password_Reset(arm_input);
	Password_Reset(arm_code);
	Password_Reset(defuse_input);
	LCD_ShowPassword(arm_input);
	config_hold_start_ms = 0U;
	config_boot_latched = 0U;
	config_hold_armed = 0U;
	config_boot_latch_deadline_ms = init_now + 120U;

	while (1)
	{
		now = Timebase_Millis();
		MatrixKey_Update();
		key = MatrixKey_GetValue();
		hold = MatrixKey_GetHold();
		defuser_active = Defuser_IsActive();

		if (!config_boot_latched)
		{
			if (hold == '#')
			{
				config_boot_latched = 1U;
				config_hold_armed = 1U;
				config_hold_start_ms = now;
			}
			else if (hold != ' ')
			{
				config_boot_latched = 1U;
				config_hold_armed = 0U;
			}
			else if (now >= config_boot_latch_deadline_ms)
			{
				config_boot_latched = 1U;
				config_hold_armed = 0U;
			}
		}

		if (app_state == STATE_IDLE && config_hold_armed)
		{
			if (hold == '#')
			{
				if ((now - config_hold_start_ms) >= 300U)
				{
					Buzzer_Off();
					ConfigMode_Init();
					while (1)
					{
						uint32_t cfg_now = Timebase_Millis();
						StartupBeep_Update(cfg_now);
						LED_Breath(cfg_now);
						MatrixKey_Update();
						key = MatrixKey_GetValue();
						if (ConfigMode_Update(key))
						{
							while (1)
							{
								uint32_t wait_now = Timebase_Millis();
								StartupBeep_Update(wait_now);
								LED_Breath(wait_now);
							}
						}
					}
				}
			}
			else
			{
				config_hold_armed = 0U;
				config_hold_start_ms = 0U;
			}
		}

		StartupBeep_Update(now);
		MP3Sequence_Update(now);

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
				if (Password_IsComplete(arm_input) && (now - arm_last_change_ms) >= g_arm_delay_ms)
				{
					if (g_arm_preset_enable)
					{
						if (Password_Match(arm_input, g_arm_preset_password))
						{
							memcpy(arm_code, g_arm_preset_password, g_password_len + 1U);
							Countdown_Start(now);
						}
						else
						{
							Password_Reset(arm_input);
							LCD_ShowPassword(arm_input);
							arm_last_change_ms = now;
						}
					}
					else
					{
						memcpy(arm_code, arm_input, g_password_len + 1U);
						Countdown_Start(now);
					}
				}
				break;
			case STATE_COUNTDOWN:
				if (g_defuse_enable_manual)
				{
					if (defuse_mode == DEFUSE_NONE && hold == '#')
					{
						if (hash_hold_ms == 0U)
						{
							hash_hold_ms = now;
						}
						else if ((now - hash_hold_ms) >= g_long_press_ms)
						{
							DefuseAnim_Start(now, DEFUSE_MANUAL);
							hash_hold_ms = 0U;
						}
					}
					else
					{
						hash_hold_ms = 0U;
					}
				}
				else
				{
					hash_hold_ms = 0U;
				}

				if (g_defuse_enable_external)
				{
					if (defuse_mode == DEFUSE_NONE && defuser_active)
					{
						DefuseAnim_Start(now, DEFUSE_EXTERNAL);
					}
				}

				if (DefuseAnim_Update(now, defuser_active, hold))
				{
					Defuse_Success(now);
					break;
				}

				if (g_defuse_enable_password && defuse_mode == DEFUSE_NONE)
				{
					if (key != ' ')
					{
						Password_InputLeft(defuse_input, &defuse_pos, key);
						LCD_ShowPassword(defuse_input);
						last_defuse_input_ms = now;
					}

					if (defuse_pos >= g_password_len)
					{
						if (Password_Match(arm_code, defuse_input))
						{
							Defuse_Success(now);
							break;
						}
						Password_Reset(defuse_input);
						defuse_pos = 0U;
						LCD_ShowPassword(defuse_input);
					}
					if (defuse_pos == 0U && (now - last_defuse_input_ms) > g_defuse_display_hold_ms)
					{
						Countdown_UpdateScroll(now);
					}
				}
				else if (defuse_mode == DEFUSE_NONE)
				{
					if ((now - last_defuse_input_ms) > g_defuse_display_hold_ms)
					{
						Countdown_UpdateScroll(now);
					}
				}

				Countdown_UpdateBeep(now);
				if (beep_active)
				{
					LED_SetColor(LED_GetPwmMax(), 0U);
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
				LED_SetGreen(LED_GetPwmMax());
				SuccessBeep_Update(now);
				if (now >= success_next_ms)
				{
					success_show_code = (uint8_t)(!success_show_code);
					success_next_ms = now + g_defuse_blink_ms;
					if (success_show_code)
					{
						LCD_ShowPassword(arm_code);
					}
					else
					{
						LCD_ShowPassword(success_stars);
					}
				}
				break;
			case STATE_EXPLODED:
				LED_SetYellow(LED_GetPwmMax());
				break;
			default:
				LED_Breath(now);
				break;
		}
	}
}

