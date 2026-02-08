#ifndef CONFIG_H
#define CONFIG_H

/* 范围检查 */
/* CONFIG_IGNORE_RANGE: 0 = 检查范围, 1 = 忽略范围 */
#define CONFIG_IGNORE_RANGE 0U

/* 核心行为 */
/* CONFIG_PASSWORD_LEN: 密码长度（位数）
   最小: 1     最大: 16     典型: 7 */
#define CONFIG_PASSWORD_LEN 7U
/* CONFIG_LCD_COLS: LCD 列数
   最小: 8     最大: 16     典型: 16 */
#define CONFIG_LCD_COLS 16U
/* CONFIG_LCD_NORMAL_MOUNT: LCD 是否正装
   0 = 倒装(默认)  1 = 正装 */
#define CONFIG_LCD_NORMAL_MOUNT 0U
/* CONFIG_PASSWORD_COL: 密码起始列
   最小: 0     最大: (LCD_COLS - PASSWORD_LEN)     典型: 3 */
#define CONFIG_PASSWORD_COL 3U
/* CONFIG_COUNTDOWN_MS: 倒计时（毫秒）
   最小: 1000  最大: 无     典型: 40000 */
#define CONFIG_COUNTDOWN_MS 40000U
/* CONFIG_ARM_DELAY_MS: 最后按键后防误触延迟（毫秒）
   最小: 0     最大: 5000   典型: 800 */
#define CONFIG_ARM_DELAY_MS 800U

/* 下包预设密码 */
/* CONFIG_ARM_PRESET_ENABLE: 下包时必须使用预设密码 1为启用（0/1） */
#define CONFIG_ARM_PRESET_ENABLE 0U
/* CONFIG_ARM_PRESET_PASSWORD: 预设密码（长度必须等于 CONFIG_PASSWORD_LEN） */
#define CONFIG_ARM_PRESET_PASSWORD "1234567"

/* 拆弹模式开关（0/1） */
/* CONFIG_DEFUSE_ENABLE_PASSWORD: 允许输入密码拆弹 1为启用（0/1） */
#define CONFIG_DEFUSE_ENABLE_PASSWORD 1U
/* CONFIG_DEFUSE_ENABLE_MANUAL: 允许长按 # 手动拆弹 1为启用（0/1） */
#define CONFIG_DEFUSE_ENABLE_MANUAL 1U
/* CONFIG_DEFUSE_ENABLE_EXTERNAL: 允许外部拆弹器拆弹 1为启用（0/1） */
#define CONFIG_DEFUSE_ENABLE_EXTERNAL 1U

/* LCD 背光 */
/* CONFIG_LCD_BACKLIGHT_PCT: LCD背光板亮度（百分比）
   最小: 0     最大: 100    典型: 50 */
#define CONFIG_LCD_BACKLIGHT_PCT 50U

/* 蜂鸣器时序 */
/* CONFIG_BEEP_LEN_MS: 倒计时每次蜂鸣开启时长（毫秒）
   最小: 10    最大: 500    典型: 125 */
#define CONFIG_BEEP_LEN_MS 125U
/* CONFIG_BEEP_INTERVAL_BASE_MS: 基础间隔（毫秒）
   最小: 10    最大: 1000   典型: 100 */
#define CONFIG_BEEP_INTERVAL_BASE_MS 100U
/* CONFIG_BEEP_INTERVAL_SCALE_MS: 间隔缩放（毫秒）
   最小: 0     最大: 2000   典型: 900 */
#define CONFIG_BEEP_INTERVAL_SCALE_MS 900U
/* CONFIG_BEEP_INTERVAL_MIN_MS: 最小间隔（毫秒）
   最小: 50    最大: 1000   典型: 150 */
#define CONFIG_BEEP_INTERVAL_MIN_MS 150U
/* CONFIG_BEEP_INITIAL_MS: 首次蜂鸣延迟（毫秒）
   最小: 0     最大: 5000   典型: 1000 */
#define CONFIG_BEEP_INITIAL_MS 1000U

/* LED 行为 */
/* CONFIG_LED_PWM_MAX: 灯光电平范围
   最小: 100   最大: 10000  典型: 1000 */
#define CONFIG_LED_PWM_MAX 1000U
/* CONFIG_LED_BREATH_PERIOD_MS: 闲置呼吸周期（毫秒）
   最小: 100   最大: 5000   典型: 500 */
#define CONFIG_LED_BREATH_PERIOD_MS 500U
/* CONFIG_LED_YELLOW_RED_PCT: 黄色中 红/绿 灯 比例（百分比）
   最小: 0     最大: 100    典型: 70 */
#define CONFIG_LED_YELLOW_RED_PCT 70U

/* 倒计时 LCD 滚动 */
/* CONFIG_SCROLL_INTERVAL_MS: 滚动更新间隔（毫秒）
   最小: 10    最大: 500    典型: 50 */
#define CONFIG_SCROLL_INTERVAL_MS 50U
/* CONFIG_SCROLL_PATTERN_LEN: 滚动模式长度
   最小: 1     最大: 16     典型: 3 */
#define CONFIG_SCROLL_PATTERN_LEN 3U

/* 拆弹时间设置 */
/* CONFIG_LONG_PRESS_MS: 长按阈值（毫秒）
   最小: 200   最大: 3000   典型: 1000 */
#define CONFIG_LONG_PRESS_MS 1000U
/* CONFIG_MANUAL_DEFUSE_MS: 手动拆弹时间（毫秒）
   最小: 1000  最大: 无     典型: 10000 */
#define CONFIG_MANUAL_DEFUSE_MS 10000U
/* CONFIG_EXTERNAL_DEFUSE_MS: 拆弹器拆弹时间（毫秒）
   最小: 1000  最大: 无     典型: 5000 */
#define CONFIG_EXTERNAL_DEFUSE_MS 5000U
/* CONFIG_DEFUSE_DISPLAY_HOLD_MS: 输入保持显示切换前的时间（毫秒）
   最小: 200   最大: 5000   典型: 1200 */
#define CONFIG_DEFUSE_DISPLAY_HOLD_MS 1200U
/* CONFIG_DEFUSE_CYCLE_STEPS: 数字循环步数
   最小: 1     最大: 20     典型: 10 */
#define CONFIG_DEFUSE_CYCLE_STEPS 10U

/* 拆弹成功效果 */
/* CONFIG_DEFUSE_FLASH_TOGGLE_MS: 拆弹成功动画闪烁切换时间（毫秒）
   最小: 10    最大: 200    典型: 50 */
#define CONFIG_DEFUSE_FLASH_TOGGLE_MS 50U
/* CONFIG_DEFUSE_FLASH_TOGGLES: 拆弹成功动画闪烁次数
   最小: 1     最大: 20     典型: 8 */
#define CONFIG_DEFUSE_FLASH_TOGGLES 8U
/* CONFIG_DEFUSE_BLINK_MS: 拆弹成功动画闪屏时长（毫秒）
   最小: 100   最大: 1000   典型: 400 */
#define CONFIG_DEFUSE_BLINK_MS 400U

/* 蜂鸣器 */
/* CONFIG_BUZZER_STARTUP_FREQ_HZ: 启动/拆弹成功 频率（Hz）
   最小: 100   最大: 10000  典型: 4250 */
#define CONFIG_BUZZER_STARTUP_FREQ_HZ 4250U
/* CONFIG_BUZZER_COUNTDOWN_FREQ_HZ: 倒计时频率（Hz）
   最小: 100   最大: 10000  典型: 3600 */
#define CONFIG_BUZZER_COUNTDOWN_FREQ_HZ 3600U
/* CONFIG_BUZZER_DUTY_PCT: 蜂鸣占空比或音量（0-50）
   最小: 0     最大: 50     典型: 50 */
#define CONFIG_BUZZER_DUTY_PCT 50U
/* CONFIG_STARTUP_BEEP_ON_MS: 上电蜂鸣时长（毫秒）
   最小: 10    最大: 500    典型: 60 */
#define CONFIG_STARTUP_BEEP_ON_MS 60U
/* CONFIG_STARTUP_BEEP_GAP_MS: 上电蜂鸣间隔（毫秒）
   最小: 10    最大: 500    典型: 80 */
#define CONFIG_STARTUP_BEEP_GAP_MS 80U

/* MP3 */
/* CONFIG_MP3_VOLUME: MP3 音量（0-30）
   最小: 0     最大: 30     典型: 25 */
#define CONFIG_MP3_VOLUME 25U
/* CONFIG_MP3_ARM_SUCCESS_ENABLE: 下包成功时播放音效（0/1） */
#define CONFIG_MP3_ARM_SUCCESS_ENABLE 1U
/* CONFIG_MP3_EXPLOSION_ENABLE: 爆炸时播放 MP3（0/1） */
#define CONFIG_MP3_EXPLOSION_ENABLE 1U
/* CONFIG_MP3_EXPLOSION_USE_MUSIC: 爆炸音效是否启用音乐盒 1为启用（0/1） */
#define CONFIG_MP3_EXPLOSION_USE_MUSIC 1U

#include "config_range.h"

#endif
