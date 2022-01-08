#ifndef _REIN_H
#define _REIN_H
#include <cstring>
#include "util.h"
#include "constant.h"
#include <algorithm>
#include <unordered_set>

#define _for(i,a,b) for( int i=(a); i<(b); ++i)
#define __for(i,a,b) for( int i=(a); i<=(b); ++i)
#define mfor(i,a,b) for(int i=(a);i>(b);--i)
#define mmfor(i,a,b) for(int i=(a);i>=(b);--i)

//const int MAX_SUBS = 2000001;
//const int MAX_ATTS = 101;
//const int MAX_BUCKS = 500;

// 01在第一维
class Rein {
	int numSub, numDimension, buckStep;
	vector<vector<vector<Combo>>> data[2];    // original Rein and AWRein 0:left parenthesis, 1:right parenthesis
	vector<vector<vector<IntervalCombo>>> fData[2]; // forward Rein (fRein)
//	vector<vector<vector<vector<IntervalCombo>>>> awData[2]; // l/h->attr->level->buck->i
	int subPredicate[subs]; // forward Rein; for AWRein, it means the number of predicates inserted to fData for forward matching
	int counter[subs]; // forward Rein
	vector<bitset<subs>> nB; // null bitset for forward Rein with C-BOMP
	//vector<bitset<subs>> nnB; // non-null bitset for backward matching, same as HEM

public:
	int numBucket;
	double compareTime = 0.0; // 所有维度上事件值落入的那个cell里逐个精确比较的时间
	double markTime = 0.0;    // 标记时间
	double bitTime = 0.0;     // 遍历bits数组得到结果所需的时间
	vector<unordered_set<int>> bucketSub;   // id相同的桶存储的不同订阅个数的和

	Rein(int);

	// Original Rein
	//void insert(Sub sub);
	void insert_backward_original(IntervalSub sub);
	//void match(const Pub& pub, int& matchSubs, const vector<Sub>& subList);
	void match_backward_original(const Pub& pub, int& matchSubs);
	bool deleteSubscription_backward_original(IntervalSub sub);
	int calMemory_backward_original();     // 计算占用内存大小, 返回MB

	// Forward Rein
	void insert_forward_native(IntervalSub sub);
	void match_forward_native(const Pub& pub, int& matchSubs);
	bool deleteSubscription_forward_native(IntervalSub sub);
	int calMemory_forward_native();

	// Forward Rein with C-BOMP
	void insert_forward_CBOMP(IntervalSub sub);
	void match_forward_CBOMP(const Pub& pub, int& matchSubs);
	bool deleteSubscription_forward_CBOMP(IntervalSub sub);

	// HybridRein (AWRein)
	void insert_hybrid_native(IntervalSub sub);
	void match_hybrid_native(const Pub& pub, int& matchSubs);
	bool deleteSubscription_hybrid_native(IntervalSub sub);

	// HybridRein (AWRein) with C-BOMP
	void insert_hybrid_CBOMP(IntervalSub sub);
	void match_hybrid_CBOMP(const Pub& pub, int& matchSubs);
	bool deleteSubscription_hybrid_CBOMP(IntervalSub sub);

	void calBucketSize(); // 计算bucketSize
	vector<int> calMarkNumForBuckets(); // 计算事件落到每个桶里时需要标记和比较的谓词个数
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
