# STM-C4 Ver4.0

<div align="right">
  <a href="README.md">English</a> | <strong>简体中文</strong>
</div>

[![GitHub](https://img.shields.io/badge/GitHub-azidopp-181717?style=flat&logo=github&logoColor=white)](https://github.com/AzidoPP/STM-C4)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow?logo=opensourceinitiative&logoColor=white)](../LICENSE)
[![YouTube](https://img.shields.io/badge/YouTube-@lyyontop-FF0000?style=flat&logo=youtube&logoColor=white)](https://www.youtube.com/@lyyontop)
[![bilibili](https://img.shields.io/badge/bilibili-1084866085-00A1D6?style=flat&logo=bilibili&logoColor=white)](https://space.bilibili.com/1084866085)

基于 STM32F103C8T6 的 CS2 C4 光电模型，第 4 版。还原度约 95%，相较于先前版本，优化了显示屏排线宽度，使用多色 LED，使用无源蜂鸣器（可以发出不同音调），添加了“拆弹器”支持。硬件方面使用了集成的电源电路，以及集成在 PCB 上的控制器取代原先的整个开发板。软件方面改进：倒计时使用了更还原的公式，修改了更好的动画，添加了更多可配置选项，使用了更还原的显示屏动画。

GitHub 仓库（程序/文档）：https://github.com/AzidoPP/STM-C4  
OSHWHUB 硬件开源地址：https://oshwhub.com/azidopp/CS2-C4

### 实物图

![Picture](../pic/model-V4-pic.png)

### 快速开始

1. PCB 打样
   - 主板 Gerber：[PCB_Gerber/Gerber_C4_MainBoardV4.1.zip](PCB_Gerber/Gerber_C4_MainBoardV4.1.zip)
   - LCD 转接板 Gerber：[PCB_Gerber/Gerber_LCD1601_AdapterBoardV4.0.zip](PCB_Gerber/Gerber_LCD1601_AdapterBoardV4.0.zip)
   - 立创 EDA 原始工程文件（本仓库）：[EasyEDA_proj](EasyEDA_proj/)
   - 详细工程请参考 OSHWHUB 开源硬件平台[工程](https://oshwhub.com/azidopp/CS2-C4)
2. 采购 BOM
   - BOM 文件：[BOM/BOM.xlsx](BOM/BOM.xlsx)
   - 9V 电池建议使用碱性电池（输出电流更稳定）
3. 焊接与装配
   - LCD1601 转接板焊接示例：
     ![LCD1601 adapter pic1](../pic/pic1.png)
     ![LCD1601 adapter pic2](../pic/pic2.png)
     ![LCD1601 adapter pic3](../pic/adapterboard.jpg)
   - 3D 打印外壳/填充件（可选）：见 [3DP_Models](3DP_Models/)
   - 完整组装：
     ![Full assambled board](../pic/pic4.png)
4. 烧录固件
   - Keil 工程：[Keil_proj/c4](Keil_proj/c4/)
   - ST-Link V2 接口位置：
     ![ST-Link header pic3](../pic/pic3.png)
   - 引脚顺序：3.3V / SWDIO / SWCLK / GND
   - 烧录固件后，如果 LCD1601 显示屏不显示，请旋转电位器调整屏幕对比度，左右旋转，直到出现清晰的文字。
     ![potentiometer](../pic/potentiometer.png)
5. MP3 音效（可选）
   - 将 [TFCard_Files/](TFCard_Files/) 内容复制到 TF 卡根目录
   - 详细说明见：[TFCard_Files](TFCard_Files/)
6. 配置与修改
   - 用户配置说明：[`CONFIG.md`](CONFIG.md)
   - 默认配置定义文件：[`Keil_proj/c4/user/config.h`](Keil_proj/c4/user/config.h)
   - 可在不重新烧录的情况下修改单个配置（上电时按住 `#` 进入配置模式）
   - 新增倒计时数字显示开关：`CONFIG_DIGITAL_COUNTDOWN_ENABLE`

### 基础玩法

- 上电阶段仅蜂鸣器提示（不播放 MP3），LED 为黄色呼吸灯模式。
- 输入任意 7 位密码，作为“下包密码”。
- 输入完成后进入倒计时（默认 40 秒，可配置），期间 LED 和蜂鸣器会逐渐急迫。
- 倒计时期间有三种拆弹方式：
  - 输入拆弹：再次输入与下包一致的密码即可拆弹成功。
  - 手动拆弹：长按 `#` 开始拆弹，持续 10 秒后成功。
  - 外部拆弹器：外部拆弹输入有效时开始拆弹，持续 5 秒即成功。
- 拆弹成功播放序列：拆弹成功音（可单独开关）->（可选）CT 音乐盒 ->（可选）CT 胜利播报。
- 倒计时结束未拆弹播放序列：
  - 启用 T 音乐盒：播放“爆炸+T 音乐盒”整合音频 ->（可选）T 胜利播报。
  - 未启用 T 音乐盒：播放纯爆炸音效 ->（可选）T 胜利播报。

### 配置

- 用户配置指南（含编号与操作步骤）：[`CONFIG.md`](CONFIG.md)
- 默认配置定义文件：[`Keil_proj/c4/user/config.h`](Keil_proj/c4/user/config.h)
- 技术说明：[`V4说明.md`](V4说明.md)

### 详细说明

更多细节与原理说明请查看：[V4说明.md](V4说明.md)

### 免责声明

本项目为外观光电复刻模型，不具备任何真实功能或危险性。  
任何改装、滥用或非法使用与原作者无关，原作者不承担责任。
