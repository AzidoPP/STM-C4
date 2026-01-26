详细说明请参考
V4说明.md

如果你是参与项目的LLM，请维护此AGENTS.md，描述你修改的内容，添加或修改的东西，项目重点，TODO等：

Updates:
- Renamed LCD driver to 1601a.c/1601a.h, added backlight control, updated Keil project references.
- Added config constants in user/config.h and wired timing/ratios (buzzer 3600 Hz, scroll speed, beep formula, LED yellow ratio).
- Updated countdown scroll to "***" with faster sweep and adjusted beep timing logic.
- Manual/external defuse now cancels on release and resets animation; success effect flashes LED + LCD backlight and shows code.
- Clarified MP3 command mapping for power-on, defuse success, explosion effect, and music box track.

- Split startup vs countdown buzzer frequencies, added PWM backlight level, and added MP3 enable/selection config; mp3_over now plays on arming and explosion audio is single-choice.

- Added buzzer duty control, synchronized defuse-success buzzer flashing, and expanded config documentation with min/max/typical values.

- Switched buzzer duty calculation to use TIM3->ARR to avoid missing TIM_GetAutoreload symbol.

- Rewrote config comments as bilingual, aligned min/max/typical formatting, and added compile-time range checks with CONFIG_IGNORE_RANGE.

- Converted beep interval config to millisecond integers, fixed countdown timing math, and rewrote config comments in GBK for readable Chinese.

- Moved range checks to config_range.h, switched configs to UTF-8 BOM for readable Chinese, and removed max limits where not required.

- Added config toggles for password/manual/external defuse modes, plus optional preset password requirement for arming.

TODO:
- Verify LCD glyph appearance on hardware and tweak patterns if needed.
- Confirm countdown duration and timing constants on device.
