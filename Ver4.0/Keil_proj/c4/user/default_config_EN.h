#ifndef CONFIG_H
#define CONFIG_H

/* Range check */
/* CONFIG_IGNORE_RANGE: 0 = check ranges, 1 = ignore ranges */
#define CONFIG_IGNORE_RANGE 0U

/* Core behavior */
/* CONFIG_PASSWORD_LEN: password length (digits)
   Min: 1     Max: 16     Typical: 7 */
#define CONFIG_PASSWORD_LEN 7U
/* CONFIG_LCD_COLS: LCD columns
   Min: 8     Max: 16     Typical: 16 */
#define CONFIG_LCD_COLS 16U
/* CONFIG_LCD_NORMAL_MOUNT: LCD mount orientation, 1 = normal (0 = inverted) */
#define CONFIG_LCD_NORMAL_MOUNT 0U
/* CONFIG_PASSWORD_COL: password start column
   Min: 0     Max: (LCD_COLS - PASSWORD_LEN)     Typical: 3 */
#define CONFIG_PASSWORD_COL 3U
/* CONFIG_COUNTDOWN_MS: countdown time (ms)
   Min: 1000  Max: none   Typical: 40000 */
#define CONFIG_COUNTDOWN_MS 40000U
/* CONFIG_ARM_DELAY_MS: arm delay after last key (ms)
   Min: 0     Max: 5000   Typical: 800 */
#define CONFIG_ARM_DELAY_MS 800U

/* Arm preset password */
/* CONFIG_ARM_PRESET_ENABLE: require preset password to arm, 1 = enable (0/1) */
#define CONFIG_ARM_PRESET_ENABLE 0U
/* CONFIG_ARM_PRESET_PASSWORD: preset password (length must equal CONFIG_PASSWORD_LEN) */
#define CONFIG_ARM_PRESET_PASSWORD "1234567"

/* Defuse mode enable (0/1) */
/* CONFIG_DEFUSE_ENABLE_PASSWORD: allow password input defuse, 1 = enable (0/1) */
#define CONFIG_DEFUSE_ENABLE_PASSWORD 1U
/* CONFIG_DEFUSE_ENABLE_MANUAL: allow long-press # manual defuse, 1 = enable (0/1) */
#define CONFIG_DEFUSE_ENABLE_MANUAL 1U
/* CONFIG_DEFUSE_ENABLE_EXTERNAL: allow external defuser defuse, 1 = enable (0/1) */
#define CONFIG_DEFUSE_ENABLE_EXTERNAL 1U

/* LCD backlight */
/* CONFIG_LCD_BACKLIGHT_PCT: backlight brightness (%) 
   Min: 0     Max: 100    Typical: 50 */
#define CONFIG_LCD_BACKLIGHT_PCT 50U

/* Beep timing */
/* CONFIG_BEEP_LEN_MS: beep on-time (ms)
   Min: 10    Max: 500    Typical: 125 */
#define CONFIG_BEEP_LEN_MS 125U
/* CONFIG_BEEP_INTERVAL_BASE_MS: base interval (ms)
   Min: 10    Max: 1000   Typical: 100 */
#define CONFIG_BEEP_INTERVAL_BASE_MS 100U
/* CONFIG_BEEP_INTERVAL_SCALE_MS: interval scale (ms)
   Min: 0     Max: 2000   Typical: 900 */
#define CONFIG_BEEP_INTERVAL_SCALE_MS 900U
/* CONFIG_BEEP_INTERVAL_MIN_MS: minimum interval (ms)
   Min: 50    Max: 1000   Typical: 150 */
#define CONFIG_BEEP_INTERVAL_MIN_MS 150U
/* CONFIG_BEEP_INITIAL_MS: first beep delay (ms)
   Min: 0     Max: 5000   Typical: 1000 */
#define CONFIG_BEEP_INITIAL_MS 1000U

/* LED behavior */
/* CONFIG_LED_PWM_MAX: light level range
   Min: 100   Max: 10000  Typical: 1000 */
#define CONFIG_LED_PWM_MAX 1000U
/* CONFIG_LED_BREATH_PERIOD_MS: idle breathing period (ms)
   Min: 100   Max: 5000   Typical: 500 */
#define CONFIG_LED_BREATH_PERIOD_MS 500U
/* CONFIG_LED_YELLOW_RED_PCT: yellow/red balance (%)
   Min: 0     Max: 100    Typical: 70 */
#define CONFIG_LED_YELLOW_RED_PCT 70U

/* Countdown LCD scroll */
/* CONFIG_SCROLL_INTERVAL_MS: scroll update interval (ms)
   Min: 10    Max: 500    Typical: 50 */
#define CONFIG_SCROLL_INTERVAL_MS 50U
/* CONFIG_SCROLL_PATTERN_LEN: scroll pattern length
   Min: 1     Max: 16     Typical: 3 */
#define CONFIG_SCROLL_PATTERN_LEN 3U

/* Defuse timings */
/* CONFIG_LONG_PRESS_MS: long-press threshold (ms)
   Min: 200   Max: 3000   Typical: 1000 */
#define CONFIG_LONG_PRESS_MS 1000U
/* CONFIG_MANUAL_DEFUSE_MS: manual defuse time (ms)
   Min: 1000  Max: none   Typical: 10000 */
#define CONFIG_MANUAL_DEFUSE_MS 10000U
/* CONFIG_EXTERNAL_DEFUSE_MS: external defuse time (ms)
   Min: 1000  Max: none   Typical: 5000 */
#define CONFIG_EXTERNAL_DEFUSE_MS 5000U
/* CONFIG_DEFUSE_DISPLAY_HOLD_MS: input hold before scroll (ms)
   Min: 200   Max: 5000   Typical: 1200 */
#define CONFIG_DEFUSE_DISPLAY_HOLD_MS 1200U
/* CONFIG_DEFUSE_CYCLE_STEPS: digit cycle steps
   Min: 1     Max: 20     Typical: 10 */
#define CONFIG_DEFUSE_CYCLE_STEPS 10U

/* Success effect */
/* CONFIG_DEFUSE_FLASH_TOGGLE_MS: flash toggle time (ms)
   Min: 10    Max: 200    Typical: 50 */
#define CONFIG_DEFUSE_FLASH_TOGGLE_MS 50U
/* CONFIG_DEFUSE_FLASH_TOGGLES: flash count
   Min: 1     Max: 20     Typical: 8 */
#define CONFIG_DEFUSE_FLASH_TOGGLES 8U
/* CONFIG_DEFUSE_BLINK_MS: blink time (ms)
   Min: 100   Max: 1000   Typical: 400 */
#define CONFIG_DEFUSE_BLINK_MS 400U

/* Buzzer */
/* CONFIG_BUZZER_STARTUP_FREQ_HZ: startup/defuse-success frequency (Hz)
   Min: 100   Max: 10000  Typical: 4250 */
#define CONFIG_BUZZER_STARTUP_FREQ_HZ 4250U
/* CONFIG_BUZZER_COUNTDOWN_FREQ_HZ: countdown frequency (Hz)
   Min: 100   Max: 10000  Typical: 3600 */
#define CONFIG_BUZZER_COUNTDOWN_FREQ_HZ 3600U
/* CONFIG_BUZZER_DUTY_PCT: buzzer volume (0-50)
   Min: 0     Max: 50     Typical: 50 */
#define CONFIG_BUZZER_DUTY_PCT 50U
/* CONFIG_STARTUP_BEEP_ON_MS: startup beep length (ms)
   Min: 10    Max: 500    Typical: 60 */
#define CONFIG_STARTUP_BEEP_ON_MS 60U
/* CONFIG_STARTUP_BEEP_GAP_MS: startup beep gap (ms)
   Min: 10    Max: 500    Typical: 80 */
#define CONFIG_STARTUP_BEEP_GAP_MS 80U

/* MP3 */
/* CONFIG_MP3_VOLUME: MP3 volume (0-30)
   Min: 0     Max: 30     Typical: 25 */
#define CONFIG_MP3_VOLUME 25U
/* CONFIG_MP3_ARM_SUCCESS_ENABLE: play MP3 on arm success (0/1) */
#define CONFIG_MP3_ARM_SUCCESS_ENABLE 1U
/* CONFIG_MP3_EXPLOSION_ENABLE: play MP3 on explosion (0/1) */
#define CONFIG_MP3_EXPLOSION_ENABLE 1U
/* CONFIG_MP3_EXPLOSION_USE_MUSIC: explosion style selection (0/1) */
#define CONFIG_MP3_EXPLOSION_USE_MUSIC 1U

#include "config_range.h"

#endif
