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
    int valDom, buckStep, bucks, numSub;
    vector<Combo> data[MAX_ATTS][2][MAX_BUCKS];    // 0:left parenthesis, 1:right parenthesis
    bool bits[MAX_SUBS];
    bool skipped[MAX_ATTS];
    attAndCount attsCounts[MAX_ATTS];
public:
    AdaRein():numSub(0) {
        buckStep = (valDom - 1) / MAX_BUCKS + 1; 
        bucks = (valDom - 1) / buckStep + 1; 
    }

    void insert(IntervalSub sub);
    bool deleteSubscription(IntervalSub sub);

    void accurate_match(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);
    void select_skipped_atts(const vector<IntervalSub>& subList, double falsePositive);
    void approx_match(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

    int calMemory();      // 计算占用内存大小, 返回MB
};

#endif //ADAREIN_H
