//#pragma once
#ifndef SIMPLE_H
#define SIMPLE_H

//#include <cstring>
#include "util.h"
#include "constant.h"
//#include <algorithm>
//#include <unordered_set>
//#include <bitset>

#define _for(i,a,b) for( int i=(a); i<(b); ++i)
#define __for(i,a,b) for( int i=(a); i<=(b); ++i)
#define mfor(i,a,b) for(int i=(a);i>(b);--i)
#define mmfor(i,a,b) for(int i=(a);i>=(b);--i)

// 暴力算法: 第一次遇到不匹配谓词就停止, 继续判断下一个订阅
class Simple
{
private:
	vector<IntervalSub> data;
public:
	Simple();
	~Simple();

	//void insert(Sub sub);
	void insert(IntervalSub sub);
	//void match(const Pub& pub, int& matchSubs, const vector<Sub>& subList);
	void match(dPub& pub, int& matchSubs);
	int calMemory();      // 计算占用内存大小
};

#endif