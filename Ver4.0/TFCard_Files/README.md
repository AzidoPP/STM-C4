# TFCard_Files 说明（Ver4.0）

## 1) 当前目录结构

- **请将`TFCard_Files`整个文件夹内部所有文件复制与TF卡根目录后，插入MP3模块。**
- `MP3/`：唯一音频目录（位于 TF 卡根目录）
- `mp3module.md`：模块指令说明

> 按 `mp3module.md` 要求，按文件名播放必须放在根目录 `MP3` 文件夹下。

---

## 2) 文件命名规则（统一 4 位前缀）

### `0xxx`：播报与纯爆炸音效

- `0101.MP3`：下包成功（arm success）
- `0102.MP3`：纯爆炸音效（explosion only）
- `0103.MP3`：拆弹成功（bomb defused）
- `0104.MP3`：CT 胜利播报（ctwin）
- `0105.MP3`：T 胜利播报（terwin）

### `1xxx`：CT 音乐盒

- 规则：`0abc` → `1abc`
- 示例：`0001.MP3` → `1001.MP3`

### `2xxx`：T 爆炸+音乐盒

- 规则：`0abc` → `2abc`
- 示例：`0001.MP3` → `2001.MP3`

### 音乐序号

```
		音乐序号  音乐名
			0	虚空
			1	军团
			2	新黑色电影
			3	困兽
			4	神枪手
			5   	追溯起源
			6   	竞技场
			7	冲破藩篱
			8	咄咄逼人
			9	哥特浮华
			10	大胆尝试
			11	GLA
			12	漂泊者
			13	战火星空
			14	黄色魔法
			15	巨龙之眼
			16	咖啡拉花
			17	巴克拉姆
			18	电锯追命
			19	决心
			20	燥起来
			21	尘归尘
			22	A.D.8
			23	疤王
			24	万夫莫敌包
			25	终极
			26	大局入侵
			27	同型节奏
			28	地球末夜
			29	莫洛托夫烈火
			30	无人之境
			31	理由
			32	精彩时刻
			33	超爆话筒
			34	叛乱
			35	雄狮之口
			36	沙漠之焰
			37	好好干，好好活
			38	深红突击
			39	骷髅爆破
			40	绝对统御
			41	金属
			42	如日中天
			43	永恒之钻
			44	圣诞之歌
			45	你急了
			46	蜂鸟
			47	海绵手指
			48	8位音乐盒
			49	迈阿密热线
			50	令人发指
			51	躺平青年
			52	爆头
			53	闪光舞
			54	尖峰时刻
			55	塔罗斯的法则
			56	有害物质环境
			57	就是我
			58	EZ4ENCE
			59	爪哇哈瓦那放克乐
			60	触摸能量
			61	通宵达旦
			62	非人类
			63	花脸
			64	征服
			65	万众瞩目
			66	冲击星
			67	人生何处不青山
			68	枪炮卷饼卡车
			69	有为青年
			70	光环：士官长合集
			71	黑帝斯音乐盒
			72	反叛者
			73	Ben Bromfield,Rabbit Hole
			74	Daniel Sadowski,Dead Shot
			75	Sam Marshall,Clutch
			76	Tree Adams,Seventh Moon
			77	Tim Huling,Devil's Paintbrush
			78	Dren McDonald,Coffee!Kofe Kahveh!
			79	Matt Levine,Agency
			80	Austin Wintory,The Devil Went Clubbing in Georgia
			81	诶嘿
```

---

## 3) 编号与配置的关系（重点）

用户配置仍使用“三位编号”（`0~999`），不需要手动写 `1xxx/2xxx` 的前缀位：

- `CONFIG_MP3_CT_WIN_MUSICBOX_TRACK = n` 对应播放 `1nnn.MP3`
- `CONFIG_MP3_T_WIN_MUSICBOX_TRACK = n` 对应播放 `2nnn.MP3`

固定播报编号使用 `0xxx`：

- `CONFIG_MP3_TRACK_ARM_SUCCESS = 101`
- `CONFIG_MP3_TRACK_EXPLOSION = 102`
- `CONFIG_MP3_TRACK_DEFUSE_SUCCESS = 103`
- `CONFIG_MP3_TRACK_CTWIN = 104`
- `CONFIG_MP3_TRACK_TERWIN = 105`

---

## 4) 播放流程（代码当前实现）

1. 上电：不播放 MP3，仅蜂鸣（可开关）
2. 下包成功：可选播放 `0101`
3. 拆弹成功阶段：
   - 播放 `0103`
   - 等待 `CONFIG_MP3_DEFUSE_SUCCESS_WAIT_MS`
   - 可选播放 `1nnn`
   - 等待 `CONFIG_MP3_CT_MUSICBOX_WAIT_MS`（仅启用 CT 音乐盒时）
   - 可选播放 `0104`
4. 爆炸阶段：
   - 若启用 T 音乐盒：播放 `2nnn`，等待 `CONFIG_MP3_T_MUSICBOX_WAIT_MS`，再可选 `0105`
   - 否则播放 `0102`，等待 `CONFIG_MP3_EXPLOSION_ONLY_WAIT_MS`，再可选 `0105`

