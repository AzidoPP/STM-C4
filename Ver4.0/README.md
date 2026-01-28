# STM-C4 Ver4.0 说明

主控 STM32F103C8T6，外部 8MHz 高速晶振，使用 Keil5 + ST-Link。

A CSGO game C4 bomb model based on STM32.

## 1. LCD1601A（替代 LCD1602A）
- LCD1601A 在板子上是倒置安装。
- 显示必须从右往左，字符需要整体旋转 180 度。
  - 例如用户看到左对齐的 `ABCDEF`，实际需要显示右对齐的 `FEDCBA`，并且每个字符旋转 180 度。
- 字符集：实际只需要倒置版的 `0123456789*`。
- 采用 4 线模式。

引脚定义（硬件连接）：
- RS: PB12
- EN: PB14
- D4~D7: PA4~PA7
- 背光：PA3（高电平点亮，推挽，PWM 控制）

电平/驱动：
- LCD1601A 使用 5V 电平，外部上拉到 5V。
- 信号脚使用开漏输出（OD）。

实现要点（简化说明）：
- 初始化流程与 HD44780 类似：上电延时、写入 0x03/0x02、功能设置、清屏、显示开等。
- 通过自定义 CGRAM 字库加载倒置字符，实际显示时将逻辑列号映射到物理列（右到左）。
- 参考实现见：`Ver4.0/Keil_proj/c4/hardware/1601a.c`

## 2. 矩阵键盘
- 引脚：PB15 PB4 PB5 PB6 PB7 PB8 PB9
- 排列：

```
        PB5     PB15    PB7
PB4     [1]     [2]     [3]
PB9     [4]     [5]     [6]
PB8     [7]     [8]     [9]
PB6     [*]     [0]     [#]
```

## 3. 蜂鸣器
- 无源蜂鸣器（含驱动电路，MOS 管，栅极 10k 下拉）。
- 栅极连接 PB0。
- PB0 输出 PWM 波驱动。

## 4. LED
- PC13：系统指示 LED（一般不用，仅调试，推挽高电平点亮）。
- 用户双色 LED（推挽高电平点亮）：
  - PA2 红
  - PA1 绿
- 黄色需要红绿同时点亮，并用 PWM 调节比例：
  - 黄色 100% 时，红色约 70%、绿色 100%（人眼对红色更敏感）。
  - 黄色 50% 时，红色约 35%、绿色 50%。

## 5. 继电器
- 已包含 MOS + 续流驱动。
- PB1 高电平：继电器吸合。

## 6. MP3 模块
- PA9 串口（USART1 TX），用于播放 C4 模型音效/音乐盒。
- MP3 命令封装参考：`Ver4.0/Keil_proj/c4/hardware/Serial.c` 与 `mp3.c`。
- 常用音效映射：
  - 上电提示音：`mp3_start()`
  - 下包完成：`mp3_over()`
  - 爆炸音效：`mp3_boom()`
  - 爆炸音乐盒：`mp3_boom_music()`

音乐序号（文件夹/曲目编号）：
```
0   虚空
1   军团
2   新黑色电影
3   困兽
4   神枪手
5   追溯起源
6   竞技场
7   冲破藩篱
8   咄咄逼人
9   哥特浮华
10  大胆尝试
11  GLA
12  漂泊者
13  战火星空
14  黄色魔法
15  巨龙之眼
16  咖啡拉花
17  巴克拉姆
18  电锯追命
19  决心
20  燥起来
21  尘归尘
22  A.D.8
23  疤王
24  万夫莫敌包
25  终极
26  大局入侵
27  同型节奏
28  地球末夜
29  莫洛托夫烈火
30  无人之境
31  理由
32  精彩时刻
33  超爆话筒
34  叛乱
35  雄狮之口
36  沙漠之焰
37  好好干，好好活
38  深红突击
39  骷髅爆破
40  绝对统御
41  金属
42  如日中天
43  永恒之钻
44  圣诞之歌
45  你急了
46  蜂鸟
47  海绵手指
48  8位音乐盒
49  迈阿密热线
50  令人发指
51  躺平青年
52  爆头
53  闪光舞
54  尖峰时刻
55  塔罗斯的法则
56  有害物质环境
57  就是我
58  EZ4ENCE
59  爪哇哈瓦那放克乐
60  触摸能量
61  通宵达旦
62  非人类
63  花脸
64  征服
65  万众瞩目
66  冲击星
67  人生何处不青山
68  枪炮卷饼卡车
69  有为青年
70  光环：士官长合集
71  黑帝斯音乐盒
72  反叛者
73  Ben Bromfield, Rabbit Hole
74  Daniel Sadowski, Dead Shot
75  Sam Marshall, Clutch
76  Tree Adams, Seventh Moon
77  Tim Huling, Devil's Paintbrush
78  Dren McDonald, Coffee!Kofe Kahveh!
79  Matt Levine, Agency
80  Austin Wintory, The Devil Went Clubbing in Georgia
81  诶嘿

82  c4 上电下包
83  c4 下包完成
84  c4 倒计时结束爆炸_完整
```

## 7. 外部“拆弹器”
- PB3 上拉输入，低电平有效。
- PB3 外部持续下拉 5 秒即判定拆弹成功。

## 流程（完整）
1. LED 先为黄色呼吸灯模式，周期 500ms。
2. 上电后快速哔哔两声（4250Hz）。
3. 倒计时蜂鸣器频率为 3600Hz。
4. 等待输入密码（7 位）。输入 7 位后，不直接进入倒计时，而是延迟一段时间，避免最后一位误触。
5. 记录该密码（输入密码期间 LED 仍为呼吸灯）。
6. 进入倒计时：
   - 按原版时间与 pattern 蜂鸣
   - 每次 beep 时 LED 红色，非 beep 时 LED 熄灭

## 倒计时蜂鸣模式
原 CS:GO C4 蜂鸣公式如下（BPS = Beeps Per Second）：

```
f(t) = 1.05 · exp(0.0054t + 0.000871t²)
```
- t：时间（秒），0 ≤ t ≤ 45

更通用的比例形式：
```
g(p) = 1.049 · exp(0.244p + 1.764p²)
```
- p：已过去时间比例，0.0 ≤ p ≤ 1.0

蜂鸣长度固定为 125ms。

代码泄露中的描述：
```
freq = MAX(0.1 + 0.9 * fComplete, 0.15)
```
- fComplete 为剩余时间比例（40/40 到 0/40）
- 下包后设置下一次蜂鸣时间：`gpGlobals->curtime + 1.0`

## 倒计时 LCD 滚动提示（旧实现示例）
原逻辑：使用 TIM2 中断每秒 20 次更新滚动位置。

```c
void TIM2_IRQHandler(void)//中断函数，每秒执行20次
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        if(Num_sign == 0)
        {
            Num ++;
            TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
            if(Num>=13){Num_sign=1;}
        }
        else
        {
            Num --;
            TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
            if(Num<=0){Num_sign=0;}
        }
    }
}
void lcdupdate(void)
{
    static int dt=0;
    static char lcdtp=0;
    dt++;
    if(dt>=30000)
    {
        switch(lcdtp)
        {
            case 0:
                LCD_WRITE_StrDATA("*            ",0);
                lcdtp++;
                break;
            case 1:
                LCD_WRITE_StrDATA(" *           ",0);
                lcdtp++;
                break;
            case 2:
                LCD_WRITE_StrDATA("  *          ",0);
                lcdtp++;
                break;
            case 3:
                LCD_WRITE_StrDATA("   *         ",0);
                lcdtp++;
                break;
            default:
                LCD_WRITE_StrDATA("    *        ",0);
                lcdtp=0;
                break;
        }
        dt=0;
    }
}
```

注意：LCD1601A 是倒装的，所以符号位置与显示方向需整体翻转 180°。

## 倒计时拆弹（3 种方式，且必须非阻塞）
> 拆弹失败或进行中都不应影响倒计时精度。旧版本因键盘扫描 + 倒计时循环阻塞，长按按键会导致消抖延时叠加、倒计时不准。V4.0 采用时间基准 + 状态机避免此问题。

1. **输入密码拆弹**：输入与下包相同密码即成功。
2. **长按 # 手动拆弹**：进入“破解手动拆弹模式”，持续 10 秒必成功。
   - 动画：每一位数字从 0~9 跳变 10 次后定格，再进入下一位；逐位“爆破”。
3. **外部拆弹器拆弹**：动画与手动拆弹相同，但速度快一倍，仅需 5 秒。

拆弹成功会播放成功音效。

## 爆炸行为
- 倒计时结束未拆弹：继电器吸合。
- 播放爆炸音效或音乐盒。
- LED 输出 100% 黄色（LED 与音乐盒同时）。

## 软件架构（文件职责）
- `Ver4.0/Keil_proj/c4/user/main.c`
  - 应用状态机与主循环（Idle/Countdown/Success/Exploded）
  - 倒计时逻辑、拆弹动画、输入处理
- `Ver4.0/Keil_proj/c4/sys/Systick.c`
  - 1ms 时间基准（TIM4 中断）
- `Ver4.0/Keil_proj/c4/user/stm32f10x_it.c`
  - TIM4 中断处理，驱动时间基准
- `Ver4.0/Keil_proj/c4/hardware/MatrixKey.c`
  - 矩阵键盘扫描与消抖
- `Ver4.0/Keil_proj/c4/hardware/1601a.c`
  - LCD1601A 驱动（倒装显示、倒置字符字库、背光 PWM）
- `Ver4.0/Keil_proj/c4/hardware/LED.c`
  - 红/绿 LED PWM、黄色配比、继电器控制
  - 蜂鸣器 PWM 频率与占空比控制
- `Ver4.0/Keil_proj/c4/hardware/Serial.c`
  - USART1 发送封装（供 MP3 模块使用）
- `Ver4.0/Keil_proj/c4/hardware/mp3.c`
  - MP3 指令封装
- `Ver4.0/Keil_proj/c4/sys/Delay.c`
  - 短时硬件延时（LCD 时序/少量效果）
- `Ver4.0/Keil_proj/c4/user/config.h`
  - 功能开关与参数配置
- `Ver4.0/Keil_proj/c4/user/config_range.h`
  - 配置范围检查

## 配置入口
核心参数集中在 `Ver4.0/Keil_proj/c4/user/config.h`：
- 密码长度、倒计时总时长、蜂鸣器频率
- 拆弹方式开关（密码/手动/外部）
- LCD 背光亮度、LED 黄色比例
- MP3 音量与音效开关

## 工程与编译
Keil 工程文件：`Ver4.0/Keil_proj/c4/Project.uvprojx`

## 备注
本工程混合了 V1.0（无 MP3，1602）/V2.0/V3.0 的旧代码与更新代码。
后续若要长期维护，建议整体梳理/重写一次。

## TODO
- 启用/关闭拆弹模式的配置化说明
- 更友好的 config 设置方式（例如直接设置倒计时时间再自动解算公式）
- 明确编译指令与构建流程文档
