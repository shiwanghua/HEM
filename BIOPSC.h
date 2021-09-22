#pragma once
//#ifndef _BIOPSC_H
//#define _BIOPSC_H

#include <cstring>
#include "util.h"
#include "constant.h"
#include <algorithm>
#include <unordered_set>
#include <bitset>

#define _for(i,a,b) for( int i=(a); i<(b); ++i)
#define __for(i,a,b) for( int i=(a); i<=(b); ++i)
#define mfor(i,a,b) for(int i=(a);i>(b);--i)
#define mmfor(i,a,b) for(int i=(a);i>=(b);--i)

// 订阅分类+动动模式 Subscriptions Classification
class BIOPSC
{
private:
	int numSub, numDimension, buckStep, levelStep, numBits;
	vector<vector<vector<vector<Combo>>>> data[2];  // low/high, 维度, 宽度, 桶号, 桶内偏移量
	vector<vector<vector<bitset<subs>>>> bits[2];   // low/high, 维度, 宽度, bitset号, 订阅id
	vector<vector<vector<int>>> fix[2];             // low/high, 维度, 宽度, 桶号
	vector<bitset<subs>> fullBits;					// 只能用于空事件维度上
	vector<vector<bitset<subs>>> fullBL;			// Bug: 只能用于每一层的双重反向匹配!存储每一层的所有订阅
	int*** endBucket[2], *** bitsID[2];             // low/high, 维度, 宽度, 桶号
	bool*** doubleReverse[2];                       // low/high, 维度, 宽度, 桶号

public:
	int numBucket;                          // 一层的最大桶数
	int numLevel;                           // 每个维度上的层数
	double compareTime = 0.0;               // 所有维度上事件值落入的那个cell里逐个精确比较的时间
	double markTime = 0.0;                  // 标记时间
	double orTime = 0.0;                    // 或运算时间
	double bitTime = 0.0;                   // 遍历bits数组得到结果所需的时间
	//vector<unordered_set<int>> bucketSub;   // id相同的桶存储的不同订阅个数的和

	BIOPSC();
	~BIOPSC();

	//void insert(Sub sub);
	void insert(IntervalSub sub);
	//void match(const Pub& pub, int& matchSubs, const vector<Sub>& subList);
	void match(const Pub& pub, int& matchSubs);

	void initBits();      // 插入完后初始化bits数组
	//void calBucketSize(); // 计算bucketSize
	int calMemory();      // 计算占用内存大小
	void printRelation(int dimension_i, int li); // 打印映射关系
};

//#endif