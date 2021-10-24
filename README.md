# BIOP

## 2021 - 开发周志

| 周数 | 时间          | 进展情况                                                     |
| ---- | :------------ | ------------------------------------------------------------ |
| 1    | 8/11 - 8/17   | 提出优化 5、6、7，Rein时间分布测量                           |
| 2    | 8/18 - 8/24   | 提出 6 种模式，实现BIOPPS、BIOP2SD，实现 Rein 和 BIOP 的空属性匹配功能 |
| 3    | 8/25 - 8/31   | 实现 BIOP1SS，修改 BIOPPS、BIOP2SD，low、high 两端统一用同一套覆盖分区，做实验，数据处理 |
| 4    | 9/1 - 9/7     | 实现 BIOP3PS、BIOP4DS、BIOP5DD                               |
| 5    | 9/8 - 9/14    | 实现 BIOPSCDD，设置两种full Bitset，修复空桶带来的bug，修改订阅产生算法(热点区域) |
| 6    | 9/15 - 9/21   | 写论文 Design 章节大部分+修改，画图                          |
| 7    | 9/22 - 9/28   | 实现 BIOPSRPS，写论文 Introduction、State Reduction Optimization、Theoretical Analysis、Achievement |
| 8    | 9/29 - 10/05  | 写论文、Setup部分(找baseline参数未果)，实现BIOP5DD插入算法、两个暴力匹配Simple和Simple2算法，优化BIOPSRPS选择bitset策略 |
| 9    | 10/06 - 10/12 | 写论文Verification Experiment部分，跑实验，画图              |
| 10   | 10/13 - 10/19 | 实现Rein和BIOP5DD删除算法写论文Verification Experiment、Related Work部分，跑实验，画图，将Tama、AdaRein整合进项目，修改论文某些部分 |
| 11   | 10/20 - 10/26 | 代码重构，将OpIndex简化后整合进项目，调参跑完所有实验，画图，写论文实验部分  |

## Note

##### Windows 10 环境：Visual Studio 2022

1. 属性页，配置(C): 选择 "活动(Debug)" 或 "活动(Release)"，平台(P): 选择 "活动(x64)"
2. 配置属性 - C/C++ - 优化 - 优化，选择 "优化(优选速度) (/Ox)"
3. 配置属性 - C/C++ - 预处理器 - 预处理器定义，删掉里面的内容，加入一行 "_CRT_SECURE_NO_WARNINGS"
4. 配置属性 - C/C++ - 代码生成 - 基本运行时检查，改成 "默认值"
5. 属性页，配置(C): 选择 "活动(Debug)"，平台(P): 选择 "活动(x64)"
6. 配置属性 - 链接器 - 系统 - 四个大小，都填3221225472
7. 左上角的下拉框里进入配置管理器，活动解决方案配置为 "Debug"，下面的配置为 "Release"，勾选生成

##### ubuntu 20 环境
1. Run command "sudo cmake ."
2. Run command "sudo make"
3. Run command "sudo ./cmakeRB"

##### 注意事项
1. "constant.h" 里 MAX_SUBS、MAX_ATTS、MAX_BUCKS 必须大于等于 subs、atts、buks (Now it's obsoleted.)
2. In "constant.h", the constant `subs` can't be adjusted by input, means you have to run relative experiments one by one.

## 实验记录

* Default Parameter: n = 1**m**, d = 20, $S_s$ = 10, $S_e$ = 20, w = 0.3, subp = 0, uniform distribution.
* Model Parameter: 
  * Rein: b = 1000
  * BIOP5DD: b = 1000, be = 4
  * Tama: level = 13
  * AdaRein: falsePositiveRate = 10

| No. | 实验名| 非默认参数设置         | 在txt文件中的记录号（expID）                                                     |
| ---- | :-------|----- | ---------------- |
|1|基准验证实验(标记时间、匹配时间)|--|403 ~ 412, 421 ~ 430|
|2|验证实验(标记时间)|$S_e$=10|434 ~ 443|
|3|订阅数实验  | n = 300**k** ~ 9**m** | 484 ~ 489|
|4|订阅大小实验 | $S_s$ = 5 ~ 30, d = $S_e$ = 30, w = 0.7 |497 ~ 502|
|5|事件大小实验 |$S_e$ = 30 ~ 80, d = 80, subp = 0.5 | 503 ~ 508|
|6|订阅宽度实验 |w = 0.1 ~ 0.7, $S_s$ = 5| 517 ~ 523|
|7|维度实验| d = 30 ~ 900，subp = w = 0.5 | 528 ~ 533|
|8|事件订阅属性Zipf分布实验|alpha = 0 ~ 5, d = 50, $S_e$ = 20, w = 0.5| 538 ~ 543|
|9|综合性能实验|$S_s$ = 1 ~ 20, $S_e$ = 1 ~ 40, w = 0 ~ 1, d = 100, alpha = 1||
|10|构建时间、删除时间实验(即实验4)| $S_s$ = 5 ~ 30, d = $S_e$ = 30, w = 0.7                      ||
|11|内存实验(即实验3)|n = 300**k** ~ 9**m**|484 ~ 489|



