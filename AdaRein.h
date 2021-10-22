#ifndef ADAREIN_H
#define ADAREIN_H
#include<vector>
#include <cstring>
#include <algorithm>
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
	vector<attAndCount> attsCounts;

public:
    int numBucket;
    AdaRein():numSub(0) {
        buckStep = (valDom - 1) / buks + 1;
        numBucket = (valDom - 1) / buckStep + 1;
		data[0].resize(atts, vector<vector<Combo>>(numBucket));
		data[1].resize(atts, vector<vector<Combo>>(numBucket));
		skipped.resize(atts, false);
		attsCounts.resize(atts);
        cout << "ExpID = " << expID << ". AdaRein: falsePositiveRate = " << falsePositiveRate << ", bucketStep = " << buckStep << ", numBucket = " << numBucket << endl;
    }

    void insert(IntervalSub sub);
    bool deleteSubscription(IntervalSub sub);

    void exact_match(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);
    void select_skipped_atts(double falsePositive, const vector<IntervalSub>& subList);
    void approx_match(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

    int calMemory();      // 计算占用内存大小, 返回MB
};

#endif //ADAREIN_H
