# STM-C4 Ver4.0

[![GitHub](https://img.shields.io/badge/GitHub-azidopp-181717?style=flat&logo=github&logoColor=white)](https://github.com/AzidoPP/STM-C4)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](../LICENSE)
[![YouTube](https://img.shields.io/badge/YouTube-@lyyontop-FF0000?style=flat&logo=youtube&logoColor=white)](https://www.youtube.com/@lyyontop)
[![bilibili](https://img.shields.io/badge/bilibili-1084866085-00A1D6?style=flat&logo=bilibili&logoColor=white)](https://space.bilibili.com/1084866085)

![pic1](./pic/pic1.png)
![pic2](./pic/pic2.png)
![pic3](./pic/pic3.png)

## 中文
基于 STM32F103C8T6 的 CS2 C4 外观模型（V4.0）。

GitHub 仓库（程序/文档）：https://github.com/AzidoPP/STM-C4  
OSHWHUB 硬件开源地址：待补充

[English](#english)

### 快速开始
1. PCB 打样
   - 主板 Gerber：[PCB_Gerber/Gerber_C4_MainBoardV4.1.zip](PCB_Gerber/Gerber_C4_MainBoardV4.1.zip)
   - LCD 转接板 Gerber：[PCB_Gerber/Gerber_LCD1601_AdapterBoardV4.0.zip](PCB_Gerber/Gerber_LCD1601_AdapterBoardV4.0.zip)
2. 采购 BOM
   - BOM 文件：[BOM/BOM.xlsx](BOM/BOM.xlsx)（或 `BOM/` 中最新版本）
   - 9V 电池建议使用碱性电池（输出电流更稳定）
3. 焊接与装配
   - LCD1601 转接板焊接示例：[pic/pic1.png](pic/pic1.png)、[pic/pic2.png](pic/pic2.png)
4. 烧录固件
   - Keil 工程：[Keil_proj/c4/Project.uvprojx](Keil_proj/c4/Project.uvprojx)
   - ST-Link V2 接口位置：[pic/pic3.png](pic/pic3.png)
   - 引脚顺序：3.3V / SWDIO / SWCLK / GND
5. MP3 音效（可选）
   - 将 [TFCard_Files/](TFCard_Files/) 内容拷贝到 TF 卡根目录
   - 详细说明见：[TFCard_Files/README.md](TFCard_Files/README.md)

### 资源与文件夹
- 源码工程：[Keil_proj/c4](Keil_proj/c4)
- 3D 模型：[3DP_Models](3DP_Models)
- PCB Gerber：[PCB_Gerber](PCB_Gerber)
- BOM：[BOM](BOM)
- 图片：[pic](pic)

### 免责声明
本项目为外观复刻模型，不具备任何真实功能或危险性。  
任何改装、滥用或非法使用与原作者无关，原作者不承担责任。

---

## English
An STM32F103C8T6-based CS2 C4 appearance model (V4.0).

GitHub repo (code/docs): https://github.com/AzidoPP/STM-C4  
OSHWHUB hardware link: TBD

### Quick Start
1. PCB fabrication
   - Main board Gerber: [PCB_Gerber/Gerber_C4_MainBoardV4.1.zip](PCB_Gerber/Gerber_C4_MainBoardV4.1.zip)
   - LCD adapter Gerber: [PCB_Gerber/Gerber_LCD1601_AdapterBoardV4.0.zip](PCB_Gerber/Gerber_LCD1601_AdapterBoardV4.0.zip)
2. BOM
   - BOM file: [BOM/BOM.xlsx](BOM/BOM.xlsx) (or the latest file in `BOM/`)
   - Use an alkaline 9V battery for stable output current
3. Soldering & assembly
   - LCD1601 adapter examples: [pic/pic1.png](pic/pic1.png), [pic/pic2.png](pic/pic2.png)
4. Firmware flashing
   - Keil project: [Keil_proj/c4/Project.uvprojx](Keil_proj/c4/Project.uvprojx)
   - ST-Link V2 header location: [pic/pic3.png](pic/pic3.png)
   - Pin order: 3.3V / SWDIO / SWCLK / GND
5. MP3 audio (optional)
   - Copy [TFCard_Files/](TFCard_Files/) to the TF card root
   - See: [TFCard_Files/README.md](TFCard_Files/README.md)

### Resources
- Source: [Keil_proj/c4](Keil_proj/c4)
- 3D models: [3DP_Models](3DP_Models)
- PCB Gerbers: [PCB_Gerber](PCB_Gerber)
- BOM: [BOM](BOM)
- Images: [pic](pic)

### Disclaimer
This project is a replica appearance model and has no real functionality or hazardous capability.  
Any modification, misuse, or illegal use is not associated with the original creator.
