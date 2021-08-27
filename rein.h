#ifndef _REIN_H
#define _REIN_H
#include <cstring>
#include "util.h"
#include <algorithm>
#include <unordered_set>

#define _for(i,a,b) for( int i=(a); i<(b); ++i)
#define __for(i,a,b) for( int i=(a); i<=(b); ++i)
#define mfor(i,a,b) for(int i=(a);i>(b);--i)
#define mmfor(i,a,b) for(int i=(a);i>=(b);--i)

const int MAX_SUBS = 2000001;
const int MAX_ATTS = 101;
const int MAX_BUCKS = 500;

// 01在第一维
class Rein {
	int numSub, numDimension, buckStep;
	vector<vector<vector<Combo>>> data[2];    // 0:left parenthesis, 1:right parenthesis

public:
	int numBucket;
	double compareTime = 0.0; // 所有维度上事件值落入的那个cell里逐个精确比较的时间
	double markTime = 0.0;    // 标记时间
	double bitTime = 0.0;     // 遍历bits数组得到结果所需的时间
	vector<unordered_set<int>> bucketSub;   // id相同的桶存储的不同订阅个数的和

	Rein(int valDom, int numSubscription, int numDim, int numBuck) :numSub(numSubscription), numDimension(numDim) {
		buckStep = (valDom - 1) / numBuck + 1;
		numBucket = (valDom - 1) / buckStep + 1;
		cout << "Rein: bucketStep = " << buckStep << ", numBucket = " << numBucket << endl;
		bucketSub.resize(numBucket);
		data[0].resize(numDimension, vector<vector<Combo>>(numBucket));
		data[1].resize(numDimension, vector<vector<Combo>>(numBucket));
	}
	//void insert(Sub sub);
	void insert(IntervalSub sub);
	//void match(const Pub& pub, int& matchSubs, const vector<Sub>& subList);
	void match(const Pub& pub, int& matchSubs);
	void calBucketSize(); // 计算bucketSize
	int calMemory();     // 计算占用内存大小
};

// 01在第二维，全动态
//class Rein {
//	int numSub, numDimension, buckStep, numBucket; // 维度，桶长度，桶数
//	vector<vector<vector<vector<Combo>>>> data;    // 0:left parenthesis, 1:right parenthesis
//
//public:
//	Rein(int valDom, int numSubscription, int numDim, int numBuck) :numSub(numSubscription), numDimension(numDim) {
//		buckStep = (valDom - 1) / numBuck + 1;
//		numBucket = (valDom - 1) / buckStep + 1;
//		cout << "Rein: bucketStep = " << buckStep << ", numBucket = " << numBucket << endl;
//		data.resize(numDimension, vector<vector<vector<Combo>>>(2, vector<vector<Combo>>(numBucket)));
//	}
//	//void insert(Sub sub);
//	void insert(IntervalSub sub);
//	//void match(const Pub& pub, int& matchSubs, const vector<Sub>& subList);
//	void match(const Pub& pub, int& matchSubs);
//};

// 01在第一维，写死
//class Rein {
//	int numSub, numDimension, buckStep, numBucket; // 维度，桶长度，桶数
//	vector<Combo> data[MAX_ATTS][2][MAX_BUCKS];
//
//public:
//	Rein(int valDom, int numSubscription, int numDim, int numBuck) :numSub(numSubscription), numDimension(numDim) {
//		buckStep = (valDom - 1) / numBuck + 1;
//		numBucket = (valDom - 1) / buckStep + 1;
//		cout << "Rein: bucketStep = " << buckStep << ", numBucket = " << numBucket << endl;
//		//data.resize(numDimension, vector<vector<vector<Combo>>>(2, vector<vector<Combo>>(numBucket)));
//	}
//	//void insert(Sub sub);
//	void insert(IntervalSub sub);
//	//void match(const Pub& pub, int& matchSubs, const vector<Sub>& subList);
//	void match(const Pub& pub, int& matchSubs);
//};

#endif
