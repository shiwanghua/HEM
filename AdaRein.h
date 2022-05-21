#ifndef ADAREIN_H
#define ADAREIN_H
#include<vector>
#include <cstring>
#include <algorithm>
#include <queue>
#include "generator.h"
#include "chrono_time.h"
#include "Util.h"
#include "data_structure.h"
#include "constant.h"

//const int MAX_SUBS = 2000001;
//const int MAX_ATTS = 3000;
//const int MAX_BUCKS = 2000;

class AdaRein {
	int buckStep, numSub;
	//int dividerValue; // SSS-C-PPH 二分界点

	//vector<Combo> data[MAX_ATTS][2][MAX_BUCKS];    // 0:left parenthesis, 1:right parenthesis
//    bool bits[MAX_SUBS];
//    bool skipped[MAX_ATTS];
//    attAndCount attsCounts[MAX_ATTS];

	vector<attAndCount> attsCounts;
	vector<bool> skipped;
	vector<vector<vector<Combo>>> data[2];
	vector<int> endBucket[2]; // i号属性上所应遍历到的终点桶 low(0)上表示遍历到小于这个桶, high(1)上表示遍历到大于等于这个桶
	vector<int> beginBucket[2]; // SSS_b: i号属性上开始遍历的桶 low(0)上表示从这个桶开始往大号桶遍历

	int levelBuks, levelBuckStep, widthStep; // 每一层的桶数, 每一层的每个桶代表的区间宽度, 相邻层之间谓词最大宽度的差
	vector<vector<bool>> skippedW; // attr->width
	//vector<vector<attAndCount>> attsCountsW;
	vector<vector<vector<vector<vector<Combo>>>>> dataW; // SSS-C-PPH SSS-C-W: attr->level->low/high->bucketId->offset
	vector<vector<pair<pair<int, int>, pair<int, int>>>> beBucketW; // attr->level-><low,high>-><begin,end>

	vector<int> attsPredicate; // 每个属性上有多少个谓词
public:
	int numBucket;
	int maxSkipPredicate;
	long numSkipPredicateInTotal; // Used in DSS-B and DSS-B-W
	long numSkipBuckInTotal; // 过滤的离散桶数（不含全过滤属性上的桶）
	long numSkipAttsInTotal;
	AdaRein(int);

	void insert(IntervalSub sub);
	bool deleteSubscription(IntervalSub sub);
	void exact_match(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	void original_selection(double falsePositive, const vector<IntervalSub>& subList);
	void approx_match_ori(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	void static_succession_selection(double falsePositive, const vector<IntervalSub>& subList);
	void approx_match_sss(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	void static_succession_selection_backward(double falsePositive, const vector<IntervalSub>& subList);
	void approx_match_sss_b(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	void static_succession_selection_crossed(double falsePositive, const vector<IntervalSub>& subList);
	void approx_match_sss_c(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	/* void insert_sss_c_pph(IntervalSub sub);
	 bool deleteSubscription_sss_c_pph(IntervalSub sub);
	 void static_succession_selection_crossed_pph(double falsePositive, const vector<IntervalSub>& subList);
	 void approx_match_sss_c_pph(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);*/

	void insert_sss_c_w(IntervalSub sub);
	void static_succession_selection_crossed_width(double falsePositive, const vector<IntervalSub>& subList);
	void approx_match_sss_c_w(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	void insert_dss_b(IntervalSub sub);
	void dynamic_succession_selection_backward(double falsePositive, const vector<IntervalSub>& subList);
	void approx_match_dss_b(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	void approx_match_dss_b_w(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	int calMemory();      // 计算占用内存大小, 返回MB
	//int calMemory_sss_c_pph();
	int calMemory_sss_c_w();
};

#endif //ADAREIN_H
