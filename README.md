# BIOP

### 2021 - 开发周志

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
| 10   | 10/13 - 10/19 | 实现Rein和BIOP5DD删除算法写论文Verification Experiment部分，跑实验，画图              |
| 11   | 10/20 - 10/26 |                                                              |

### Note

##### Windows 10 环境：Visual Studio 2022

1. 属性页，配置(C): 选择 "活动(Debug)"，平台(P): 选择 "活动(x64)"
2. 配置属性 - C/C++ - 优化 - 优化，选择 "优化(优选速度) (/Ox)"

3. 配置属性 - C/C++ - 预处理器 - 预处理器定义，加入一行 "_CRT_SECURE_NO_WARNINGS"
4. 配置属性 - C/C++ - 代码生成 - 基本运行时检查，改成 "默认值"
5. 属性页，配置(C): 选择 "活动(Debug)"，平台(P): 选择 "活动(x64)"
6. 配置属性 - 链接器 - 系统 - 四个大小，分别输入5368709120、3221225472、5368709120、3221225472

##### ubuntu 20 环境

