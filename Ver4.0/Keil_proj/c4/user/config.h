#ifndef CONFIG_H
#define CONFIG_H

/* Core behavior */
#define CONFIG_PASSWORD_LEN 7U
#define CONFIG_LCD_COLS 16U
#define CONFIG_PASSWORD_COL 3U
#define CONFIG_COUNTDOWN_MS 40000U
#define CONFIG_ARM_DELAY_MS 800U

/* LCD backlight (PWM, percent 0-100) */
#define CONFIG_LCD_BACKLIGHT_PCT 50U

/* Beep timing (interval = base + scale * remaining_ratio, min clamp) */
#define CONFIG_BEEP_LEN_MS 125U
#define CONFIG_BEEP_INTERVAL_BASE_S 0.1f
#define CONFIG_BEEP_INTERVAL_SCALE_S 0.9f
#define CONFIG_BEEP_INTERVAL_MIN_S 0.15f
#define CONFIG_BEEP_INITIAL_MS 1000U

/* LED behavior */
#define CONFIG_LED_PWM_MAX 1000U
#define CONFIG_LED_BREATH_PERIOD_MS 500U
#define CONFIG_LED_YELLOW_RED_PCT 70U

/* Countdown LCD scroll */
#define CONFIG_SCROLL_INTERVAL_MS 50U
#define CONFIG_SCROLL_PATTERN_LEN 3U

/* Defuse timings */
#define CONFIG_LONG_PRESS_MS 1000U
#define CONFIG_MANUAL_DEFUSE_MS 10000U
#define CONFIG_EXTERNAL_DEFUSE_MS 5000U
#define CONFIG_DEFUSE_DISPLAY_HOLD_MS 1200U
#define CONFIG_DEFUSE_CYCLE_STEPS 10U

/* Success effect timing */
#define CONFIG_DEFUSE_FLASH_TOGGLE_MS 50U
#define CONFIG_DEFUSE_FLASH_TOGGLES 8U
#define CONFIG_DEFUSE_BLINK_MS 400U

/* Buzzer */
#define CONFIG_BUZZER_STARTUP_FREQ_HZ 4250U
#define CONFIG_BUZZER_COUNTDOWN_FREQ_HZ 3600U
#define CONFIG_STARTUP_BEEP_ON_MS 60U
#define CONFIG_STARTUP_BEEP_GAP_MS 80U

/* MP3 */
#define CONFIG_MP3_VOLUME 25U
/* mp3_over() is the arm-success (bomb planted) cue, not defuse success. */
#define CONFIG_MP3_ARM_SUCCESS_ENABLE 1U
/* When enabled, explosion uses mp3_boom_music() if CONFIG_MP3_EXPLOSION_USE_MUSIC is 1. */
#define CONFIG_MP3_EXPLOSION_ENABLE 1U
/* 1 = music box explosion, 0 = pure explosion sound. */
#define CONFIG_MP3_EXPLOSION_USE_MUSIC 1U

#endif
