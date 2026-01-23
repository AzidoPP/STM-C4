详细说明请参考
V4说明.md

如果你是参与项目的LLM，请维护此AGENTS.md，描述你修改的内容，添加或修改的东西，项目重点，TODO等：

Updates:
- Rebuilt LCD driver for 1601A 4-bit mode with reversed columns and custom glyph cache for digits/*.
- Added TIM4 1ms timebase, non-blocking keypad debounce, LED PWM (PA1/PA2), buzzer PWM (PB0), relay (PB1), defuser input (PB3).
- Rewrote main state machine for idle/arm/countdown/defuse/explosion, beep pattern timing, scroll star, and defuse animations.
- Added Serial/mp3 drivers and OLED/1602a4 stubs; updated Keil project file lists.

TODO:
- Verify LCD glyph appearance on hardware and tweak patterns if needed.
- Confirm countdown duration and timing constants on device.
