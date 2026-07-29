# STM-C4 Ver4.0

<div align="right">
  <strong>English</strong> | <a href="README_zh.md">简体中文</a>
</div>

[![GitHub](https://img.shields.io/badge/GitHub-azidopp-181717?style=flat&logo=github&logoColor=white)](https://github.com/AzidoPP/STM-C4)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow?logo=opensourceinitiative&logoColor=white)](../LICENSE)
[![YouTube](https://img.shields.io/badge/YouTube-@lyyontop-FF0000?style=flat&logo=youtube&logoColor=white)](https://www.youtube.com/@lyyontop)
[![bilibili](https://img.shields.io/badge/bilibili-1084866085-00A1D6?style=flat&logo=bilibili&logoColor=white)](https://space.bilibili.com/1084866085)

An STM32F103C8T6-based CS2 C4 appearance model (V4.0). About 95% visual accuracy. Compared to the previous version, this revision optimizes the LCD ribbon width, uses multi-color LEDs, adds a passive buzzer for distinct tones, and introduces an external defuser option. The hardware integrates the power circuit and uses a PCB-mounted controller instead of a full dev board. On the software side, the countdown uses a more accurate formula, smoother animations, and more configurable options with a more authentic LCD effect.

GitHub repo (firmware/docs): https://github.com/AzidoPP/STM-C4  
OSHWHUB hardware page: https://oshwhub.com/azidopp/CS2-C4

### Real Picture

![Picture](../pic/model-V4-pic.png)

### Quick Start

1. PCB fabrication
   - Main board Gerber: [PCB_Gerber/Gerber_C4_MainBoardV4.1.zip](PCB_Gerber/Gerber_C4_MainBoardV4.1.zip)
   - LCD adapter Gerber: [PCB_Gerber/Gerber_LCD1601_AdapterBoardV4.0.zip](PCB_Gerber/Gerber_LCD1601_AdapterBoardV4.0.zip)
   - EasyEDA original project files (this repo): [EasyEDA_proj](EasyEDA_proj/)
   - See OSHWHUB for the full hardware [project files](https://oshwhub.com/azidopp/CS2-C4)
2. BOM purchase
   - BOM file: [BOM/BOM.xlsx](BOM/BOM.xlsx)
   - 9V alkaline batteries are recommended for more stable current output
3. Soldering & assembly
   - LCD1601 adapter soldering example:
     ![LCD1601 adapter pic1](../pic/pic1.png)
     ![LCD1601 adapter pic2](../pic/pic2.png)
     ![LCD1601 adapter pic3](../pic/adapterboard.jpg)
   - 3D printed shell/fillers (optional): see [3DP_Models](3DP_Models/)
   - Fully assembled board:
     ![Full assambled board](../pic/pic4.png)
4. Flash firmware
   - Keil project: [Keil_proj/c4](Keil_proj/c4/)
   - ST-Link V2 header location:
     ![ST-Link header pic3](../pic/pic3.png)
   - Pin order: 3.3V / SWDIO / SWCLK / GND
   - After flashing the firmware, if the LCD1601 shows no text, rotate the potentiometer in either direction to adjust the display contrast until the text is clear.
     ![potentiometer](../pic/potentiometer.png)
5. MP3 audio (optional)
   - Copy [TFCard_Files/](TFCard_Files/) contents to the TF card root
   - Details: [TFCard_Files](TFCard_Files/)
6. Configuration and tuning
   - User guide: [`CONFIG.md`](CONFIG.md)
   - Default configuration source: [`config.h`](Keil_proj/c4/user/config.h)
   - Supports single-item configuration update without reflashing (hold `#` during power-on)
   - Added digital countdown toggle: `CONFIG_DIGITAL_COUNTDOWN_ENABLE`

### Basic Gameplay

- Startup uses buzzer only (no MP3), while LED breathes in yellow.
- Enter any 7-digit code as the “plant” password.
- After entry, the countdown starts (default 40s, configurable); the LED and buzzer become more urgent.
- During the countdown, there are three defuse methods:
  - Password defuse: enter the same password again to defuse successfully.
  - Manual defuse: long press `#` to start; succeed after 10 seconds.
  - External defuser: when the external defuse input is active, hold for 5 seconds to succeed.
- Defuse-success sequence: defuse-success cue (separate toggle) -> (optional) CT music box -> (optional) CT win announcement.
- Explosion sequence:
  - With T music box enabled: play integrated explosion+music-box track -> (optional) T win announcement.
  - Without T music box: play pure explosion track -> (optional) T win announcement.

### Configuration

- User-facing configuration guide: [`CONFIG.md`](CONFIG.md)
- Technical notes: [`V4说明.md`](V4说明.md)
- Default configuration source: [`config.h`](Keil_proj/c4/user/config.h)

### Detailed Docs

For deeper details and theory, see the Chinese technical document: [V4说明.md](V4说明.md)

### Disclaimer

This project is a visual/electronic replica and does not have any real functionality or danger.  
Any modification, misuse, or illegal use is not associated with the author, and the author assumes no liability.
