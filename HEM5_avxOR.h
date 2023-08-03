//#pragma once
#ifndef _HEM5_256OR_H
#define _HEM5_256OR_H

#include <cstring>
#include "Util.h"
#include "constant.h"
#include <algorithm>
#include <unordered_set>
#include <bitset>

#define _for(i,a,b) for( int i=(a); i<(b); ++i)
#define __for(i,a,b) for( int i=(a); i<=(b); ++i)
#define mfor(i,a,b) for(int i=(a);i>(b);--i)
#define mmfor(i,a,b) for(int i=(a);i>=(b);--i)

// 动动模式 用avx2 做256位的或运算
// 用于和 HEM5 对比
class HEM5_avxOR
{
private:
	int numSub, numDimension, buckStep, numBits;
	vector<vector<vector<Combo>>> data[2];  // 0:left parenthesis, 1:right parenthesis
	vector<vector<bitset<subs>>> bits[2];   // 需要提前知道订阅个数...
	vector<vector<int>> fix[2];             // 0是low上的后缀和，1是high上的前缀和，可以用于计算任务量
	vector<bitset<subs>> fullBits;          // 全覆盖的bits单独存，因为只要存一次
	int** endBucket[2], ** bitsID[2];       // 落入这个bucket的事件在标记时终止于哪一个bucket、用到的bits数组的下标
	bool** doubleReverse[2];                // 为true时是把1标成0

public:
	int numBucket;
	double compareTime = 0.0;               // 所有维度上事件值落入的那个cell里逐个精确比较的时间
	double markTime = 0.0;                  // 标记时间
	double orTime = 0.0;                    // 或运算时间
	double bitTime = 0.0;                   // 遍历bits数组得到结果所需的时间
	//vector<unordered_set<int>> bucketSub;   // id相同的桶存储的不同订阅个数的和

	HEM5_avxOR();
	~HEM5_avxOR();

	//void insert(Sub sub);
	void insert(IntervalSub sub); // 没有bitset时的插入算法
	void insert_online(IntervalSub sub); // 构建好订阅集后的在线插入订阅算法
	bool deleteSubscription(IntervalSub sub);
	//void match(const Pub& pub, int& matchSubs, const vector<Sub>& subList);
	void match(const Pub& pub, int& matchSubs);
	void match_debug(const Pub& pub, int& matchSubs);

	void initBits();      // 插入完后初始化bits数组
	//void calBucketSize(); // 计算bucketSize
	int calMemory();      // 计算占用内存大小
	void printRelation(int dimension_i); // 打印映射关系
	vector<int> calMarkNumForBuckets(); // 计算事件落到每个属性的同一个桶里时需要标记和比较的谓词个数
};

#endif