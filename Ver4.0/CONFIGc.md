# Ver4.0 配置说明 / Configuration Guide

## 中文（教程 + 完整配置）

### 快速教程（不重新下载程序也能改配置）
1. 先按住 `#` 不放。
2. 在按住 `#` 的同时，按一下 `RESET`（或重新上电）。
3. 继续按住 `#` 约 1 秒，直到屏幕进入配置界面。

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
- 把 `CONFIG_LONG_PRESS_MS`（编号 `24`）改成 `500`：进入配置模式 -> 输入 `24` -> `#` -> 输入 `500` -> `#` -> `#` 保存 -> `RESET`。

### 配置总表（中文）
| 编号 | 配置名 | 说明 | 默认值 | 范围 / 选项（典型） |
|---:|---|---|---|---|
| 0 | `CONFIG_IGNORE_RANGE` | 是否忽略范围检查 | `0` | `0/1` |
| 1 | `CONFIG_PASSWORD_LEN` | 密码长度（位） | `7` | `1..16`（典型 `7`） |
| 2 | `CONFIG_LCD_COLS` | LCD 列数 | `16` | `8..16`（典型 `16`） |
| 3 | `CONFIG_LCD_NORMAL_MOUNT` | LCD 安装方向 | `0` | `0=倒装, 1=正装` |
| 4 | `CONFIG_PASSWORD_COL` | 密码起始列 | `3` | `0..(LCD_COLS-PASSWORD_LEN)`（典型 `3`） |
| 5 | `CONFIG_COUNTDOWN_MS` | 倒计时毫秒数 | `15000` | `>=1000`（典型 `15000`） |
| 6 | `CONFIG_ARM_DELAY_MS` | 下包前最后按键延迟（毫秒） | `800` | `0..5000`（典型 `800`） |
| 7 | `CONFIG_ARM_PRESET_ENABLE` | 是否启用固定下包密码 | `0` | `0/1` |
| 8 | `CONFIG_ARM_PRESET_PASSWORD` | 固定下包密码 | `1234567` | 仅数字；启用固定密码时长度必须等于密码长度 |
| 9 | `CONFIG_DEFUSE_ENABLE_PASSWORD` | 是否启用密码拆弹 | `1` | `0/1` |
| 10 | `CONFIG_DEFUSE_ENABLE_MANUAL` | 是否启用长按 `#` 拆弹 | `1` | `0/1` |
| 11 | `CONFIG_DEFUSE_ENABLE_EXTERNAL` | 是否启用外部拆弹器 | `1` | `0/1` |
| 12 | `CONFIG_LCD_BACKLIGHT_PCT` | LCD 背光亮度百分比 | `50` | `0..100`（典型 `50`） |
| 13 | `CONFIG_BEEP_LEN_MS` | 倒计时每次蜂鸣时长（毫秒） | `125` | `10..500`（典型 `125`） |
| 14 | `CONFIG_BEEP_INTERVAL_BASE_MS` | 蜂鸣基础间隔（毫秒） | `100` | `10..1000`（典型 `100`） |
| 15 | `CONFIG_BEEP_INTERVAL_SCALE_MS` | 蜂鸣间隔缩放（毫秒） | `900` | `0..2000`（典型 `900`） |
| 16 | `CONFIG_BEEP_INTERVAL_MIN_MS` | 蜂鸣最小间隔（毫秒） | `150` | `50..1000`（典型 `150`） |
| 17 | `CONFIG_BEEP_INITIAL_MS` | 首次蜂鸣延迟（毫秒） | `1000` | `0..5000`（典型 `1000`） |
| 18 | `CONFIG_LED_PWM_MAX` | LED 最大亮度级别 | `1000` | `100..10000`（典型 `1000`） |
| 19 | `CONFIG_LED_BREATH_PERIOD_MS` | 待机呼吸周期（毫秒） | `700` | `100..5000`（典型 `700`） |
| 20 | `CONFIG_LED_YELLOW_RED_PCT` | 黄色中红灯比例（百分比） | `40` | `0..100`（典型 `40`） |
| 21 | `CONFIG_SCROLL_INTERVAL_MS` | 滚动动画刷新间隔（毫秒） | `50` | `10..500`（典型 `50`） |
| 22 | `CONFIG_SCROLL_PATTERN_LEN` | 滚动图案长度 | `3` | `1..16`（典型 `3`） |
| 23 | `CONFIG_DIGITAL_COUNTDOWN_ENABLE` | 是否显示数字倒计时 | `0` | `0/1` |
| 24 | `CONFIG_LONG_PRESS_MS` | 长按阈值（毫秒） | `200` | `200..3000`（典型 `200`） |
| 25 | `CONFIG_MANUAL_DEFUSE_MS` | 手动拆弹总时长（毫秒） | `10000` | `>=1000`（典型 `10000`） |
| 26 | `CONFIG_EXTERNAL_DEFUSE_MS` | 外部拆弹总时长（毫秒） | `5000` | `>=1000`（典型 `5000`） |
| 27 | `CONFIG_DEFUSE_DISPLAY_HOLD_MS` | 拆弹显示保持时长（毫秒） | `1200` | `200..5000`（典型 `1200`） |
| 28 | `CONFIG_DEFUSE_CYCLE_STEPS` | 拆弹循环步数 | `10` | `1..20`（典型 `10`） |
| 29 | `CONFIG_DEFUSE_FLASH_TOGGLE_MS` | 拆弹成功闪烁切换间隔（毫秒） | `10` | `10..200`（典型 `10`） |
| 30 | `CONFIG_DEFUSE_FLASH_TOGGLES` | 拆弹成功闪烁次数 | `8` | `1..20`（典型 `8`） |
| 31 | `CONFIG_DEFUSE_BLINK_MS` | 拆弹成功密码/星号切换间隔（毫秒） | `400` | `100..1000`（典型 `400`） |
| 32 | `CONFIG_BUZZER_STARTUP_FREQ_HZ` | 上电蜂鸣频率（Hz） | `4250` | `100..10000`（典型 `4250`） |
| 33 | `CONFIG_BUZZER_COUNTDOWN_FREQ_HZ` | 倒计时蜂鸣频率（Hz） | `3600` | `100..10000`（典型 `3600`） |
| 34 | `CONFIG_BUZZER_DUTY_PCT` | 蜂鸣占空比（百分比） | `50` | `0..50`（典型 `50`） |
| 35 | `CONFIG_STARTUP_BEEP_ON_MS` | 上电蜂鸣开启时长（毫秒） | `50` | `10..500`（典型 `50`） |
| 36 | `CONFIG_STARTUP_BEEP_GAP_MS` | 上电蜂鸣间隔时长（毫秒） | `50` | `10..500`（典型 `50`） |
| 37 | `CONFIG_STARTUP_BEEP_ENABLE` | 是否启用上电蜂鸣 | `1` | `0/1` |
| 38 | `CONFIG_MP3_VOLUME` | MP3 音量 | `25` | `0..30`（典型 `25`） |
| 39 | `CONFIG_MP3_ARM_SUCCESS_ENABLE` | 下包成功播报开关 | `1` | `0/1` |
| 40 | `CONFIG_MP3_DEFUSE_SUCCESS_ENABLE` | 拆弹成功播报开关 | `1` | `0/1` |
| 41 | `CONFIG_MP3_TRACK_ARM_SUCCESS` | 下包成功播报号 | `101` | `1..999`（典型 `101`） |
| 42 | `CONFIG_MP3_TRACK_EXPLOSION` | 爆炸音效号 | `102` | `1..999`（典型 `102`） |
| 43 | `CONFIG_MP3_TRACK_DEFUSE_SUCCESS` | 拆弹成功播报号 | `103` | `1..999`（典型 `103`） |
| 44 | `CONFIG_MP3_TRACK_CTWIN` | CT 胜利播报号 | `104` | `1..999`（典型 `104`） |
| 45 | `CONFIG_MP3_TRACK_TERWIN` | T 胜利播报号 | `105` | `1..999`（典型 `105`） |
| 46 | `CONFIG_MP3_DEFUSE_STAGE_ENABLE` | 拆弹成功阶段 MP3 总开关 | `1` | `0/1` |
| 47 | `CONFIG_MP3_EXPLODED_STAGE_ENABLE` | 爆炸阶段 MP3 总开关 | `1` | `0/1` |
| 48 | `CONFIG_MP3_EXPLOSION_ENABLE` | 爆炸音效开关 | `1` | `0/1` |
| 49 | `CONFIG_MP3_CT_WIN_MUSICBOX_ENABLE` | CT 音乐盒开关 | `1` | `0/1` |
| 50 | `CONFIG_MP3_T_WIN_MUSICBOX_ENABLE` | T 音乐盒开关 | `1` | `0/1` |
| 51 | `CONFIG_MP3_CTWIN_ENABLE` | CT 胜利播报开关 | `1` | `0/1` |
| 52 | `CONFIG_MP3_TERWIN_ENABLE` | T 胜利播报开关 | `1` | `0/1` |
| 53 | `CONFIG_MP3_CT_WIN_MUSICBOX_TRACK` | CT 音乐盒编号 | `10` | `0..999`（典型 `10`） |
| 54 | `CONFIG_MP3_T_WIN_MUSICBOX_TRACK` | T 音乐盒编号 | `1` | `0..999`（典型 `1`） |
| 55 | `CONFIG_MP3_DEFUSE_SUCCESS_WAIT_MS` | 拆弹播报后等待时长（毫秒） | `2700` | `0..20000`（典型 `2700`） |
| 56 | `CONFIG_MP3_CT_MUSICBOX_WAIT_MS` | CT 音乐盒后等待时长（毫秒） | `9000` | `0..20000`（典型 `9000`） |
| 57 | `CONFIG_MP3_T_MUSICBOX_WAIT_MS` | T 音乐盒后等待时长（毫秒） | `10000` | `0..20000`（典型 `10000`） |
| 58 | `CONFIG_MP3_EXPLOSION_ONLY_WAIT_MS` | 爆炸音效后等待时长（毫秒） | `7000` | `0..20000`（典型 `7000`） |
| 59 | `CONFIG_EXPLOSION_BEEP_MS` | 倒计时结束后、爆炸流程前的预蜂鸣时长（毫秒） | `500` | `0..5000`（典型 `500`） |
| 60 | `CONFIG_ARM_ERROR_HINT_ENABLE` | 固定下包密码错误时是否显示 `ERROR` | `0` | `0=静默清空, 1=显示ERROR后清空` |

---

## English (Tutorial + Full Config List)

### Quick Tutorial (change settings without re-uploading code)
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

### Keys
- `0~9`: input digits
- `*`: delete last digit
- `#`: confirm current step

### Restore factory settings
- Enter `090405`, then press `#` to confirm restore.

### Example
- Set `CONFIG_LONG_PRESS_MS` (ID `24`) to `500`: enter config mode -> `24` -> `#` -> `500` -> `#` -> `#` save -> `RESET`.

### Full Config Table (English)
| ID | Name | Description | Default | Range / Options (Typical) |
|---:|---|---|---|---|
| 0 | `CONFIG_IGNORE_RANGE` | Ignore range checking | `0` | `0/1` |
| 1 | `CONFIG_PASSWORD_LEN` | Password length (digits) | `7` | `1..16` (typical `7`) |
| 2 | `CONFIG_LCD_COLS` | LCD columns | `16` | `8..16` (typical `16`) |
| 3 | `CONFIG_LCD_NORMAL_MOUNT` | LCD mount orientation | `0` | `0=inverted, 1=normal` |
| 4 | `CONFIG_PASSWORD_COL` | Password start column | `3` | `0..(LCD_COLS-PASSWORD_LEN)` (typical `3`) |
| 5 | `CONFIG_COUNTDOWN_MS` | Countdown duration (ms) | `15000` | `>=1000` (typical `15000`) |
| 6 | `CONFIG_ARM_DELAY_MS` | Delay after last key before arming (ms) | `800` | `0..5000` (typical `800`) |
| 7 | `CONFIG_ARM_PRESET_ENABLE` | Require preset password to arm | `0` | `0/1` |
| 8 | `CONFIG_ARM_PRESET_PASSWORD` | Preset arm password | `1234567` | Digits only; if preset enabled, length must equal password length |
| 9 | `CONFIG_DEFUSE_ENABLE_PASSWORD` | Enable password defuse | `1` | `0/1` |
| 10 | `CONFIG_DEFUSE_ENABLE_MANUAL` | Enable long-press `#` defuse | `1` | `0/1` |
| 11 | `CONFIG_DEFUSE_ENABLE_EXTERNAL` | Enable external defuser | `1` | `0/1` |
| 12 | `CONFIG_LCD_BACKLIGHT_PCT` | LCD backlight percent | `50` | `0..100` (typical `50`) |
| 13 | `CONFIG_BEEP_LEN_MS` | Countdown beep ON length (ms) | `125` | `10..500` (typical `125`) |
| 14 | `CONFIG_BEEP_INTERVAL_BASE_MS` | Base beep interval (ms) | `100` | `10..1000` (typical `100`) |
| 15 | `CONFIG_BEEP_INTERVAL_SCALE_MS` | Beep interval scaling (ms) | `900` | `0..2000` (typical `900`) |
| 16 | `CONFIG_BEEP_INTERVAL_MIN_MS` | Minimum beep interval (ms) | `150` | `50..1000` (typical `150`) |
| 17 | `CONFIG_BEEP_INITIAL_MS` | Initial beep delay (ms) | `1000` | `0..5000` (typical `1000`) |
| 18 | `CONFIG_LED_PWM_MAX` | LED max brightness level | `1000` | `100..10000` (typical `1000`) |
| 19 | `CONFIG_LED_BREATH_PERIOD_MS` | Idle breathing period (ms) | `700` | `100..5000` (typical `700`) |
| 20 | `CONFIG_LED_YELLOW_RED_PCT` | Red ratio in yellow color (%) | `40` | `0..100` (typical `40`) |
| 21 | `CONFIG_SCROLL_INTERVAL_MS` | Scroll animation update interval (ms) | `50` | `10..500` (typical `50`) |
| 22 | `CONFIG_SCROLL_PATTERN_LEN` | Scroll pattern length | `3` | `1..16` (typical `3`) |
| 23 | `CONFIG_DIGITAL_COUNTDOWN_ENABLE` | Enable digital countdown display | `0` | `0/1` |
| 24 | `CONFIG_LONG_PRESS_MS` | Long-press threshold (ms) | `200` | `200..3000` (typical `200`) |
| 25 | `CONFIG_MANUAL_DEFUSE_MS` | Manual defuse total duration (ms) | `10000` | `>=1000` (typical `10000`) |
| 26 | `CONFIG_EXTERNAL_DEFUSE_MS` | External defuse total duration (ms) | `5000` | `>=1000` (typical `5000`) |
| 27 | `CONFIG_DEFUSE_DISPLAY_HOLD_MS` | Defuse display hold time (ms) | `1200` | `200..5000` (typical `1200`) |
| 28 | `CONFIG_DEFUSE_CYCLE_STEPS` | Defuse cycle steps | `10` | `1..20` (typical `10`) |
| 29 | `CONFIG_DEFUSE_FLASH_TOGGLE_MS` | Defuse-success flash toggle interval (ms) | `10` | `10..200` (typical `10`) |
| 30 | `CONFIG_DEFUSE_FLASH_TOGGLES` | Defuse-success flash toggle count | `8` | `1..20` (typical `8`) |
| 31 | `CONFIG_DEFUSE_BLINK_MS` | Defuse-success code/star swap interval (ms) | `400` | `100..1000` (typical `400`) |
| 32 | `CONFIG_BUZZER_STARTUP_FREQ_HZ` | Startup buzzer frequency (Hz) | `4250` | `100..10000` (typical `4250`) |
| 33 | `CONFIG_BUZZER_COUNTDOWN_FREQ_HZ` | Countdown buzzer frequency (Hz) | `3600` | `100..10000` (typical `3600`) |
| 34 | `CONFIG_BUZZER_DUTY_PCT` | Buzzer duty cycle (%) | `50` | `0..50` (typical `50`) |
| 35 | `CONFIG_STARTUP_BEEP_ON_MS` | Startup beep ON time (ms) | `50` | `10..500` (typical `50`) |
| 36 | `CONFIG_STARTUP_BEEP_GAP_MS` | Startup beep gap time (ms) | `50` | `10..500` (typical `50`) |
| 37 | `CONFIG_STARTUP_BEEP_ENABLE` | Enable startup beeps | `1` | `0/1` |
| 38 | `CONFIG_MP3_VOLUME` | MP3 volume | `25` | `0..30` (typical `25`) |
| 39 | `CONFIG_MP3_ARM_SUCCESS_ENABLE` | Arm-success announcement enable | `1` | `0/1` |
| 40 | `CONFIG_MP3_DEFUSE_SUCCESS_ENABLE` | Defuse-success announcement enable | `1` | `0/1` |
| 41 | `CONFIG_MP3_TRACK_ARM_SUCCESS` | Arm-success track number | `101` | `1..999` (typical `101`) |
| 42 | `CONFIG_MP3_TRACK_EXPLOSION` | Explosion track number | `102` | `1..999` (typical `102`) |
| 43 | `CONFIG_MP3_TRACK_DEFUSE_SUCCESS` | Defuse-success track number | `103` | `1..999` (typical `103`) |
| 44 | `CONFIG_MP3_TRACK_CTWIN` | CT-win track number | `104` | `1..999` (typical `104`) |
| 45 | `CONFIG_MP3_TRACK_TERWIN` | T-win track number | `105` | `1..999` (typical `105`) |
| 46 | `CONFIG_MP3_DEFUSE_STAGE_ENABLE` | MP3 master switch in defuse-success stage | `1` | `0/1` |
| 47 | `CONFIG_MP3_EXPLODED_STAGE_ENABLE` | MP3 master switch in explosion stage | `1` | `0/1` |
| 48 | `CONFIG_MP3_EXPLOSION_ENABLE` | Explosion sound effect enable | `1` | `0/1` |
| 49 | `CONFIG_MP3_CT_WIN_MUSICBOX_ENABLE` | CT music-box enable | `1` | `0/1` |
| 50 | `CONFIG_MP3_T_WIN_MUSICBOX_ENABLE` | T music-box enable | `1` | `0/1` |
| 51 | `CONFIG_MP3_CTWIN_ENABLE` | CT-win announcement enable | `1` | `0/1` |
| 52 | `CONFIG_MP3_TERWIN_ENABLE` | T-win announcement enable | `1` | `0/1` |
| 53 | `CONFIG_MP3_CT_WIN_MUSICBOX_TRACK` | CT music-box track number | `10` | `0..999` (typical `10`) |
| 54 | `CONFIG_MP3_T_WIN_MUSICBOX_TRACK` | T music-box track number | `1` | `0..999` (typical `1`) |
| 55 | `CONFIG_MP3_DEFUSE_SUCCESS_WAIT_MS` | Wait after defuse announcement (ms) | `2700` | `0..20000` (typical `2700`) |
| 56 | `CONFIG_MP3_CT_MUSICBOX_WAIT_MS` | Wait after CT music-box (ms) | `9000` | `0..20000` (typical `9000`) |
| 57 | `CONFIG_MP3_T_MUSICBOX_WAIT_MS` | Wait after T music-box (ms) | `10000` | `0..20000` (typical `10000`) |
| 58 | `CONFIG_MP3_EXPLOSION_ONLY_WAIT_MS` | Wait after explosion-only sound (ms) | `7000` | `0..20000` (typical `7000`) |
| 59 | `CONFIG_EXPLOSION_BEEP_MS` | Pre-explosion beep duration (ms) | `500` | `0..5000` (typical `500`) |
| 60 | `CONFIG_ARM_ERROR_HINT_ENABLE` | Show `ERROR` when preset arm password is wrong | `0` | `0=silent clear, 1=show ERROR then clear` |

