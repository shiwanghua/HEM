#ifndef _TAMA_H
#define _TAMA_H
#include "util.h"
#include "constant.h"

using namespace std;

const int MAX_ATTS = 101;

class Tama {
    int nodeCounter;
    int* lchild, * rchild, * mid;
    vector<int>* data[MAX_ATTS];
    int counter[subs];

    void initiate(int p, int l, int r, int lvl);

    int median(int l, int r);

    void insert(int p, int att, int subID, int l, int r, int low, int high, int lvl);
    bool deleteSubscription(int p, int att, int subID, int l, int r, int low, int high, int lvl);
    void match(int p, int att, int l, int r, int value, int lvl);

public:
    Tama()
    {
        for (int i = 0; i < atts; i++)
            data[i] = new vector<int>[1 << level];
        lchild = new int[1 << level];
        rchild = new int[1 << level];
        mid = new int[1 << level];
        nodeCounter = 0;
        initiate(0, 0, valDom - 1, 1);
    }

    void insert(IntervalSub sub);
    bool deleteSubscription(IntervalSub sub);
    void match(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);
    int calMemory();
};

#endif //_TAMA_H
