#ifndef TAMA_H
#define TAMA_H
#include "util.h"
#include "constant.h"

using namespace std;

//const int MAX_ATTS = 101;

class Tama {
    int nodeCounter;
    int* lchild, * rchild, * mid;
    vector<int>* data[MAX_ATTS];
    int counter[subs];

    void initiate(int p, int l, int r, int lvl);

    int median(int l, int r);

    void insert(int p, int att, int subID, int l, int r, int low, int high, int lvl);
    bool deleteSubscription(int p, int att, int subID, int l, int r, int low, int high, int lvl);
    void match_accurate(int p, int att, int l, int r, const int value, int lvl, const vector<IntervalSub>& subList);
    void match_vague(int p, int att, int l, int r, const int value, int lvl);

public:
    Tama()
    {
        int nodeNumber = 1 << level;
        for (int i = 0; i < atts; i++)
            data[i] = new vector<int>[nodeNumber];
        lchild = new int[nodeNumber];
        rchild = new int[nodeNumber];
        mid = new int[nodeNumber];
        nodeCounter = 0;
        initiate(0, 0, valDom - 1, 1);
    }

    void insert(IntervalSub sub);
    bool deleteSubscription(IntervalSub sub);
    void match_accurate(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);
    void match_vague(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);
    int calMemory();
};

#endif //TAMA_H
