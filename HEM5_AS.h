#pragma once

#include <cstring>
#include "Util.h"
#include "constant.h"
#include <algorithm>
#include <unordered_set>
#include <bitset>
#include "ThreadPool.h"

// Virtual/Real Attribute Group Mode
class HEM5_AS {
private:
	int numSub, numDimension, buckStep, numBits, numAttrGroup, attrGroupSize;
	vector<vector<vector<Combo>>> data[2];  // 0:left parenthesis, 1:right parenthesis
	vector<vector<bitset<subs>>> bits[2];   // 需要提前知道订阅个数...
	vector<vector<int>> fix[2];             // 0是low上的后缀和，1是high上的前缀和，可以用于计算任务量
	vector<bitset<subs>> fullBits;          // 全覆盖的bits单独存，因为只要存一次
	vector<bitset<subs>> attrGroupBits;     // 每个实or虚属性组对应一个bitset
	int **endBucket[2], **bitsID[2];       // 落入这个bucket的事件在标记时终止于哪一个bucket、用到的bits数组的下标
	bool **doubleReverse[2];                // 为true时是把1标成0
	ThreadPool threadPool;
public:
	int numBucket;
	double compareTime = 0.0;               // 所有维度上事件值落入的那个cell里逐个精确比较的时间
	double markTime = 0.0;                  // 标记时间
	double orTime = 0.0;                    // 或运算时间
	double bitTime = 0.0;                   // 遍历bits数组得到结果所需的时间
	double mergeTime = 0.0;
	//vector<unordered_set<int>> bucketSub;   // id相同的桶存储的不同订阅个数的和

	HEM5_AS(int);

	~HEM5_AS();

	void insert_VAS(IntervalSub sub); // 没有bitset时的插入算法
	void insert_RAS(IntervalSub sub); // 没有bitset时的插入算法

	void initBits();      // 插入完后初始化bits数组

	void insert_online_VAS(IntervalSub sub); // 构建好订阅集后的在线插入订阅算法, 虚拟属性组版本
	void insert_online_RAS(IntervalSub sub); // 构建好订阅集后的在线插入订阅算法, 实际属性组版本

	bool deleteSubscription_VAS(IntervalSub sub);

	bool deleteSubscription_RAS(IntervalSub sub);

	void match_VAS(const Pub &pub, int &matchSubs);

	void match_RAS(const Pub &pub, int &matchSubs);

	void match_RAS_avxOR_parallel(const Pub &pub, int &matchSubs);

	//void calBucketSize(); // 计算bucketSize
	int calMemory();      // 计算占用内存大小
	void printRelation(int dimension_i); // 打印映射关系
	vector<int> calMarkNumForBuckets(); // 计算事件落到每个属性的同一个桶里时需要标记和比较的谓词个数
};
