#ifndef bTAMA_H
#define bTAMA_H

#include "Util.h"
#include "constant.h"
#include <bitset>
#include "ThreadPool.h"
using namespace std;

//const int MAX_ATTS = 101;

class bTama
{
	int nodeCounter;
	int numSub; // Sub inserted
	int* lchild, * rchild, * mid;
	//    vector<int>* data[MAX_ATTS];
	vector<vector<vector<int>>> data;
	//int counter[subs];
	vector<bitset<subs>> nB; // null bitset
	vector<bitset<subs>> nnB; // non-null bitset, same as HEM
	ThreadPool threadPool;

	void initiate(int p, int l, int r, int lvl);

	int median(int l, int r);

	void insert(int p, int att, int subID, int l, int r, int low, int high, int lvl);

	bool deleteSubscription(int p, int att, int subID, int l, int r, int low, int high, int lvl);

	void
	forward_match_accurate(int p, int att, int l, int r, const int value, int lvl, const vector<IntervalSub>& subList, bitset<
		subs>& mB);

	void
	backward2_match_accurate(int p, int att, int l, int r, const int value, int lvl, const vector<IntervalSub>& subList, bitset<
		subs>& mB);

	//void match_vague(int p, int att, int l, int r, const int value, int lvl);

 public:
	bTama(int32_t);

	void insert(IntervalSub sub);

	bool deleteSubscription(IntervalSub sub);

	// bTAMA6 C-BOMP
	void forward_match_accurate(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);
	void forward_match_parallel(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	// bTAMA7
	void backward1_match_accurate(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	// bTAMA8
	void backward2_match_accurate(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	//void match_vague(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);
	int calMemory();
};

#endif //TAMA_H
