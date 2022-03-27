# ReadMe

# 2021 - HEM 开发周志

| 周数 | 时间          | 进展情况                                                     |
| ---- | :------------ | ------------------------------------------------------------ |
| 1    | 8/11 - 8/17   | 提出优化 5、6、7，Rein时间分布测量                           |
| 2    | 8/18 - 8/24   | 提出 6 种模式，实现HEMPS、HEM2SD，实现 Rein 和 HEM 的空属性匹配功能 |
| 3    | 8/25 - 8/31   | 实现 HEM1SS，修改 HEMPS、HEM2SD，low、high 两端统一用同一套覆盖分区，做实验，数据处理 |
| 4    | 9/1 - 9/7     | 实现 HEM3PS、HEM4DS、HEM5DD                               |
| 5    | 9/8 - 9/14    | 实现 HEMSCDD，设置两种full Bitset，修复空桶带来的bug，修改订阅产生算法(热点区域) |
| 6    | 9/15 - 9/21   | 写论文 Design 章节大部分+修改，画图                          |
| 7    | 9/22 - 9/28   | 实现 HEMSRPS，写论文 Introduction、State Reduction Optimization、Theoretical Analysis、Achievement |
| 8    | 9/29 - 10/05  | 写论文、Setup部分(找baseline参数未果)，实现HEM5DD插入算法、两个暴力匹配Simple和Simple2算法，优化HEMSRPS选择bitset策略 |
| 9    | 10/06 - 10/12 | 写论文Verification Experiment部分，跑实验，画图              |
| 10   | 10/13 - 10/19 | 实现Rein和HEM5DD删除算法写论文Verification Experiment、Related Work部分，跑实验，画图，将Tama、AdaRein整合进项目，修改论文某些部分 |
| 11   | 10/20 - 10/26 | 代码重构，将OpIndex简化后整合进项目，调参跑完所有实验，画图，写论文实验部分  |
| 12   | 10/27 - 11/02 | 整体上修改论文，加入完整匹配例子  |
| 13   | 11/03 - 11/09 | 压缩论文，去掉两个优化，计数方法改为调用count函数，计数时间减少90%以上，重跑所有实验，重画图，重写实验部分，11-08提交论文 |
||||

## NOTE

#### Windows 10 环境：Visual Studio 2022

1. 属性页，配置(C): 选择 "活动(Debug)" 或 "活动(Release)"，平台(P): 选择 "活动(x64)"
2. 配置属性 - C/C++ - 优化 - 优化，选择 "优化(优选速度) (/Ox)"
3. 配置属性 - C/C++ - 预处理器 - 预处理器定义，删掉里面的内容，加入一行 "_CRT_SECURE_NO_WARNINGS"
4. 配置属性 - C/C++ - 代码生成 - 基本运行时检查，改成 "默认值"
5. 属性页，配置(C): 选择 "活动(Debug)"，平台(P): 选择 "活动(x64)"
6. 配置属性 - 链接器 - 系统 - 四个大小，都填3221225472
7. 左上角的下拉框里进入配置管理器，活动解决方案配置为 "Debug"，下面的配置为 "Release"，勾选生成

#### ubuntu 20 环境
1. Run command "sudo cmake ."
2. Run command "sudo make"
3. Run command "sudo cpufreq-set -c 20 -g performance" (调整第20号核为性能模式下运行)
4. sudo cpufreq-set -c 20 -d 3.6G
5. sudo cpufreq-set -c 20 -u 4.9G
6. Run command "ulimit -s unlimited; sudo taskset -c 20 ./run" (20指指定第20号核单独运行程序)

#### 注意事项
1. "constant.h" 里 MAX_SUBS、MAX_ATTS、MAX_BUCKS 必须大于等于 subs、atts、buks (Now it's obsoleted.)
2. In "constant.h", the constant `subs` can't be adjusted by input, means you have to run relative experiments one by one.

## 实验记录
* 真实数据预处理：https://www.aconvert.com/cn/document/xlsx-to-txt/

* Default Parameters: n = 1**m**, d = 20, $S_s$ = 10, $S_e$ = 20, w = 0.3, subp = 0, uniform distribution.
* Model Parameters: 
  * Rein: b = 1000
  * HEM5DD: b = 1000, be = 4
  * Tama: level = 13
  * AdaRein: falsePositiveRate = 10

| No. | 实验名| 非默认参数设置         | 在txt文件中的记录号（expID）                                                     |
| ---- | :-------|----- | ---------------- |
|1|基准验证实验(标记时间、匹配时间)|--|403 ~ 412, 421 ~ 430; 571 ~ 580, 681 ~ 690|
|2|空维度验证实验(标记时间)|$S_e$=10, p = 0.5|434 ~ 443; 593 ~ 602|
|3|订阅数实验  | n = 300**k** ~ 9**m**, 补: p = 0.5 | 484 ~ 489; 613 ~ 618|
|4|订阅大小实验 | $S_s$ = 5 ~ 30, d = $S_e$ = 30, w = 0.7, 补: p = 0.5 |497 ~ 502, 559 ~ 564; 620 ~ 625|
|5|事件大小实验 |$S_e$ = 30 ~ 80, d = 80, p = 0.5 | 503 ~ 508; 626 ~ 631|
|6|订阅宽度实验 |w = 0.1 ~ 0.9, $S_s$ = 5| 517 ~ 523; 646 ~ 654|
|7|维度实验| d = 30 ~ 900，p = w = 0.5 | 528 ~ 533; 657 ~ 662 |
|8(a)|事件订阅属性Zipf分布实验|alpha = 0 ~ 5, d = 50, w = 0.5| 538 ~ 543|
|8(b)|真实事件订阅属性Zipf分布实验|alpha = 0 ~ 5, d = 50, Ss = 5| 666 ~ 671|
|9|综合性能实验|$S_s$ = 1 ~ 20, $S_e$ = 1 ~ 40, w = 0 ~ 1, d = 100, alpha = 1|566, 567|
|10|构建时间、删除时间实验(即实验4)| $S_s$ = 5 ~ 30, d = $S_e$ = 30, w = 0.7, 补: p = 0.5  |559 ~ 564; 674 ~ 679|
|11|内存实验(即实验3)|n = 300**k** ~ 9**m**|484 ~ 489; 613 ~ 618|

#### Unresolved Error

* 实验9的匹配个数有错，两个Simple算法的结果是一样的，其他所有算法的结果是一样的，在500、10000个事件时两个平均匹配个数相差**都**少于1
* Why can setting p=0.5 increase the matching probability clearly in Exp 3 and 4 for real data set ?

# BG-Tree 开发月志

| 月      | 进展情况                                                     |
| ------- | :----------------------------------------------------------- |
| 2021-07 | 提出三层划分模型（0-0.5, 0.5-1.0, 跨0.5）                    |
| 2021-11 | 基于三层划分模型结合AWB+Tree提出新数据结构BGTree，做初步设计(讲PPT)与分析，实现初始化函数 |
| 2021-12 | 做插入匹配的数据结构详细设计1.0，实现插入删除函数，提出正向计数优化范式C-BOMP和方向匹配基本定理1 |
| 2022-01 | 提出将OpIndex、PS-Tree、C-BOMP、节点动态伸缩结合在一起的优化，提出多分割点优化(多叉树)，解剖BG-Tree做可视化分析，提出方向匹配基本定理2，做支持正反匹配的数据结构详细设计2.0 |
| 2022-02 | BG-Tree-d-vsr                                                             |
