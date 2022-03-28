#ifndef ADAREIN_H
#define ADAREIN_H
#include<vector>
#include <cstring>
#include <algorithm>
#include <queue>
#include "generator.h"
#include "chrono_time.h"
#include "util.h"
#include "data_structure.h"
#include "constant.h"

//const int MAX_SUBS = 2000001;
//const int MAX_ATTS = 3000;
//const int MAX_BUCKS = 2000;

class AdaRein {
    int buckStep, numSub;
    //vector<Combo> data[MAX_ATTS][2][MAX_BUCKS];    // 0:left parenthesis, 1:right parenthesis
//    bool bits[MAX_SUBS];
//    bool skipped[MAX_ATTS];
//    attAndCount attsCounts[MAX_ATTS];
	vector<vector<vector<Combo>>> data[2];
	vector<bool> skipped;
	vector<attAndCount> attsCounts; // 用于原始版本
    vector<int> endBucket[2]; // i号属性上所应遍历到的终点桶 low(0)上表示遍历到小于这个桶, high(1)上表示遍历到大于等于这个桶

public:
    int numBucket;
    AdaRein(int);

    void insert(IntervalSub sub);
    bool deleteSubscription(IntervalSub sub);
    void exact_match(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

    void original_selection(double falsePositive, const vector<IntervalSub>& subList);
    void approx_match_ori(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

    void static_succession_selection(double falsePositive, const vector<IntervalSub>& subList);
    void approx_match_sss(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

    int calMemory();      // 计算占用内存大小, 返回MB
};

#endif //ADAREIN_H
