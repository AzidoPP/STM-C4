详细说明请参考
V4说明.md

如果你是参与项目的LLM，请维护此AGENTS.md，描述你修改的内容，添加或修改的东西，项目重点，TODO等：

Updates:
- Clarified 3DP_Models readme wording and link to the model provider for print requests in CN/EN.
- Reformatted `Ver4.0/3DP_Models/readme.md` with clearer wording and bilingual (CN/EN) sections.
- Updated root README with V4 image, V1 EOL notice + link to Ver4.0, and bilingual intro with English jump anchor.
- Added LCD normal-mount config and moved orientation handling (column mapping + glyph rotation) into the 1601a driver so app logic stays logical-only.
- Reworked countdown scroll rendering to draw stars at i-1/i/i+1 within bounds using a single moving position.
- Added symmetric scroll blur ramping at both edges and a 3-beep defuse-success tone using the startup frequency.
- Adjusted countdown scroll range to 0..mid screen with edge-length ramping (* / ** / ***) and reworked defuse-success to blink stars vs code without backlight flashing.
- Added password-specific LCD write path to avoid CGRAM slot reuse flicker during defuse animation.
- Rewrote Ver4.0/README.md to fully include V4说明.md content with normalized language, condensed code, and a clear architecture overview.
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
- When preset arm password is enabled, a wrong entry now clears the input back to stars.
- Normalized config toggle comments (explicit 1=enable) and kept preset password length check as a typedef guard for Keil compatibility.

TODO:
- Verify LCD glyph appearance on hardware and tweak patterns if needed.
- Confirm countdown duration and timing constants on device.
