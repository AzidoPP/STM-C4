# Ver4.0 配置说明 / Configuration Guide

## 中文（教程 + 完整配置）

### 快速教程
1. 先按住 `#` 不放。
2. 在按住 `#` 的同时，按一下 `RESET`（或重新上电）。
3. 继续按住 `#` 约 1 秒，直到屏幕进入配置界面。

> 这样修改配置时，不需要重新下载程序；按步骤保存后即可按新设置运行。

### 修改一个配置
1. 输入配置编号（例如 `24`）。
2. 按 `#` 确认编号。
3. 输入新数值（例如 `500`）。
4. 按 `#` 进入保存确认。
5. 再按一次 `#` 保存。
6. 屏幕出现 `STORED` 后，按 `RESET`。

### 按键说明
- `0~9`：输入数字
- `*`：删掉最后一位
- `#`：确认当前步骤

### 恢复出厂设置
- 输入 `090405`，再按 `#` 确认恢复。

### 示例
- 把 `CONFIG_LONG_PRESS_MS`（`[24]`）改成 `500`：进入配置模式 -> 输入 `24` -> `#` -> 输入 `500` -> `#` -> `#` 保存 -> `RESET`。

/* 范围检查 */
/* CONFIG_IGNORE_RANGE: 0 = 检查范围, 1 = 忽略范围 [0] */
#define CONFIG_IGNORE_RANGE 0U

/* 核心行为 */
/* CONFIG_PASSWORD_LEN: 密码长度（位数） [1]
   最小: 1     最大: 16     典型: 7 */
#define CONFIG_PASSWORD_LEN 7U
/* CONFIG_LCD_COLS: LCD 列数 [2]
   最小: 8     最大: 16     典型: 16 */
#define CONFIG_LCD_COLS 16U
/* CONFIG_LCD_NORMAL_MOUNT: LCD 是否正装 [3]
   0 = 倒装(默认)  1 = 正装 */
#define CONFIG_LCD_NORMAL_MOUNT 0U
/* CONFIG_PASSWORD_COL: 密码起始列 [4]
   最小: 0     最大: (LCD_COLS - PASSWORD_LEN)     典型: 3 */
#define CONFIG_PASSWORD_COL 3U
/* CONFIG_COUNTDOWN_MS: 倒计时（毫秒） [5]
   最小: 1000  最大: 无     典型: 15000 */
#define CONFIG_COUNTDOWN_MS 15000U
/* CONFIG_ARM_DELAY_MS: 最后按键后防误触延迟（毫秒） [6]
   最小: 0     最大: 5000   典型: 800 */
#define CONFIG_ARM_DELAY_MS 800U

/* 下包预设密码 */
/* CONFIG_ARM_PRESET_ENABLE: 下包时必须使用预设密码 1为启用（0/1） [7] */
#define CONFIG_ARM_PRESET_ENABLE 0U
/* CONFIG_ARM_PRESET_PASSWORD: 预设密码（长度必须等于 CONFIG_PASSWORD_LEN） [8] */
#define CONFIG_ARM_PRESET_PASSWORD "1234567"
/* CONFIG_ARM_ERROR_HINT_ENABLE: 下包固定密码输错时是否显示 ERROR（0/1） [60]
   0 = 静默清空（当前默认）
   1 = 显示 ERROR 后清空 */
#define CONFIG_ARM_ERROR_HINT_ENABLE 0U

/* 拆弹模式开关（0/1） */
/* CONFIG_DEFUSE_ENABLE_PASSWORD: 允许输入密码拆弹 1为启用（0/1） [9] */
#define CONFIG_DEFUSE_ENABLE_PASSWORD 1U
/* CONFIG_DEFUSE_ENABLE_MANUAL: 允许长按 # 手动拆弹 1为启用（0/1） [10] */
#define CONFIG_DEFUSE_ENABLE_MANUAL 1U
/* CONFIG_DEFUSE_ENABLE_EXTERNAL: 允许外部拆弹器拆弹 1为启用（0/1） [11] */
#define CONFIG_DEFUSE_ENABLE_EXTERNAL 1U

/* LCD 背光 */
/* CONFIG_LCD_BACKLIGHT_PCT: LCD背光板亮度（百分比） [12]
   最小: 0     最大: 100    典型: 50 */
#define CONFIG_LCD_BACKLIGHT_PCT 50U

/* 蜂鸣器时序 */
/* CONFIG_BEEP_LEN_MS: 倒计时每次蜂鸣开启时长（毫秒） [13]
   最小: 10    最大: 500    典型: 125 */
#define CONFIG_BEEP_LEN_MS 125U
/* CONFIG_BEEP_INTERVAL_BASE_MS: 基础间隔（毫秒） [14]
   最小: 10    最大: 1000   典型: 100 */
#define CONFIG_BEEP_INTERVAL_BASE_MS 100U
/* CONFIG_BEEP_INTERVAL_SCALE_MS: 间隔缩放（毫秒） [15]
   最小: 0     最大: 2000   典型: 900 */
#define CONFIG_BEEP_INTERVAL_SCALE_MS 900U
/* CONFIG_BEEP_INTERVAL_MIN_MS: 最小间隔（毫秒） [16]
   最小: 50    最大: 1000   典型: 150 */
#define CONFIG_BEEP_INTERVAL_MIN_MS 150U
/* CONFIG_BEEP_INITIAL_MS: 首次蜂鸣延迟（毫秒） [17]
   最小: 0     最大: 5000   典型: 1000 */
#define CONFIG_BEEP_INITIAL_MS 1000U

/* LED 行为 */
/* CONFIG_LED_PWM_MAX: 灯光电平范围 [18]
   最小: 100   最大: 10000  典型: 1000 */
#define CONFIG_LED_PWM_MAX 1000U
/* CONFIG_LED_BREATH_PERIOD_MS: 闲置呼吸周期（毫秒） [19]
   最小: 100   最大: 5000   典型: 700 */
#define CONFIG_LED_BREATH_PERIOD_MS 700U
/* CONFIG_LED_YELLOW_RED_PCT: 黄色中 红/绿 灯 比例（百分比） [20]
   最小: 0     最大: 100    典型: 40 */
#define CONFIG_LED_YELLOW_RED_PCT 40U

/* 倒计时 LCD 滚动 */
/* CONFIG_SCROLL_INTERVAL_MS: 滚动更新间隔（毫秒） [21]
   最小: 10    最大: 500    典型: 50 */
#define CONFIG_SCROLL_INTERVAL_MS 50U
/* CONFIG_SCROLL_PATTERN_LEN: 滚动模式长度 [22]
   最小: 1     最大: 16     典型: 3 */
#define CONFIG_SCROLL_PATTERN_LEN 3U
/* CONFIG_DIGITAL_COUNTDOWN_ENABLE: 倒计时数字显示开关（0/1） [23]
   0 = 仅左侧滚动动画
   1 = 左侧滚动 + 右侧 mm:ss */
#define CONFIG_DIGITAL_COUNTDOWN_ENABLE 0U

/* 拆弹时间设置 */
/* CONFIG_LONG_PRESS_MS: 长按阈值（毫秒） [24]
   最小: 200   最大: 3000   典型: 200 */
#define CONFIG_LONG_PRESS_MS 200U
/* CONFIG_MANUAL_DEFUSE_MS: 手动拆弹时间（毫秒） [25]
   最小: 1000  最大: 无     典型: 10000 */
#define CONFIG_MANUAL_DEFUSE_MS 10000U
/* CONFIG_EXTERNAL_DEFUSE_MS: 拆弹器拆弹时间（毫秒） [26]
   最小: 1000  最大: 无     典型: 5000 */
#define CONFIG_EXTERNAL_DEFUSE_MS 5000U
/* CONFIG_DEFUSE_DISPLAY_HOLD_MS: 输入保持显示切换前的时间（毫秒） [27]
   最小: 200   最大: 5000   典型: 1200 */
#define CONFIG_DEFUSE_DISPLAY_HOLD_MS 1200U
/* CONFIG_DEFUSE_CYCLE_STEPS: 数字循环步数 [28]
   最小: 1     最大: 20     典型: 10 */
#define CONFIG_DEFUSE_CYCLE_STEPS 10U

/* 拆弹成功效果 */
/* CONFIG_DEFUSE_FLASH_TOGGLE_MS: 拆弹成功动画闪烁切换时间（毫秒） [29]
   最小: 10    最大: 200    典型: 10 */
#define CONFIG_DEFUSE_FLASH_TOGGLE_MS 10U
/* CONFIG_DEFUSE_FLASH_TOGGLES: 拆弹成功动画闪烁次数 [30]
   最小: 1     最大: 20     典型: 8 */
#define CONFIG_DEFUSE_FLASH_TOGGLES 8U
/* CONFIG_DEFUSE_BLINK_MS: 拆弹成功动画闪屏时长（毫秒） [31]
   最小: 100   最大: 1000   典型: 400 */
#define CONFIG_DEFUSE_BLINK_MS 400U

/* 蜂鸣器 */
/* CONFIG_BUZZER_STARTUP_FREQ_HZ: 启动/拆弹成功 频率（Hz） [32]
   最小: 100   最大: 10000  典型: 4250 */
#define CONFIG_BUZZER_STARTUP_FREQ_HZ 4250U
/* CONFIG_BUZZER_COUNTDOWN_FREQ_HZ: 倒计时频率（Hz） [33]
   最小: 100   最大: 10000  典型: 3600 */
#define CONFIG_BUZZER_COUNTDOWN_FREQ_HZ 3600U
/* CONFIG_BUZZER_DUTY_PCT: 蜂鸣占空比或音量（0-50） [34]
   最小: 0     最大: 50     典型: 50 */
#define CONFIG_BUZZER_DUTY_PCT 50U
/* CONFIG_STARTUP_BEEP_ON_MS: 上电蜂鸣时长（毫秒） [35]
   最小: 10    最大: 500    典型: 50 */
#define CONFIG_STARTUP_BEEP_ON_MS 50U
/* CONFIG_STARTUP_BEEP_GAP_MS: 上电蜂鸣间隔（毫秒） [36]
   最小: 10    最大: 500    典型: 50 */
#define CONFIG_STARTUP_BEEP_GAP_MS 50U
/* CONFIG_STARTUP_BEEP_ENABLE: 是否启用上电蜂鸣 1为启用（0/1） [37] */
#define CONFIG_STARTUP_BEEP_ENABLE 1U

/* MP3 */
/* CONFIG_MP3_VOLUME: MP3 音量（0-30） [38]
   最小: 0     最大: 30     典型: 25 */
#define CONFIG_MP3_VOLUME 25U
/* CONFIG_MP3_ARM_SUCCESS_ENABLE: 下包成功时播放播报音效（0/1） [39]
   最小: 0     最大: 1      典型: 1 */
#define CONFIG_MP3_ARM_SUCCESS_ENABLE 1U
/* CONFIG_MP3_DEFUSE_SUCCESS_ENABLE: 拆弹成功时播放播报音效（0/1） [40]
   最小: 0     最大: 1      典型: 1 */
#define CONFIG_MP3_DEFUSE_SUCCESS_ENABLE 1U

/* 固定播报编号（MP3 文件夹，0xxx）
   CONFIG_MP3_TRACK_ARM_SUCCESS: 下包成功播报号（映射到 0101）
   最小: 1     最大: 999    典型: 101 [41] */
#define CONFIG_MP3_TRACK_ARM_SUCCESS 101U
/* CONFIG_MP3_TRACK_EXPLOSION: 纯爆炸音效号（映射到 0102） [42]
   最小: 1     最大: 999    典型: 102 */
#define CONFIG_MP3_TRACK_EXPLOSION 102U
/* CONFIG_MP3_TRACK_DEFUSE_SUCCESS: 拆弹成功播报号（映射到 0103） [43]
   最小: 1     最大: 999    典型: 103 */
#define CONFIG_MP3_TRACK_DEFUSE_SUCCESS 103U
/* CONFIG_MP3_TRACK_CTWIN: CT 胜利播报号（映射到 0104） [44]
   最小: 1     最大: 999    典型: 104 */
#define CONFIG_MP3_TRACK_CTWIN 104U
/* CONFIG_MP3_TRACK_TERWIN: T 胜利播报号（映射到 0105） [45]
   最小: 1     最大: 999    典型: 105 */
#define CONFIG_MP3_TRACK_TERWIN 105U

/* CONFIG_MP3_DEFUSE_STAGE_ENABLE: 拆弹成功阶段 MP3 总开关（0/1） [46]
   最小: 0     最大: 1      典型: 1 */
#define CONFIG_MP3_DEFUSE_STAGE_ENABLE 1U
/* CONFIG_MP3_EXPLODED_STAGE_ENABLE: 倒计时结束阶段 MP3 总开关（0/1） [47]
   最小: 0     最大: 1      典型: 1 */
#define CONFIG_MP3_EXPLODED_STAGE_ENABLE 1U

/* CONFIG_MP3_EXPLOSION_ENABLE: 倒计时结束时是否播放爆炸音效（0/1） [48]
   最小: 0     最大: 1      典型: 1 */
#define CONFIG_MP3_EXPLOSION_ENABLE 1U
/* CONFIG_MP3_CT_WIN_MUSICBOX_ENABLE: 拆弹成功后是否播放 CT 音乐盒（0/1） [49]
   最小: 0     最大: 1      典型: 1 */
#define CONFIG_MP3_CT_WIN_MUSICBOX_ENABLE 1U
/* CONFIG_MP3_T_WIN_MUSICBOX_ENABLE: 爆炸后是否播放 T 音乐盒（0/1） [50]
   最小: 0     最大: 1      典型: 1 */
#define CONFIG_MP3_T_WIN_MUSICBOX_ENABLE 1U
/* CONFIG_MP3_CTWIN_ENABLE: 是否播放 CT 胜利播报（0/1） [51]
   最小: 0     最大: 1      典型: 1 */
#define CONFIG_MP3_CTWIN_ENABLE 1U
/* CONFIG_MP3_TERWIN_ENABLE: 是否播放 T 胜利播报（0/1） [52]
   最小: 0     最大: 1      典型: 1 */
#define CONFIG_MP3_TERWIN_ENABLE 1U

/* CONFIG_MP3_CT_WIN_MUSICBOX_TRACK: CT 音乐盒编号 [53]
   最小: 0     最大: 999    典型: 10 */
#define CONFIG_MP3_CT_WIN_MUSICBOX_TRACK 10U
/* CONFIG_MP3_T_WIN_MUSICBOX_TRACK: T 音乐盒编号 [54]
   最小: 0     最大: 999    典型: 1 */
#define CONFIG_MP3_T_WIN_MUSICBOX_TRACK 1U

/* CONFIG_MP3_DEFUSE_SUCCESS_WAIT_MS: 拆弹成功播报后等待时长（毫秒） [55]
   最小: 0     最大: 20000  典型: 2700 */
#define CONFIG_MP3_DEFUSE_SUCCESS_WAIT_MS 2700U
/* CONFIG_MP3_CT_MUSICBOX_WAIT_MS: CT 音乐盒播放后等待时长（毫秒） [56]
   最小: 0     最大: 20000  典型: 9000 */
#define CONFIG_MP3_CT_MUSICBOX_WAIT_MS 9000U
/* CONFIG_MP3_T_MUSICBOX_WAIT_MS: T 音乐盒播放后等待时长（毫秒） [57]
   最小: 0     最大: 20000  典型: 10000 */
#define CONFIG_MP3_T_MUSICBOX_WAIT_MS 10000U
/* CONFIG_MP3_EXPLOSION_ONLY_WAIT_MS: 纯爆炸音效后等待时长（毫秒） [58]
   最小: 0     最大: 20000  典型: 7000 */
#define CONFIG_MP3_EXPLOSION_ONLY_WAIT_MS 7000U

/* CONFIG_EXPLOSION_BEEP_MS: 爆炸开始前置蜂鸣时长（毫秒） [59]
   最小: 0     最大: 5000   典型: 500 */
#define CONFIG_EXPLOSION_BEEP_MS 500U

---

## English (Tutorial + Full Config List)

### Quick Tutorial
1. Hold `#`.
2. While holding `#`, press `RESET` (or power cycle).
3. Keep holding `#` for about 1 second until config screen appears.

### Change one config item
1. Enter the config ID (example: `24`).
2. Press `#` to confirm the ID.
3. Enter the new value (example: `500`).
4. Press `#` to go to save confirmation.
5. Press `#` again to save.
6. When `STORED` appears, press `RESET`.

> This lets you change settings without uploading code again; after saving, the new setting is used directly.

### Keys
- `0~9`: input digits
- `*`: delete last digit
- `#`: confirm current step

### Restore factory settings
- Enter `090405`, then press `#` to confirm restore.

### Example
- Set `CONFIG_LONG_PRESS_MS` (`[24]`) to `500`: enter config mode -> `24` -> `#` -> `500` -> `#` -> `#` save -> `RESET`.

/* Range check */
/* CONFIG_IGNORE_RANGE: 0 = check ranges, 1 = ignore ranges [0] */
#define CONFIG_IGNORE_RANGE 0U

/* Core behavior */
/* CONFIG_PASSWORD_LEN: password length (digits) [1]
   Min: 1     Max: 16     Typical: 7 */
#define CONFIG_PASSWORD_LEN 7U
/* CONFIG_LCD_COLS: LCD columns [2]
   Min: 8     Max: 16     Typical: 16 */
#define CONFIG_LCD_COLS 16U
/* CONFIG_LCD_NORMAL_MOUNT: LCD mount orientation, 1 = normal (0 = inverted) [3] */
#define CONFIG_LCD_NORMAL_MOUNT 0U
/* CONFIG_PASSWORD_COL: password start column [4]
   Min: 0     Max: (LCD_COLS - PASSWORD_LEN)     Typical: 3 */
#define CONFIG_PASSWORD_COL 3U
/* CONFIG_COUNTDOWN_MS: countdown time (ms) [5]
   Min: 1000  Max: none   Typical: 15000 */
#define CONFIG_COUNTDOWN_MS 15000U
/* CONFIG_ARM_DELAY_MS: arm delay after last key (ms) [6]
   Min: 0     Max: 5000   Typical: 800 */
#define CONFIG_ARM_DELAY_MS 800U

/* Arm preset password */
/* CONFIG_ARM_PRESET_ENABLE: require preset password to arm, 1 = enable (0/1) [7] */
#define CONFIG_ARM_PRESET_ENABLE 0U
/* CONFIG_ARM_PRESET_PASSWORD: preset password (length must equal CONFIG_PASSWORD_LEN) [8] */
#define CONFIG_ARM_PRESET_PASSWORD "1234567"
/* CONFIG_ARM_ERROR_HINT_ENABLE: show ERROR when preset arm password is wrong (0/1) [60]
   0 = silent clear (current default)
   1 = show ERROR then clear */
#define CONFIG_ARM_ERROR_HINT_ENABLE 0U

/* Defuse mode enable (0/1) */
/* CONFIG_DEFUSE_ENABLE_PASSWORD: allow password input defuse, 1 = enable (0/1) [9] */
#define CONFIG_DEFUSE_ENABLE_PASSWORD 1U
/* CONFIG_DEFUSE_ENABLE_MANUAL: allow long-press # manual defuse, 1 = enable (0/1) [10] */
#define CONFIG_DEFUSE_ENABLE_MANUAL 1U
/* CONFIG_DEFUSE_ENABLE_EXTERNAL: allow external defuser defuse, 1 = enable (0/1) [11] */
#define CONFIG_DEFUSE_ENABLE_EXTERNAL 1U

/* LCD backlight */
/* CONFIG_LCD_BACKLIGHT_PCT: backlight brightness (%)  [12]
   Min: 0     Max: 100    Typical: 50 */
#define CONFIG_LCD_BACKLIGHT_PCT 50U

/* Beep timing */
/* CONFIG_BEEP_LEN_MS: beep on-time (ms) [13]
   Min: 10    Max: 500    Typical: 125 */
#define CONFIG_BEEP_LEN_MS 125U
/* CONFIG_BEEP_INTERVAL_BASE_MS: base interval (ms) [14]
   Min: 10    Max: 1000   Typical: 100 */
#define CONFIG_BEEP_INTERVAL_BASE_MS 100U
/* CONFIG_BEEP_INTERVAL_SCALE_MS: interval scale (ms) [15]
   Min: 0     Max: 2000   Typical: 900 */
#define CONFIG_BEEP_INTERVAL_SCALE_MS 900U
/* CONFIG_BEEP_INTERVAL_MIN_MS: minimum interval (ms) [16]
   Min: 50    Max: 1000   Typical: 150 */
#define CONFIG_BEEP_INTERVAL_MIN_MS 150U
/* CONFIG_BEEP_INITIAL_MS: first beep delay (ms) [17]
   Min: 0     Max: 5000   Typical: 1000 */
#define CONFIG_BEEP_INITIAL_MS 1000U

/* LED behavior */
/* CONFIG_LED_PWM_MAX: light level range [18]
   Min: 100   Max: 10000  Typical: 1000 */
#define CONFIG_LED_PWM_MAX 1000U
/* CONFIG_LED_BREATH_PERIOD_MS: idle breathing period (ms) [19]
   Min: 100   Max: 5000   Typical: 700 */
#define CONFIG_LED_BREATH_PERIOD_MS 700U
/* CONFIG_LED_YELLOW_RED_PCT: yellow/red balance (%) [20]
   Min: 0     Max: 100    Typical: 40 */
#define CONFIG_LED_YELLOW_RED_PCT 40U

/* Countdown LCD scroll */
/* CONFIG_SCROLL_INTERVAL_MS: scroll update interval (ms) [21]
   Min: 10    Max: 500    Typical: 50 */
#define CONFIG_SCROLL_INTERVAL_MS 50U
/* CONFIG_SCROLL_PATTERN_LEN: scroll pattern length [22]
   Min: 1     Max: 16     Typical: 3 */
#define CONFIG_SCROLL_PATTERN_LEN 3U
/* CONFIG_DIGITAL_COUNTDOWN_ENABLE: digital countdown display (0/1) [23]
   0 = left scroll only
   1 = left scroll + right mm:ss */
#define CONFIG_DIGITAL_COUNTDOWN_ENABLE 0U

/* Defuse timings */
/* CONFIG_LONG_PRESS_MS: long-press threshold (ms) [24]
   Min: 200   Max: 3000   Typical: 200 */
#define CONFIG_LONG_PRESS_MS 200U
/* CONFIG_MANUAL_DEFUSE_MS: manual defuse time (ms) [25]
   Min: 1000  Max: none   Typical: 10000 */
#define CONFIG_MANUAL_DEFUSE_MS 10000U
/* CONFIG_EXTERNAL_DEFUSE_MS: external defuse time (ms) [26]
   Min: 1000  Max: none   Typical: 5000 */
#define CONFIG_EXTERNAL_DEFUSE_MS 5000U
/* CONFIG_DEFUSE_DISPLAY_HOLD_MS: input hold before scroll (ms) [27]
   Min: 200   Max: 5000   Typical: 1200 */
#define CONFIG_DEFUSE_DISPLAY_HOLD_MS 1200U
/* CONFIG_DEFUSE_CYCLE_STEPS: digit cycle steps [28]
   Min: 1     Max: 20     Typical: 10 */
#define CONFIG_DEFUSE_CYCLE_STEPS 10U

/* Success effect */
/* CONFIG_DEFUSE_FLASH_TOGGLE_MS: flash toggle time (ms) [29]
   Min: 10    Max: 200    Typical: 10 */
#define CONFIG_DEFUSE_FLASH_TOGGLE_MS 10U
/* CONFIG_DEFUSE_FLASH_TOGGLES: flash count [30]
   Min: 1     Max: 20     Typical: 8 */
#define CONFIG_DEFUSE_FLASH_TOGGLES 8U
/* CONFIG_DEFUSE_BLINK_MS: blink time (ms) [31]
   Min: 100   Max: 1000   Typical: 400 */
#define CONFIG_DEFUSE_BLINK_MS 400U

/* Buzzer */
/* CONFIG_BUZZER_STARTUP_FREQ_HZ: startup/defuse-success frequency (Hz) [32]
   Min: 100   Max: 10000  Typical: 4250 */
#define CONFIG_BUZZER_STARTUP_FREQ_HZ 4250U
/* CONFIG_BUZZER_COUNTDOWN_FREQ_HZ: countdown frequency (Hz) [33]
   Min: 100   Max: 10000  Typical: 3600 */
#define CONFIG_BUZZER_COUNTDOWN_FREQ_HZ 3600U
/* CONFIG_BUZZER_DUTY_PCT: buzzer volume (0-50) [34]
   Min: 0     Max: 50     Typical: 50 */
#define CONFIG_BUZZER_DUTY_PCT 50U
/* CONFIG_STARTUP_BEEP_ON_MS: startup beep length (ms) [35]
   Min: 10    Max: 500    Typical: 50 */
#define CONFIG_STARTUP_BEEP_ON_MS 50U
/* CONFIG_STARTUP_BEEP_GAP_MS: startup beep gap (ms) [36]
   Min: 10    Max: 500    Typical: 50 */
#define CONFIG_STARTUP_BEEP_GAP_MS 50U
/* CONFIG_STARTUP_BEEP_ENABLE: startup buzzer enable (0/1) [37] */
#define CONFIG_STARTUP_BEEP_ENABLE 1U

/* MP3 */
/* CONFIG_MP3_VOLUME: MP3 volume (0-30) [38]
   Min: 0     Max: 30     Typical: 25 */
#define CONFIG_MP3_VOLUME 25U
/* CONFIG_MP3_ARM_SUCCESS_ENABLE: play arm-success announcement (0/1) [39]
   Min: 0     Max: 1      Typical: 1 */
#define CONFIG_MP3_ARM_SUCCESS_ENABLE 1U
/* CONFIG_MP3_DEFUSE_SUCCESS_ENABLE: play defuse-success announcement (0/1) [40]
   Min: 0     Max: 1      Typical: 1 */
#define CONFIG_MP3_DEFUSE_SUCCESS_ENABLE 1U

/* Fixed announcement indexes in MP3 folder (0xxx)
   CONFIG_MP3_TRACK_ARM_SUCCESS: arm-success announce id (maps to 0101)
   Min: 1     Max: 999    Typical: 101 [41] */
#define CONFIG_MP3_TRACK_ARM_SUCCESS 101U
/* CONFIG_MP3_TRACK_EXPLOSION: pure explosion id (maps to 0102) [42]
   Min: 1     Max: 999    Typical: 102 */
#define CONFIG_MP3_TRACK_EXPLOSION 102U
/* CONFIG_MP3_TRACK_DEFUSE_SUCCESS: defuse-success announce id (maps to 0103) [43]
   Min: 1     Max: 999    Typical: 103 */
#define CONFIG_MP3_TRACK_DEFUSE_SUCCESS 103U
/* CONFIG_MP3_TRACK_CTWIN: CT-win announce id (maps to 0104) [44]
   Min: 1     Max: 999    Typical: 104 */
#define CONFIG_MP3_TRACK_CTWIN 104U
/* CONFIG_MP3_TRACK_TERWIN: T-win announce id (maps to 0105) [45]
   Min: 1     Max: 999    Typical: 105 */
#define CONFIG_MP3_TRACK_TERWIN 105U

/* CONFIG_MP3_DEFUSE_STAGE_ENABLE: defuse-success stage MP3 master toggle (0/1) [46]
   Min: 0     Max: 1      Typical: 1 */
#define CONFIG_MP3_DEFUSE_STAGE_ENABLE 1U
/* CONFIG_MP3_EXPLODED_STAGE_ENABLE: exploded stage MP3 master toggle (0/1) [47]
   Min: 0     Max: 1      Typical: 1 */
#define CONFIG_MP3_EXPLODED_STAGE_ENABLE 1U

/* CONFIG_MP3_EXPLOSION_ENABLE: play explosion effect in exploded stage (0/1) [48]
   Min: 0     Max: 1      Typical: 1 */
#define CONFIG_MP3_EXPLOSION_ENABLE 1U
/* CONFIG_MP3_CT_WIN_MUSICBOX_ENABLE: enable CT music-box stage after defuse (0/1) [49]
   Min: 0     Max: 1      Typical: 1 */
#define CONFIG_MP3_CT_WIN_MUSICBOX_ENABLE 1U
/* CONFIG_MP3_T_WIN_MUSICBOX_ENABLE: enable T music-box stage after explosion (0/1) [50]
   Min: 0     Max: 1      Typical: 1 */
#define CONFIG_MP3_T_WIN_MUSICBOX_ENABLE 1U
/* CONFIG_MP3_CTWIN_ENABLE: enable CT-win announcement (0/1) [51]
   Min: 0     Max: 1      Typical: 1 */
#define CONFIG_MP3_CTWIN_ENABLE 1U
/* CONFIG_MP3_TERWIN_ENABLE: enable T-win announcement (0/1) [52]
   Min: 0     Max: 1      Typical: 1 */
#define CONFIG_MP3_TERWIN_ENABLE 1U

/* CONFIG_MP3_CT_WIN_MUSICBOX_TRACK: CT music-box index [53]
   Min: 0     Max: 999    Typical: 10 */
#define CONFIG_MP3_CT_WIN_MUSICBOX_TRACK 10U
/* CONFIG_MP3_T_WIN_MUSICBOX_TRACK: T music-box index [54]
   Min: 0     Max: 999    Typical: 1 */
#define CONFIG_MP3_T_WIN_MUSICBOX_TRACK 1U

/* CONFIG_MP3_DEFUSE_SUCCESS_WAIT_MS: wait after defuse-success announcement (ms) [55]
   Min: 0     Max: 20000  Typical: 2700 */
#define CONFIG_MP3_DEFUSE_SUCCESS_WAIT_MS 2700U
/* CONFIG_MP3_CT_MUSICBOX_WAIT_MS: wait after CT music-box playback (ms) [56]
   Min: 0     Max: 20000  Typical: 9000 */
#define CONFIG_MP3_CT_MUSICBOX_WAIT_MS 9000U
/* CONFIG_MP3_T_MUSICBOX_WAIT_MS: wait after T music-box playback (ms) [57]
   Min: 0     Max: 20000  Typical: 10000 */
#define CONFIG_MP3_T_MUSICBOX_WAIT_MS 10000U
/* CONFIG_MP3_EXPLOSION_ONLY_WAIT_MS: wait after pure explosion playback (ms) [58]
   Min: 0     Max: 20000  Typical: 7000 */
#define CONFIG_MP3_EXPLOSION_ONLY_WAIT_MS 7000U

/* CONFIG_EXPLOSION_BEEP_MS: pre-explosion beep duration (ms) [59]
   Min: 0     Max: 5000   Typical: 500 */
#define CONFIG_EXPLOSION_BEEP_MS 500U

---

