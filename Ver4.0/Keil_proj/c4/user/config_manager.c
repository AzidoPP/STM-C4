#include "config_manager.h"
#include "config.h"
#include "stm32f10x_flash.h"
#include <stddef.h>
#include <string.h>

#define CONFIG_STORE_MAGIC 0x34474643U
#define CONFIG_STORE_VERSION 2U
#define CONFIG_FLASH_PAGE_ADDR 0x0800FC00U

#define CFG_TYPE_U32 0U
#define CFG_TYPE_STR 1U

typedef struct
{
	const char *name;
	uint8_t type;
	uint16_t offset;
	uint16_t size;
	uint32_t min;
	uint32_t max;
} ConfigMeta;

typedef struct
{
	uint32_t magic;
	uint32_t version;
	uint32_t config_count;
	uint32_t payload_size;
	uint32_t defaults_checksum;
	uint32_t checksum;
	AppConfig payload;
} ConfigStoreImage;

#define CFG_OFF(member) ((uint16_t)offsetof(AppConfig, member))

static const ConfigMeta g_config_meta[CONFIG_ID_COUNT] = {
	{"CONFIG_IGNORE_RANGE", CFG_TYPE_U32, CFG_OFF(ignore_range), 4U, 0U, 1U},
	{"CONFIG_PASSWORD_LEN", CFG_TYPE_U32, CFG_OFF(password_len), 4U, 1U, 16U},
	{"CONFIG_LCD_COLS", CFG_TYPE_U32, CFG_OFF(lcd_cols), 4U, 8U, 16U},
	{"CONFIG_LCD_NORMAL_MOUNT", CFG_TYPE_U32, CFG_OFF(lcd_normal_mount), 4U, 0U, 1U},
	{"CONFIG_PASSWORD_COL", CFG_TYPE_U32, CFG_OFF(password_col), 4U, 0U, 15U},
	{"CONFIG_COUNTDOWN_MS", CFG_TYPE_U32, CFG_OFF(countdown_ms), 4U, 1000U, 0xFFFFFFFFU},
	{"CONFIG_ARM_DELAY_MS", CFG_TYPE_U32, CFG_OFF(arm_delay_ms), 4U, 0U, 5000U},
	{"CONFIG_ARM_PRESET_ENABLE", CFG_TYPE_U32, CFG_OFF(arm_preset_enable), 4U, 0U, 1U},
	{"CONFIG_ARM_PRESET_PASSWORD", CFG_TYPE_STR, CFG_OFF(arm_preset_password), CONFIG_PASSWORD_LEN_MAX + 1U, 0U, 0U},
	{"CONFIG_DEFUSE_ENABLE_PASSWORD", CFG_TYPE_U32, CFG_OFF(defuse_enable_password), 4U, 0U, 1U},
	{"CONFIG_DEFUSE_ENABLE_MANUAL", CFG_TYPE_U32, CFG_OFF(defuse_enable_manual), 4U, 0U, 1U},
	{"CONFIG_DEFUSE_ENABLE_EXTERNAL", CFG_TYPE_U32, CFG_OFF(defuse_enable_external), 4U, 0U, 1U},
	{"CONFIG_LCD_BACKLIGHT_PCT", CFG_TYPE_U32, CFG_OFF(lcd_backlight_pct), 4U, 0U, 100U},
	{"CONFIG_BEEP_LEN_MS", CFG_TYPE_U32, CFG_OFF(beep_len_ms), 4U, 10U, 500U},
	{"CONFIG_BEEP_INTERVAL_BASE_MS", CFG_TYPE_U32, CFG_OFF(beep_interval_base_ms), 4U, 10U, 1000U},
	{"CONFIG_BEEP_INTERVAL_SCALE_MS", CFG_TYPE_U32, CFG_OFF(beep_interval_scale_ms), 4U, 0U, 2000U},
	{"CONFIG_BEEP_INTERVAL_MIN_MS", CFG_TYPE_U32, CFG_OFF(beep_interval_min_ms), 4U, 50U, 1000U},
	{"CONFIG_BEEP_INITIAL_MS", CFG_TYPE_U32, CFG_OFF(beep_initial_ms), 4U, 0U, 5000U},
	{"CONFIG_LED_PWM_MAX", CFG_TYPE_U32, CFG_OFF(led_pwm_max), 4U, 100U, 10000U},
	{"CONFIG_LED_BREATH_PERIOD_MS", CFG_TYPE_U32, CFG_OFF(led_breath_period_ms), 4U, 100U, 5000U},
	{"CONFIG_LED_YELLOW_RED_PCT", CFG_TYPE_U32, CFG_OFF(led_yellow_red_pct), 4U, 0U, 100U},
	{"CONFIG_SCROLL_INTERVAL_MS", CFG_TYPE_U32, CFG_OFF(scroll_interval_ms), 4U, 10U, 500U},
	{"CONFIG_SCROLL_PATTERN_LEN", CFG_TYPE_U32, CFG_OFF(scroll_pattern_len), 4U, 1U, 16U},
	{"CONFIG_DIGITAL_COUNTDOWN_ENABLE", CFG_TYPE_U32, CFG_OFF(digital_countdown_enable), 4U, 0U, 1U},
	{"CONFIG_LONG_PRESS_MS", CFG_TYPE_U32, CFG_OFF(long_press_ms), 4U, 200U, 3000U},
	{"CONFIG_MANUAL_DEFUSE_MS", CFG_TYPE_U32, CFG_OFF(manual_defuse_ms), 4U, 1000U, 0xFFFFFFFFU},
	{"CONFIG_EXTERNAL_DEFUSE_MS", CFG_TYPE_U32, CFG_OFF(external_defuse_ms), 4U, 1000U, 0xFFFFFFFFU},
	{"CONFIG_DEFUSE_DISPLAY_HOLD_MS", CFG_TYPE_U32, CFG_OFF(defuse_display_hold_ms), 4U, 200U, 5000U},
	{"CONFIG_DEFUSE_CYCLE_STEPS", CFG_TYPE_U32, CFG_OFF(defuse_cycle_steps), 4U, 1U, 20U},
	{"CONFIG_DEFUSE_FLASH_TOGGLE_MS", CFG_TYPE_U32, CFG_OFF(defuse_flash_toggle_ms), 4U, 10U, 200U},
	{"CONFIG_DEFUSE_FLASH_TOGGLES", CFG_TYPE_U32, CFG_OFF(defuse_flash_toggles), 4U, 1U, 20U},
	{"CONFIG_DEFUSE_BLINK_MS", CFG_TYPE_U32, CFG_OFF(defuse_blink_ms), 4U, 100U, 1000U},
	{"CONFIG_BUZZER_STARTUP_FREQ_HZ", CFG_TYPE_U32, CFG_OFF(buzzer_startup_freq_hz), 4U, 100U, 10000U},
	{"CONFIG_BUZZER_COUNTDOWN_FREQ_HZ", CFG_TYPE_U32, CFG_OFF(buzzer_countdown_freq_hz), 4U, 100U, 10000U},
	{"CONFIG_BUZZER_DUTY_PCT", CFG_TYPE_U32, CFG_OFF(buzzer_duty_pct), 4U, 0U, 50U},
	{"CONFIG_STARTUP_BEEP_ON_MS", CFG_TYPE_U32, CFG_OFF(startup_beep_on_ms), 4U, 10U, 500U},
	{"CONFIG_STARTUP_BEEP_GAP_MS", CFG_TYPE_U32, CFG_OFF(startup_beep_gap_ms), 4U, 10U, 500U},
	{"CONFIG_STARTUP_BEEP_ENABLE", CFG_TYPE_U32, CFG_OFF(startup_beep_enable), 4U, 0U, 1U},
	{"CONFIG_MP3_VOLUME", CFG_TYPE_U32, CFG_OFF(mp3_volume), 4U, 0U, 30U},
	{"CONFIG_MP3_ARM_SUCCESS_ENABLE", CFG_TYPE_U32, CFG_OFF(mp3_arm_success_enable), 4U, 0U, 1U},
	{"CONFIG_MP3_DEFUSE_SUCCESS_ENABLE", CFG_TYPE_U32, CFG_OFF(mp3_defuse_success_enable), 4U, 0U, 1U},
	{"CONFIG_MP3_TRACK_ARM_SUCCESS", CFG_TYPE_U32, CFG_OFF(mp3_track_arm_success), 4U, 1U, 999U},
	{"CONFIG_MP3_TRACK_EXPLOSION", CFG_TYPE_U32, CFG_OFF(mp3_track_explosion), 4U, 1U, 999U},
	{"CONFIG_MP3_TRACK_DEFUSE_SUCCESS", CFG_TYPE_U32, CFG_OFF(mp3_track_defuse_success), 4U, 1U, 999U},
	{"CONFIG_MP3_TRACK_CTWIN", CFG_TYPE_U32, CFG_OFF(mp3_track_ctwin), 4U, 1U, 999U},
	{"CONFIG_MP3_TRACK_TERWIN", CFG_TYPE_U32, CFG_OFF(mp3_track_terwin), 4U, 1U, 999U},
	{"CONFIG_MP3_DEFUSE_STAGE_ENABLE", CFG_TYPE_U32, CFG_OFF(mp3_defuse_stage_enable), 4U, 0U, 1U},
	{"CONFIG_MP3_EXPLODED_STAGE_ENABLE", CFG_TYPE_U32, CFG_OFF(mp3_exploded_stage_enable), 4U, 0U, 1U},
	{"CONFIG_MP3_EXPLOSION_ENABLE", CFG_TYPE_U32, CFG_OFF(mp3_explosion_enable), 4U, 0U, 1U},
	{"CONFIG_MP3_CT_WIN_MUSICBOX_ENABLE", CFG_TYPE_U32, CFG_OFF(mp3_ct_win_musicbox_enable), 4U, 0U, 1U},
	{"CONFIG_MP3_T_WIN_MUSICBOX_ENABLE", CFG_TYPE_U32, CFG_OFF(mp3_t_win_musicbox_enable), 4U, 0U, 1U},
	{"CONFIG_MP3_CTWIN_ENABLE", CFG_TYPE_U32, CFG_OFF(mp3_ctwin_enable), 4U, 0U, 1U},
	{"CONFIG_MP3_TERWIN_ENABLE", CFG_TYPE_U32, CFG_OFF(mp3_terwin_enable), 4U, 0U, 1U},
	{"CONFIG_MP3_CT_WIN_MUSICBOX_TRACK", CFG_TYPE_U32, CFG_OFF(mp3_ct_win_musicbox_track), 4U, 0U, 999U},
	{"CONFIG_MP3_T_WIN_MUSICBOX_TRACK", CFG_TYPE_U32, CFG_OFF(mp3_t_win_musicbox_track), 4U, 0U, 999U},
	{"CONFIG_MP3_DEFUSE_SUCCESS_WAIT_MS", CFG_TYPE_U32, CFG_OFF(mp3_defuse_success_wait_ms), 4U, 0U, 20000U},
	{"CONFIG_MP3_CT_MUSICBOX_WAIT_MS", CFG_TYPE_U32, CFG_OFF(mp3_ct_musicbox_wait_ms), 4U, 0U, 20000U},
	{"CONFIG_MP3_T_MUSICBOX_WAIT_MS", CFG_TYPE_U32, CFG_OFF(mp3_t_musicbox_wait_ms), 4U, 0U, 20000U},
	{"CONFIG_MP3_EXPLOSION_ONLY_WAIT_MS", CFG_TYPE_U32, CFG_OFF(mp3_explosion_only_wait_ms), 4U, 0U, 20000U},
	{"CONFIG_EXPLOSION_BEEP_MS", CFG_TYPE_U32, CFG_OFF(explosion_beep_ms), 4U, 0U, 5000U}
};

static AppConfig g_cfg;

static uint32_t ConfigDefaultsChecksum(void);

static uint32_t ConfigChecksum(const uint8_t *data, uint32_t len)
{
	uint32_t hash = 0x13572468U;
	uint32_t i;
	for (i = 0; i < len; i++)
	{
		hash = (hash << 5) | (hash >> 27);
		hash ^= data[i];
	}
	return hash;
}

static uint32_t ParseUint(const char *text, uint8_t *ok)
{
	uint32_t value = 0U;
	uint8_t has_digit = 0U;
	*ok = 0U;
	if (text == NULL)
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
		value = value * 10U + (uint32_t)(*text - '0');
		text++;
	}
	if (!has_digit)
	{
		return 0U;
	}
	*ok = 1U;
	return value;
}

static void UintToText(uint32_t value, char *out, uint8_t out_len)
{
	char rev[11];
	uint8_t i = 0U;
	uint8_t j;
	if (out == NULL || out_len == 0U)
	{
		return;
	}
	if (value == 0U)
	{
		if (out_len >= 2U)
		{
			out[0] = '0';
			out[1] = '\0';
		}
		else
		{
			out[0] = '\0';
		}
		return;
	}
	while (value > 0U && i < sizeof(rev))
	{
		rev[i++] = (char)('0' + (value % 10U));
		value /= 10U;
	}
	if ((uint8_t)(i + 1U) > out_len)
	{
		out[0] = '\0';
		return;
	}
	for (j = 0U; j < i; j++)
	{
		out[j] = rev[i - 1U - j];
	}
	out[i] = '\0';
}

static uint8_t IsStringDigits(const char *text)
{
	if (text == NULL)
	{
		return 0U;
	}
	while (*text != '\0')
	{
		if (*text < '0' || *text > '9')
		{
			return 0U;
		}
		text++;
	}
	return 1U;
}

static uint32_t ReadU32(const AppConfig *cfg, uint16_t offset)
{
	uint32_t value;
	memcpy(&value, ((const uint8_t *)cfg) + offset, sizeof(value));
	return value;
}

static void WriteU32(AppConfig *cfg, uint16_t offset, uint32_t value)
{
	memcpy(((uint8_t *)cfg) + offset, &value, sizeof(value));
}

static uint8_t FlashWrite(const uint8_t *bytes, uint16_t len)
{
	uint16_t i;
	FLASH_Status status;

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

	status = FLASH_ErasePage(CONFIG_FLASH_PAGE_ADDR);
	if (status != FLASH_COMPLETE)
	{
		FLASH_Lock();
		return 0U;
	}

	for (i = 0U; i < len; i = (uint16_t)(i + 2U))
	{
		uint16_t halfword = bytes[i];
		if ((uint16_t)(i + 1U) < len)
		{
			halfword |= (uint16_t)((uint16_t)bytes[i + 1U] << 8);
		}
		else
		{
			halfword |= 0xFF00U;
		}
		status = FLASH_ProgramHalfWord((uint32_t)(CONFIG_FLASH_PAGE_ADDR + i), halfword);
		if (status != FLASH_COMPLETE)
		{
			FLASH_Lock();
			return 0U;
		}
	}

	FLASH_Lock();
	return 1U;
}

static uint32_t ConfigDefaultsChecksum(void)
{
	AppConfig defaults;
	ConfigManager_LoadDefaults(&defaults);
	return ConfigChecksum((const uint8_t *)&defaults, sizeof(AppConfig));
}

void ConfigManager_LoadDefaults(AppConfig *cfg)
{
	if (cfg == NULL)
	{
		return;
	}
	memset(cfg, 0, sizeof(AppConfig));

	cfg->ignore_range = CONFIG_IGNORE_RANGE;

	cfg->password_len = CONFIG_PASSWORD_LEN;
	cfg->lcd_cols = CONFIG_LCD_COLS;
	cfg->lcd_normal_mount = CONFIG_LCD_NORMAL_MOUNT;
	cfg->password_col = CONFIG_PASSWORD_COL;
	cfg->countdown_ms = CONFIG_COUNTDOWN_MS;
	cfg->arm_delay_ms = CONFIG_ARM_DELAY_MS;

	cfg->arm_preset_enable = CONFIG_ARM_PRESET_ENABLE;
	strncpy(cfg->arm_preset_password, CONFIG_ARM_PRESET_PASSWORD, CONFIG_PASSWORD_LEN_MAX);
	cfg->arm_preset_password[CONFIG_PASSWORD_LEN_MAX] = '\0';

	cfg->defuse_enable_password = CONFIG_DEFUSE_ENABLE_PASSWORD;
	cfg->defuse_enable_manual = CONFIG_DEFUSE_ENABLE_MANUAL;
	cfg->defuse_enable_external = CONFIG_DEFUSE_ENABLE_EXTERNAL;

	cfg->lcd_backlight_pct = CONFIG_LCD_BACKLIGHT_PCT;

	cfg->beep_len_ms = CONFIG_BEEP_LEN_MS;
	cfg->beep_interval_base_ms = CONFIG_BEEP_INTERVAL_BASE_MS;
	cfg->beep_interval_scale_ms = CONFIG_BEEP_INTERVAL_SCALE_MS;
	cfg->beep_interval_min_ms = CONFIG_BEEP_INTERVAL_MIN_MS;
	cfg->beep_initial_ms = CONFIG_BEEP_INITIAL_MS;

	cfg->led_pwm_max = CONFIG_LED_PWM_MAX;
	cfg->led_breath_period_ms = CONFIG_LED_BREATH_PERIOD_MS;
	cfg->led_yellow_red_pct = CONFIG_LED_YELLOW_RED_PCT;

	cfg->scroll_interval_ms = CONFIG_SCROLL_INTERVAL_MS;
	cfg->scroll_pattern_len = CONFIG_SCROLL_PATTERN_LEN;
	cfg->digital_countdown_enable = CONFIG_DIGITAL_COUNTDOWN_ENABLE;

	cfg->long_press_ms = CONFIG_LONG_PRESS_MS;
	cfg->manual_defuse_ms = CONFIG_MANUAL_DEFUSE_MS;
	cfg->external_defuse_ms = CONFIG_EXTERNAL_DEFUSE_MS;
	cfg->defuse_display_hold_ms = CONFIG_DEFUSE_DISPLAY_HOLD_MS;
	cfg->defuse_cycle_steps = CONFIG_DEFUSE_CYCLE_STEPS;

	cfg->defuse_flash_toggle_ms = CONFIG_DEFUSE_FLASH_TOGGLE_MS;
	cfg->defuse_flash_toggles = CONFIG_DEFUSE_FLASH_TOGGLES;
	cfg->defuse_blink_ms = CONFIG_DEFUSE_BLINK_MS;

	cfg->buzzer_startup_freq_hz = CONFIG_BUZZER_STARTUP_FREQ_HZ;
	cfg->buzzer_countdown_freq_hz = CONFIG_BUZZER_COUNTDOWN_FREQ_HZ;
	cfg->buzzer_duty_pct = CONFIG_BUZZER_DUTY_PCT;
	cfg->startup_beep_on_ms = CONFIG_STARTUP_BEEP_ON_MS;
	cfg->startup_beep_gap_ms = CONFIG_STARTUP_BEEP_GAP_MS;
	cfg->startup_beep_enable = CONFIG_STARTUP_BEEP_ENABLE;

	cfg->mp3_volume = CONFIG_MP3_VOLUME;
	cfg->mp3_arm_success_enable = CONFIG_MP3_ARM_SUCCESS_ENABLE;
	cfg->mp3_defuse_success_enable = CONFIG_MP3_DEFUSE_SUCCESS_ENABLE;

	cfg->mp3_track_arm_success = CONFIG_MP3_TRACK_ARM_SUCCESS;
	cfg->mp3_track_explosion = CONFIG_MP3_TRACK_EXPLOSION;
	cfg->mp3_track_defuse_success = CONFIG_MP3_TRACK_DEFUSE_SUCCESS;
	cfg->mp3_track_ctwin = CONFIG_MP3_TRACK_CTWIN;
	cfg->mp3_track_terwin = CONFIG_MP3_TRACK_TERWIN;

	cfg->mp3_defuse_stage_enable = CONFIG_MP3_DEFUSE_STAGE_ENABLE;
	cfg->mp3_exploded_stage_enable = CONFIG_MP3_EXPLODED_STAGE_ENABLE;

	cfg->mp3_explosion_enable = CONFIG_MP3_EXPLOSION_ENABLE;
	cfg->mp3_ct_win_musicbox_enable = CONFIG_MP3_CT_WIN_MUSICBOX_ENABLE;
	cfg->mp3_t_win_musicbox_enable = CONFIG_MP3_T_WIN_MUSICBOX_ENABLE;
	cfg->mp3_ctwin_enable = CONFIG_MP3_CTWIN_ENABLE;
	cfg->mp3_terwin_enable = CONFIG_MP3_TERWIN_ENABLE;

	cfg->mp3_ct_win_musicbox_track = CONFIG_MP3_CT_WIN_MUSICBOX_TRACK;
	cfg->mp3_t_win_musicbox_track = CONFIG_MP3_T_WIN_MUSICBOX_TRACK;

	cfg->mp3_defuse_success_wait_ms = CONFIG_MP3_DEFUSE_SUCCESS_WAIT_MS;
	cfg->mp3_ct_musicbox_wait_ms = CONFIG_MP3_CT_MUSICBOX_WAIT_MS;
	cfg->mp3_t_musicbox_wait_ms = CONFIG_MP3_T_MUSICBOX_WAIT_MS;
	cfg->mp3_explosion_only_wait_ms = CONFIG_MP3_EXPLOSION_ONLY_WAIT_MS;
	cfg->explosion_beep_ms = CONFIG_EXPLOSION_BEEP_MS;
}

void ConfigManager_CopyCurrent(AppConfig *cfg)
{
	if (cfg != NULL)
	{
		memcpy(cfg, &g_cfg, sizeof(AppConfig));
	}
}

const AppConfig *ConfigManager_Get(void)
{
	return &g_cfg;
}

void ConfigManager_Commit(const AppConfig *cfg)
{
	if (cfg != NULL)
	{
		memcpy(&g_cfg, cfg, sizeof(AppConfig));
	}
}

uint8_t ConfigManager_Validate(const AppConfig *cfg)
{
	uint16_t i;
	if (cfg == NULL)
	{
		return 0U;
	}

	if (cfg->password_len < 1U || cfg->password_len > CONFIG_PASSWORD_LEN_MAX)
	{
		return 0U;
	}
	if (cfg->lcd_cols < 8U || cfg->lcd_cols > CONFIG_LCD_COLS_MAX)
	{
		return 0U;
	}
	if (cfg->password_col > (cfg->lcd_cols - cfg->password_len))
	{
		return 0U;
	}
	if (!IsStringDigits(cfg->arm_preset_password))
	{
		return 0U;
	}
	if (cfg->arm_preset_enable)
	{
		if ((uint32_t)strlen(cfg->arm_preset_password) != cfg->password_len)
		{
			return 0U;
		}
	}

	if (cfg->ignore_range)
	{
		return 1U;
	}

	for (i = 0U; i < CONFIG_ID_COUNT; i++)
	{
		const ConfigMeta *meta = &g_config_meta[i];
		if (meta->type == CFG_TYPE_U32)
		{
			uint32_t value = ReadU32(cfg, meta->offset);
			if (value < meta->min || value > meta->max)
			{
				return 0U;
			}
		}
	}

	return 1U;
}

uint8_t ConfigManager_Save(const AppConfig *cfg)
{
	ConfigStoreImage img;
	if (cfg == NULL)
	{
		return 0U;
	}
	if (!ConfigManager_Validate(cfg))
	{
		return 0U;
	}
	img.magic = CONFIG_STORE_MAGIC;
	img.version = CONFIG_STORE_VERSION;
	img.config_count = CONFIG_ID_COUNT;
	img.payload_size = sizeof(AppConfig);
	img.defaults_checksum = ConfigDefaultsChecksum();
	memcpy(&img.payload, cfg, sizeof(AppConfig));
	img.checksum = ConfigChecksum((const uint8_t *)&img.payload, sizeof(AppConfig));

	if (!FlashWrite((const uint8_t *)&img, (uint16_t)sizeof(ConfigStoreImage)))
	{
		return 0U;
	}

	{
		const ConfigStoreImage *verify = (const ConfigStoreImage *)CONFIG_FLASH_PAGE_ADDR;
		if (verify->magic != img.magic || verify->version != img.version || verify->config_count != img.config_count || verify->payload_size != img.payload_size || verify->defaults_checksum != img.defaults_checksum || verify->checksum != img.checksum)
		{
			return 0U;
		}
	}

	return 1U;
}

uint8_t ConfigManager_SaveCurrent(void)
{
	return ConfigManager_Save(&g_cfg);
}

uint8_t ConfigManager_RestoreDefaults(void)
{
	AppConfig defaults;
	ConfigManager_LoadDefaults(&defaults);
	if (!ConfigManager_Save(&defaults))
	{
		return 0U;
	}
	ConfigManager_Commit(&defaults);
	return 1U;
}

uint16_t ConfigManager_GetCount(void)
{
	return CONFIG_ID_COUNT;
}

const char *ConfigManager_GetName(uint16_t id)
{
	if (id >= CONFIG_ID_COUNT)
	{
		return "";
	}
	return g_config_meta[id].name;
}

uint8_t ConfigManager_GetValueString(const AppConfig *cfg, uint16_t id, char *out, uint8_t out_len)
{
	const ConfigMeta *meta;
	if (cfg == NULL || out == NULL || out_len == 0U || id >= CONFIG_ID_COUNT)
	{
		return 0U;
	}
	meta = &g_config_meta[id];
	if (meta->type == CFG_TYPE_U32)
	{
		UintToText(ReadU32(cfg, meta->offset), out, out_len);
		return (out[0] != '\0') ? 1U : 0U;
	}
	strncpy(out, (const char *)(((const uint8_t *)cfg) + meta->offset), (size_t)(out_len - 1U));
	out[out_len - 1U] = '\0';
	return 1U;
}

uint8_t ConfigManager_SetValueString(AppConfig *cfg, uint16_t id, const char *value)
{
	const ConfigMeta *meta;
	uint8_t ok;
	if (cfg == NULL || value == NULL || id >= CONFIG_ID_COUNT)
	{
		return 0U;
	}
	meta = &g_config_meta[id];
	if (meta->type == CFG_TYPE_U32)
	{
		uint32_t parsed = ParseUint(value, &ok);
		if (!ok)
		{
			return 0U;
		}
		WriteU32(cfg, meta->offset, parsed);
		return 1U;
	}
	if (!IsStringDigits(value))
	{
		return 0U;
	}
	if (strlen(value) >= meta->size)
	{
		return 0U;
	}
	memset(((uint8_t *)cfg) + meta->offset, 0, meta->size);
	strncpy((char *)(((uint8_t *)cfg) + meta->offset), value, meta->size - 1U);
	return 1U;
}

void ConfigManager_Init(void)
{
	const ConfigStoreImage *img = (const ConfigStoreImage *)CONFIG_FLASH_PAGE_ADDR;
	uint32_t defaults_checksum;
	ConfigManager_LoadDefaults(&g_cfg);
	defaults_checksum = ConfigDefaultsChecksum();

	if (img->magic != CONFIG_STORE_MAGIC)
	{
		return;
	}
	if (img->version != CONFIG_STORE_VERSION)
	{
		return;
	}
	if (img->config_count != CONFIG_ID_COUNT)
	{
		return;
	}
	if (img->payload_size != sizeof(AppConfig))
	{
		return;
	}
	if (img->defaults_checksum != defaults_checksum)
	{
		return;
	}
	if (img->checksum != ConfigChecksum((const uint8_t *)&img->payload, sizeof(AppConfig)))
	{
		return;
	}
	if (!ConfigManager_Validate(&img->payload))
	{
		return;
	}

	memcpy(&g_cfg, &img->payload, sizeof(AppConfig));
}


