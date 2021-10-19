//#pragma once
#ifndef SIMPLE2_H
#define SIMPLE2_H

//#include <cstring>
#include "util.h"
#include "constant.h"
#include <algorithm>
//#include <unordered_set>
//#include <bitset>

#define _for(i,a,b) for( int i=(a); i<(b); ++i)
#define __for(i,a,b) for( int i=(a); i<=(b); ++i)
#define mfor(i,a,b) for(int i=(a);i>(b);--i)
#define mmfor(i,a,b) for(int i=(a);i>=(b);--i)

// 暴力算法+订阅的谓词按宽度排序
class Simple2
{
private:
	vector<IntervalSub> data;
public:
	int numSub;
	Simple2();
	~Simple2();

	//void insert(Sub sub);
	void insert(IntervalSub sub);
	bool deleteSubscription(IntervalSub sub);
	//void match(const Pub& pub, int& matchSubs, const vector<Sub>& subList);
	void match(dPub& pub, int& matchSubs);
	int calMemory();      // 计算占用内存大小
};

#endif