#ifndef bTAMA_H
#define bTAMA_H

#include "Util.h"
#include "constant.h"
#include <bitset>
#include "ThreadPool.h"
using namespace std;

//const int MAX_ATTS = 101;
// Variants of TAMA_FORWARD algorithm
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
	match_forward_CBOMP_accurate(int p, int att, int l, int r, const int value, int lvl, const vector<IntervalSub>& subList, bitset<
		subs>& mB);

	void
	match_backward2_DMFT_TAMA_CBOMP_accurate(int p, int att, int l, int r, const int value, int lvl, const vector<
		IntervalSub>& subList, bitset<
		subs>& mB);

	void
	match_backward_native(const int& p, const int& att, const int& l, const int& r, const int& value, const int& lvl, \
    const vector<IntervalSub>& subList, bitset<subs>& gB);

	void
	match_forward_DMFT_bTAMA(const int& p, const int& att, const int& l, const int& r, const int& value, const int& lvl, \
    const vector<IntervalSub>& subList, bitset<subs>& mB);
	//void match_vague(int p, int att, int l, int r, const int value, int lvl);

 public:
	bTama(int32_t);

	void insert(IntervalSub sub);

	bool deleteSubscription(IntervalSub sub);

	// TAMA6-C-BOMP  TAMA6-CF
	void match_forward_CBOMP_accurate(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);
	// TAMA6-CFP
	void match_forward_CBOMP_parallel(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	// TAMA7-C-BOMP TAMA_FORWARD-CB1
	void match_backward1_DMFT_TAMA_CBOMP_accurate(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	// TAMA8-C-BOMP TAMA_FORWARD-CB2
	void match_backward2_DMFT_TAMA_CBOMP_accurate(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);
	// TAMA_FORWARD-CBP2
	void
	match_backward2_DMFT_TAMA_CBOMP_accurate_parallel(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	// backward TAMA_FORWARD / bTAMA (from FBMAD)
	void insert_backward(IntervalSub sub);
	void match_backward_native(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);
	void match_forward_DMFT_bTAMA(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);

	//void match_vague(const Pub& pub, int& matchSubs, const vector<IntervalSub>& subList);
	int calMemory();
};

#endif //TAMA_H
