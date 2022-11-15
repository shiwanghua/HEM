#ifndef TAMA_H
#define TAMA_H

#include "Util.h"
#include "constant.h"
#include "ThreadPool.h"

using namespace std;

//const int MAX_ATTS = 101;

class Tama
{
	int nodeCounter;
	int* lchild, * rchild, * mid;
//    vector<int>* data[MAX_ATTS];
	vector<vector<vector<int32_t>>> data;
	int32_t counter[subs];
	vector<mutex*> mutexs;
	ThreadPool threadPool;

	void initiate(int p, int l, int r, int lvl);

	int median(int l, int r);

	void insert(int p, int att, int subID, int l, int r, int low, int high, int lvl);

	bool deleteSubscription(int p, int att, int subID, int l, int r, int low, int high, int lvl);

	void match_accurate(int p, int att, int l, int r, const int value, int lvl, const vector<IntervalSub>& subList);

	void match_vague(int p, int att, int l, int r, const int value, int lvl);

	void match_parallel_lock(int p, int a, int l, int r, const int v, int lv, const vector<IntervalSub>&);

	void match_parallel_reduce(int p, int a, int l, int r, const int v, int lv, const vector<IntervalSub>&, vector<int32_t>& pCounter);

 public:
	Tama(int type);
	~Tama();

	void insert(IntervalSub sub);

	bool deleteSubscription(IntervalSub sub);

	void match_accurate(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	void match_vague(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	void match_parallel_lock(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	void match_parallel_reduce(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	int calMemory();
};

#endif //TAMA_H
