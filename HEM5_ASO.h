#pragma once

#include <algorithm>
#include <bitset>
#include <cstring>
#include <unordered_set>
#include "Util.h"
#include "constant.h"
#include "ThreadPool.h"

// Virtual/Real Attribute Subset/Class Mode with OR operation result precomputed optimization
class HEM5_ASO
{
private:
    int numSub;                            // 插入的订阅个数
    int numDimension;                      // 内容空间的维度
    int buckStep;                          // 每个桶负责的范围大小
    int numBits;                           // 位集的个数
    int numAttrGroup;                      // 属性组数
    int attrGroupSize;                     // 每个属性组负责的属性个数
    vector<vector<vector<Combo>>> data[2]; // 0: 低值端, 1:高值端
    vector<vector<bitset<subs>>> bits;     // 每个属性上高低两端只需要存一份位集了！
    vector<vector<int>> fix[2];            // 0 代表 low 上的后缀和，1是 high 上的前缀和，用于计算任务量
    // vector<bitset<subs>> fullBits;         // 全覆盖的 bits 单独存，因为只要存一次 // deprecated，高低端位集都只需要一份了
    vector<bitset<subs>> attrGroupBits; // 每个实 or 虚属性组对应一个bitset
    int **endBucket, **bitsID;          // 每个维度上，落入这个 bucket 的事件在标记时终止于哪一个 bucket、用到的 bits 数组的下标，也都只需要存一份了
    // bool **doubleReverse[2];            // 为 true 时是把 1 标成 0 // deprecated，取高左、低右端位集做或运算，不支持再反转了，即高端必须往事件桶左端标记，低端必须往事件桶右端标记
    ThreadPool threadPool; // 每个维度上不需要做或运算了，负载更小了

public:
    int numBucket;
    double compareTime = 0.0; // 所有维度上事件值落入的那个cell里逐个精确比较的时间
    double markTime = 0.0;    // 遍历完全不匹配桶的标记时间
    double orTime = 0.0;      // 维度之间做或运算的时间
    double bitTime = 0.0;     // 遍历结果数组获取匹配个数所花的时间
    double mergeTime = 0.0;   // 合并每个并行线程部分匹配结果的时间

    HEM5_ASO(int);

    ~HEM5_ASO();

    void insert_VASO(IntervalSub sub); // 没有bitset时的插入算法
    void insert_RASO(IntervalSub sub); // 没有bitset时的插入算法

    void initBits(); // 初始化 bits 数组 / 重新初始化 bits 数组以负载均衡，可以看成一个训练索引的过程

    void insert_online_VASO(IntervalSub sub); // 构建好订阅集后的在线插入订阅算法, 虚拟属性组版本
    void insert_online_RASO(IntervalSub sub); // 构建好订阅集后的在线插入订阅算法, 实际属性组版本

    bool deleteSubscription_VASO(IntervalSub sub);

    bool deleteSubscription_RASO(IntervalSub sub);

    void match_VASO(const Pub &pub, int &matchSubs);

    void match_RASO(const Pub &pub, int &matchSubs);

    void match_RASO_avxOR_parallel(const Pub &pub, int &matchSubs);

    int calMemory();                     // 计算占用内存大小
    void printRelation(int dimension_i); // 打印映射关系
    vector<int> calMarkNumForBuckets();  // 计算事件落到每个属性的同一个桶里时需要标记和比较的谓词个数
};
