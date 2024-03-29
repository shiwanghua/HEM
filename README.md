# ReadMe

# 2021 - HEM 开发周志

| 周数 | 时间          | 进展情况                                                                                                                                   |
| ---- | :------------ | ------------------------------------------------------------------------------------------------------------------------------------------ |
| 1    | 8/11 - 8/17   | 提出优化 5、6、7，Rein 时间分布测量                                                                                                        |
| 2    | 8/18 - 8/24   | 提出 6 种模式，实现 HEMPS、HEM2SD，实现 Rein 和 HEM 的空属性匹配功能                                                                       |
| 3    | 8/25 - 8/31   | 实现 HEM1SS，修改 HEMPS、HEM2SD，low、high 两端统一用同一套覆盖分区，做实验，数据处理                                                      |
| 4    | 9/1 - 9/7     | 实现 HEM3PS、HEM4DS、HEM5DD                                                                                                                |
| 5    | 9/8 - 9/14    | 实现 HEMSCDD，设置两种 full Bitset，修复空桶带来的 bug，修改订阅产生算法(热点区域)                                                         |
| 6    | 9/15 - 9/21   | 写论文 Design 章节大部分+修改，画图                                                                                                        |
| 7    | 9/22 - 9/28   | 实现 HEMSRPS，写论文 Introduction、State Reduction Optimization、Theoretical Analysis、Achievement                                         |
| 8    | 9/29 - 10/05  | 写论文、Setup 部分(找 baseline 参数未果)，实现 HEM5DD 插入算法、两个暴力匹配 Simple 和 Simple2 算法，优化 HEMSRPS 选择 bitset 策略         |
| 9    | 10/06 - 10/12 | 写论文 Verification Experiment 部分，跑实验，画图                                                                                          |
| 10   | 10/13 - 10/19 | 实现 Rein 和 HEM5DD 删除算法写论文 Verification Experiment、Related Work 部分，跑实验，画图，将 Tama、AdaRein 整合进项目，修改论文某些部分 |
| 11   | 10/20 - 10/26 | 代码重构，将 OpIndex 简化后整合进项目，调参跑完所有实验，画图，写论文实验部分                                                              |
| 12   | 10/27 - 11/02 | 整体上修改论文，加入完整匹配例子                                                                                                           |
| 13   | 11/03 - 11/09 | 压缩论文，去掉两个优化，计数方法改为调用 count 函数，计数时间减少 90%以上，重跑所有实验，重画图，重写实验部分，11-08 提交论文              |
|      |               |                                                                                                                                            |

## NOTE

#### Windows 10 环境：Visual Studio 2022

1. 属性页，配置(C): 选择 "活动(Debug)" 或 "活动(Release)"，平台(P): 选择 "活动(x64)"
2. 配置属性 - C/C++ - 优化 - 优化，选择 "优化(优选速度) (/Ox)"
3. 配置属性 - C/C++ - 预处理器 - 预处理器定义，删掉里面的内容，加入一行 "\_CRT_SECURE_NO_WARNINGS"
4. 配置属性 - C/C++ - 代码生成 - 基本运行时检查，改成 "默认值"
5. 属性页，配置(C): 选择 "活动(Debug)"，平台(P): 选择 "活动(x64)"
6. 配置属性 - 链接器 - 系统 - 四个大小，都填 3221225472
7. 左上角的下拉框里进入配置管理器，活动解决方案配置为 "Debug"，下面的配置为 "Release"，勾选生成

#### ubuntu 20 环境

1. Run command "sudo cmake ."
2. Run command "sudo make"
3. Run command "sudo cpufreq-set -c 8 -g performance" (调整第 8 号核为性能模式下运行)
4. sudo cpufreq-set -c 8 -d 3.6G
5. sudo cpufreq-set -c 8 -u 4.9G
6. Run command "ulimit -s unlimited; taskset -c 8 ./run" (8 指指定第 8 号核单独运行程序)
7. 查看最近一次部分算法的结果：head -1 Rein.txt Rein_F_DMFT.txt fRein.txt fRein_C.txt fRein_C_B.txt hRein.txt hRein_c.txt  HEM5.txt Tama.txt bTama6.txt bTama8.txt fBGTree.txt fBGTree_C.txt fBGTree_CB.txt bBGTree.txt bBGTree_F.txt fBGTree_d.txt fBGTree_d_C.txt fBGTree_d_CB.txt bBGTree_d.txt bBGTree_d_F.txt
8. Debug: g++ -g -o derun *.h *.cpp -mavx2 -std=c++2a -lboost_system -lpthread; gdb derun

#### 注意事项

1. "constant.h" 里 MAX_SUBS、MAX_ATTS、MAX_BUCKS 必须大于等于 subs、atts、buks (Now it's obsoleted.)
2. In "constant.h", the constant `subs` can't be adjusted by input, means you have to run relative experiments one by one.
3. be=1 的 bitset 是覆盖的全部桶, 即只有 fullBits 有效, bits 数组不存数据, 若想切换成覆盖一半桶, 需在构造函数里修改数组初始化代码, 在 initBits 函数里修改初始化 bits 代码, 开启“if (numBits == 1)”特判代码, 有双重反向优化的还需在 match 函数里取 bitset 时增加两处“&& numBits > 1”判断, 还有多事件类型时对空属性的处理需特判, 以及插入和删除逻辑特判

## 实验记录

- 真实数据预处理：https://www.aconvert.com/cn/document/xlsx-to-txt/

- Default Parameters: n = 1**m**, d = 20, $S_s$ = 10, $S_e$ = 20, w = 0.3, subp = 0, uniform distribution.
- Model Parameters:
  - Rein: b = 1000
  - HEM5DD: b = 1000, be = 4
  - Tama: level = 13
  - AdaRein: falsePositiveRate = 10

| No.  | 实验名                           | 非默认参数设置                                                | 在 txt 文件中的记录号（expID）             |
| ---- | :------------------------------- | ------------------------------------------------------------- | ------------------------------------------ |
| 1    | 基准验证实验(标记时间、匹配时间) | --                                                            | 403 ~ 412, 421 ~ 430; 571 ~ 580, 681 ~ 690 |
| 2    | 空维度验证实验(标记时间)         | $S_e$=10, p = 0.5                                             | 434 ~ 443; 593 ~ 602                       |
| 3    | 订阅数实验                       | n = 300**k** ~ 9**m**, 补: p = 0.5                            | 484 ~ 489; 613 ~ 618                       |
| 4    | 订阅大小实验                     | $S_s$ = 5 ~ 30, d = $S_e$ = 30, w = 0.7, 补: p = 0.5          | 497 ~ 502, 559 ~ 564; 620 ~ 625            |
| 5    | 事件大小实验                     | $S_e$ = 30 ~ 80, d = 80, p = 0.5                              | 503 ~ 508; 626 ~ 631                       |
| 6    | 订阅宽度实验                     | w = 0.1 ~ 0.9, $S_s$ = 5                                      | 517 ~ 523; 646 ~ 654                       |
| 7    | 维度实验                         | d = 30 ~ 900，p = w = 0.5                                     | 528 ~ 533; 657 ~ 662                       |
| 8(a) | 事件订阅属性 Zipf 分布实验       | alpha = 0 ~ 5, d = 50, w = 0.5                                | 538 ~ 543                                  |
| 8(b) | 真实事件订阅属性 Zipf 分布实验   | alpha = 0 ~ 5, d = 50, Ss = 5                                 | 666 ~ 671                                  |
| 9    | 综合性能实验                     | $S_s$ = 1 ~ 20, $S_e$ = 1 ~ 40, w = 0 ~ 1, d = 100, alpha = 1 | 566, 567                                   |
| 10   | 构建时间、删除时间实验(即实验 4) | $S_s$ = 5 ~ 30, d = $S_e$ = 30, w = 0.7, 补: p = 0.5          | 559 ~ 564; 674 ~ 679                       |
| 11   | 内存实验(即实验 3)               | n = 300**k** ~ 9**m**                                         | 484 ~ 489; 613 ~ 618                       |

#### Unresolved Error

- 实验 9 的匹配个数有错，两个 Simple 算法的结果是一样的，其他所有算法的结果是一样的，在 500、10000 个事件时两个平均匹配个数相差**都**少于 1
- Why can setting p=0.5 increase the matching probability clearly in Exp 3 and 4 for real data set ?

# BG-Tree/FBMAD/C-BOMP/DMFT 开发月志

| 月      | 进展情况                                                                                                                                                                                                                        |
| ------- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| 2021-07 | 提出三层划分模型（0-0.5, 0.5-1.0, 跨 0.5）                                                                                                                                                                                      |
| 2021-11 | 基于三层划分模型结合 AWB+Tree 提出新数据结构 BGTree，做初步设计(讲 PPT)与分析，实现初始化函数                                                                                                                                   |
| 2021-12 | 做插入匹配的数据结构详细设计 1.0，实现插入删除函数，提出正向计数优化范式 C-BOMP 和方向匹配基本定理 1                                                                                                                            |
| 2022-01 | 实现 fRein, fRein-C, hRein, hRein-C，提出将 OpIndex、PS-Tree、C-BOMP、节点动态伸缩结合在一起的优化，提出多分割点优化(多叉树)，解剖 BG-Tree 做可视化分析，提出方向匹配基本定理 2，做支持正反匹配的数据结构详细设计 2.0 以及实现，基于此实现节点动态伸缩、正向事件匹配 |
| 2022-02 | 基于第二版本的同一数据结构实现反向事件匹配；细化 BG-Tree-d-vsr 并开始实现
| 2022-11 | 实现 pTama_Lock, pTama_Reduce, bTama6_p, bTama8_p, fRein/hRein 的 PGWO 系列，fRein-C-B，Rein-F
|2022-12| 重构BGTree1.0，实现 bBGTree，bBGTree_F，fBGTree_d_C，fBGTree_d_CB, bBGTree_d_F 并优化其他5个BGTree版本

# 2022-HEM 扩展版开发周志

| 周数 | 时间        | 进展情况                                                     |
| ---- | ----------- | :----------------------------------------------------------- |
| 1    | 3/1 - 3/2   | 细化虚实属性组优化方法，分析订阅是否跨属性组的4种情况。      |
| 2    | 3/19 - 3/25 | Debug，把 $g=1$ 时桶组跨一半桶的情况改成跨所有桶的情况(即不再考虑这一特殊情况)；做HEM-PPT，写演讲稿，做演讲视频。 |
| 3    | 3/26 - 3/29 | 实现 VAS 和 RAS 优化，做实验；代码重构，实现HEM系列10个版本自动切换是否测量组成时间。 |
| 4    | 4/5 - 4/10  | 设计并执行实验；调期刊论文模板；分析虚实属性组优化性能，推导最优划分属性子集数公式；处理数据，画图。 |
| 5    | 4/12 - 4/17 | 画图，贴图；写论文： DRO 及其理论分析，DGO动态分组优化。     |
| 6    | 4/19 - 4/25 | 写论文：虚实属性子集优化和两条定理，复杂度-瓶颈分析，改图，改实验，写完初稿。 |
| 7    | 5/4 - 5/10  | 实现pRein、pHEM5DD、pHEM5DD-avxOR，做实验，论文加入实现优化方面的内容和总结 |
| 8    | 5/11 - 5/15 | 跑更多并行、SIMD、高负载 RAS 实验，处理数据，画图，写论文，加入扩展实验部分 |

## 2022 HEM 扩展版实验

- Default Parameters: n = 1000000, d = 20, $\phi_s$ = 10, $\phi_e$ = 20, w = 0.4, attrSubsetNum = 1, valDom = 1000000, numPub = 1000, parallelDegree(pD) = 1 inform distribution.
- Model Parameters:
  - Rein: c = 1000
  - HEM5DD: c = 1000, g = 4
  - Tama: level = 13
  - AdaRein: falsePositiveRate = 5%

| No.  | 实验名                                                 | 非默认参数设置                                               | 在 txt 文件中的记录号（expID）             |
| ---- | :----------------------------------------------------- | ------------------------------------------------------------ | ------------------------------------------ |
| 1    | 基准验证实验(标记时间、匹配时间、内存)                 | --                                                           | 952-961                                    |
| 2    | 空维度验证实验(匹配时间)                               | d=1000, $\phi_e$=[1000,800,600,400,200,10], be=1, attrSubsetNum=[1,1,1,2,5,100] | 1097-1102                                  |
| 3    | 高维虚属性组实验(或运算时间)                           | $b_e$=1, d=10000, w=0.8, $\phi_s$=5, attrSubsetNum=[20, 50,100,400,500,1000] | 1015-1020                                  |
| 4    | 高维实属性组实验(或运算时间)                           | $b_e$=1, d=10000, w=0.8, $\phi_s$=5, $\phi_e$=[20, 10], <br/>attrSubsetNum=[20, 50,100,400,500,1000] | 1021-1026                                  |
| 5    | 订阅数实验                                             | n = 300**k** ~ 9**m**                                        | 1029-1034、PSTB‘: 1141、1142               |
| 6    | 订阅大小实验                                           | $\phi_s$=5~30, d=$\phi_e$= 30, w=0.8                         | 1035-1040、PSTB‘: 1143-1148                |
| 7    | 事件大小实验                                           | $S_e$ = 30 ~ 80, d = 80, $S_s$ = 5                           | 1041-1046、PSTB: 1149-1154                 |
| 8    | 订阅宽度实验                                           | w=0.1 ~ 0.9, $\phi_s$=5                                      | 1047-1055、PSTB: 1155-1163                 |
| 9    | 虚拟属性组维度实验                                     | d=30 ~ 10000, be=4~1, $\phi_s$=5, attrSubset=1~400, w=0.8    | 1056-1061、PSTB: 1165-1170                 |
| 10   | 实属性组维度实验                                       | d=30 ~ 10000, be=4~1, $\phi_s$=5, attrSubset=1~500, w=0.8    | 1176-1181                                  |
| 11   | 事件订阅属性 Zipf 分布实验                             | alpha = 0 ~ 5, d=100, attrSubsetNum=25                       | 1069-1074                                  |
| 12   | 值域实验                                               | d=1000, attrGroup=40, w=0.8, valDom=100~1000000              | 1091-1095(PSTB 有 3 个错误)                |
| 13   | 综合性能实验                                           | $\phi_s$= 1 ~ 10, $\phi_e$ = 1 ~ 40, w=0~1, d=100, alpha=1, attrSubsetNum=25 | 1081                                       |
| 14   | 构建时间、删除时间实验(即实验 6)                       | $\phi_s$=5~30, d=$\phi_e$=30, w=0.8                          | 1082-1087                                  |
| 15   | 内存实验(即实验 5)                                     | n = 300**k** ~ 9**m**                                        | 1029-1034                                  |
| 16   | SIMD实验 on HEMDD (匹配时间和或运算时间下降比例-2个图) | $\psi_e$=d=[16, 32, 64, 128, 256, 512], BlockSize=256,512    | 1355-1360, 1361-1366，1393-1401, 1409-1417 |
| 17   | pHEMDD 并行实验（加速比和效率-两图）                   | $\psi_e=d=$ 512, pD = [1, 2, 4, 8, 16, 32, 64]               | 1402-1408                                  |
| 18   | pHEMDD-avxOR 组合实验（和HEMDD、pHEMDD 比较）          | $\psi_e=d=$ 512, pD = [1, 2, 4, 8, 16, 32, 64], BlockSize=512 | 1402-1408                                  |
| 19   | pHEMDD-RAS-avxOR 高负载高维度实验（和HEMDD-RAS比较）   | n = $2^{27}$, d=2048, attrSubset=32, $\psi_e$=64, pD = [1, 2, 4, 8, 16, 32, 64], $b_e$ = 1 | 1386-1392                                  |
|20| 事件值实验| 0-0.1M, 0.1M-0.2M,...,0.9M-1M    | 随机宽度: 1658-1667，固定w0.4: 1668-1677  |

## HEM 命名史

* ReinBits (**RB**) ~ **BIOP** ~ **HEM**
* Multi-level Pre-mark Caching (**MPC**) ~ Cache-based Mismatch Pre-mark (**CMP**)
* Double Reverse Optimization ~ Dual Reverse Optimization (**DRO**) ~ Group Selection Optimization (**GSO**) ~ Dual Selection Strategy (**DSS**) ~ Optimal Group Selection (**OGS**)
* Dynamic Group Optimization (**DGO**) ~ Group Partitioning Optimization (**GPO**) ~ Load-oriented Grouping Scheme (**LGS**) ~ Skewness-aware Cell Grouping (**SCG**)
* Virtual/Real Attribute Subset Optimization (**ASO**) ~ Attribute Clustering Method (**ACM**) ~ Attribute Set Partition (**ASP**)
* Pure Static HEM (**HEMPS**) ~ HEM with  Equal Cell-number Tolerance GPO (**HEM-C)** ~ Basic HEM (**HEM-B**)
* Static Static HEM (**HEMSS**) ~ HEM with  Equal Cell-number Tolerance GPO and Cell Number GSO (**HEM-CC**) ~ HEM with Dual Selection Strategy (**HEM-BD**) ~ **HEM-BO**
* Static Dynamic HEM (**HEMSD**) ~ HEM with  Equal Cell-number Tolerance GPO and Workload GSO (**HEM-CW**) ~ **HEM-BL** ~ **HEM-BS**
* Pure Dynamic HEM (**HEMPD**) ~ HEM with Equal Predicate-number Tolerance GPO (**HEM-P**) ~ HEM with Load-oriented Grouping Scheme (**HEM-L**) ~ **HEM-S**
* Dynamic Static HEM (**HEMSD**) ~ HEM with Equal Predicate-number Tolerance GPO and Cell Number GSO (**HEM-PC**) ~ **HEM-LD' **~ **HEM-SO'**
* Dynamic Dynamic HEM (HEMDD) ~ HEM with Equal Predicate-number Tolerance GPO and Workload GSO (**HEM-PW**) ~ **HEM-LD **~ **HEM-SO**
