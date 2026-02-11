#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <stdint.h>

#define CONFIG_PASSWORD_LEN_MAX 16U
#define CONFIG_LCD_COLS_MAX 16U

typedef struct
{
	uint32_t ignore_range;

	uint32_t password_len;
	uint32_t lcd_cols;
	uint32_t lcd_normal_mount;
	uint32_t password_col;
	uint32_t countdown_ms;
	uint32_t arm_delay_ms;

	uint32_t arm_preset_enable;
	char arm_preset_password[CONFIG_PASSWORD_LEN_MAX + 1U];

	uint32_t defuse_enable_password;
	uint32_t defuse_enable_manual;
	uint32_t defuse_enable_external;

	uint32_t lcd_backlight_pct;

	uint32_t beep_len_ms;
	uint32_t beep_interval_base_ms;
	uint32_t beep_interval_scale_ms;
	uint32_t beep_interval_min_ms;
	uint32_t beep_initial_ms;

	uint32_t led_pwm_max;
	uint32_t led_breath_period_ms;
	uint32_t led_yellow_red_pct;

	uint32_t scroll_interval_ms;
	uint32_t scroll_pattern_len;
	uint32_t digital_countdown_enable;

	uint32_t long_press_ms;
	uint32_t manual_defuse_ms;
	uint32_t external_defuse_ms;
	uint32_t defuse_display_hold_ms;
	uint32_t defuse_cycle_steps;

	uint32_t defuse_flash_toggle_ms;
	uint32_t defuse_flash_toggles;
	uint32_t defuse_blink_ms;

	uint32_t buzzer_startup_freq_hz;
	uint32_t buzzer_countdown_freq_hz;
	uint32_t buzzer_duty_pct;
	uint32_t startup_beep_on_ms;
	uint32_t startup_beep_gap_ms;
	uint32_t startup_beep_enable;

	uint32_t mp3_volume;
	uint32_t mp3_arm_success_enable;
	uint32_t mp3_defuse_success_enable;

	uint32_t mp3_track_arm_success;
	uint32_t mp3_track_explosion;
	uint32_t mp3_track_defuse_success;
	uint32_t mp3_track_ctwin;
	uint32_t mp3_track_terwin;

	uint32_t mp3_defuse_stage_enable;
	uint32_t mp3_exploded_stage_enable;

	uint32_t mp3_explosion_enable;
	uint32_t mp3_ct_win_musicbox_enable;
	uint32_t mp3_t_win_musicbox_enable;
	uint32_t mp3_ctwin_enable;
	uint32_t mp3_terwin_enable;

	uint32_t mp3_ct_win_musicbox_track;
	uint32_t mp3_t_win_musicbox_track;

	uint32_t mp3_defuse_success_wait_ms;
	uint32_t mp3_ct_musicbox_wait_ms;
	uint32_t mp3_t_musicbox_wait_ms;
	uint32_t mp3_explosion_only_wait_ms;
	uint32_t explosion_beep_ms;
} AppConfig;

typedef enum
{
	CONFIG_ID_IGNORE_RANGE = 0,
	CONFIG_ID_PASSWORD_LEN,
	CONFIG_ID_LCD_COLS,
	CONFIG_ID_LCD_NORMAL_MOUNT,
	CONFIG_ID_PASSWORD_COL,
	CONFIG_ID_COUNTDOWN_MS,
	CONFIG_ID_ARM_DELAY_MS,
	CONFIG_ID_ARM_PRESET_ENABLE,
	CONFIG_ID_ARM_PRESET_PASSWORD,
	CONFIG_ID_DEFUSE_ENABLE_PASSWORD,
	CONFIG_ID_DEFUSE_ENABLE_MANUAL,
	CONFIG_ID_DEFUSE_ENABLE_EXTERNAL,
	CONFIG_ID_LCD_BACKLIGHT_PCT,
	CONFIG_ID_BEEP_LEN_MS,
	CONFIG_ID_BEEP_INTERVAL_BASE_MS,
	CONFIG_ID_BEEP_INTERVAL_SCALE_MS,
	CONFIG_ID_BEEP_INTERVAL_MIN_MS,
	CONFIG_ID_BEEP_INITIAL_MS,
	CONFIG_ID_LED_PWM_MAX,
	CONFIG_ID_LED_BREATH_PERIOD_MS,
	CONFIG_ID_LED_YELLOW_RED_PCT,
	CONFIG_ID_SCROLL_INTERVAL_MS,
	CONFIG_ID_SCROLL_PATTERN_LEN,
	CONFIG_ID_DIGITAL_COUNTDOWN_ENABLE,
	CONFIG_ID_LONG_PRESS_MS,
	CONFIG_ID_MANUAL_DEFUSE_MS,
	CONFIG_ID_EXTERNAL_DEFUSE_MS,
	CONFIG_ID_DEFUSE_DISPLAY_HOLD_MS,
	CONFIG_ID_DEFUSE_CYCLE_STEPS,
	CONFIG_ID_DEFUSE_FLASH_TOGGLE_MS,
	CONFIG_ID_DEFUSE_FLASH_TOGGLES,
	CONFIG_ID_DEFUSE_BLINK_MS,
	CONFIG_ID_BUZZER_STARTUP_FREQ_HZ,
	CONFIG_ID_BUZZER_COUNTDOWN_FREQ_HZ,
	CONFIG_ID_BUZZER_DUTY_PCT,
	CONFIG_ID_STARTUP_BEEP_ON_MS,
	CONFIG_ID_STARTUP_BEEP_GAP_MS,
	CONFIG_ID_STARTUP_BEEP_ENABLE,
	CONFIG_ID_MP3_VOLUME,
	CONFIG_ID_MP3_ARM_SUCCESS_ENABLE,
	CONFIG_ID_MP3_DEFUSE_SUCCESS_ENABLE,
	CONFIG_ID_MP3_TRACK_ARM_SUCCESS,
	CONFIG_ID_MP3_TRACK_EXPLOSION,
	CONFIG_ID_MP3_TRACK_DEFUSE_SUCCESS,
	CONFIG_ID_MP3_TRACK_CTWIN,
	CONFIG_ID_MP3_TRACK_TERWIN,
	CONFIG_ID_MP3_DEFUSE_STAGE_ENABLE,
	CONFIG_ID_MP3_EXPLODED_STAGE_ENABLE,
	CONFIG_ID_MP3_EXPLOSION_ENABLE,
	CONFIG_ID_MP3_CT_WIN_MUSICBOX_ENABLE,
	CONFIG_ID_MP3_T_WIN_MUSICBOX_ENABLE,
	CONFIG_ID_MP3_CTWIN_ENABLE,
	CONFIG_ID_MP3_TERWIN_ENABLE,
	CONFIG_ID_MP3_CT_WIN_MUSICBOX_TRACK,
	CONFIG_ID_MP3_T_WIN_MUSICBOX_TRACK,
	CONFIG_ID_MP3_DEFUSE_SUCCESS_WAIT_MS,
	CONFIG_ID_MP3_CT_MUSICBOX_WAIT_MS,
	CONFIG_ID_MP3_T_MUSICBOX_WAIT_MS,
	CONFIG_ID_MP3_EXPLOSION_ONLY_WAIT_MS,
	CONFIG_ID_EXPLOSION_BEEP_MS,
	CONFIG_ID_COUNT
} ConfigId;

void ConfigManager_Init(void);
const AppConfig *ConfigManager_Get(void);

void ConfigManager_LoadDefaults(AppConfig *cfg);
void ConfigManager_CopyCurrent(AppConfig *cfg);
void ConfigManager_Commit(const AppConfig *cfg);

uint8_t ConfigManager_Validate(const AppConfig *cfg);
uint8_t ConfigManager_Save(const AppConfig *cfg);
uint8_t ConfigManager_SaveCurrent(void);
uint8_t ConfigManager_RestoreDefaults(void);

uint16_t ConfigManager_GetCount(void);
const char *ConfigManager_GetName(uint16_t id);
uint8_t ConfigManager_GetValueString(const AppConfig *cfg, uint16_t id, char *out, uint8_t out_len);
uint8_t ConfigManager_SetValueString(AppConfig *cfg, uint16_t id, const char *value);

#endif
